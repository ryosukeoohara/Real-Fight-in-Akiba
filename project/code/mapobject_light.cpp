//===========================================================
//
// マップに配置れている外灯処理[mapobject_light.cpp]
// Author 大原怜将
//
//===========================================================
#include "mapobject_light.h"
#include "mapobject_brokenlight.h"
#include "player.h"
#include "collision.h"
#include "particle.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CMapObject_Light* CMapObject_Light::m_pTop = nullptr;      // 先頭のポインタ
CMapObject_Light* CMapObject_Light::m_pCur = nullptr;      // 最後尾のポインタ

//===========================================================
// 定数定義
//===========================================================
namespace
{
	int FALL_DOWN = 3;  // ３回殴られたら倒れる
	int SHAKE_TIME = 60;  // 殴られてから揺れている時間
}


//===========================================================
// コンストラクタ
//===========================================================
CMapObject_Light::CMapObject_Light()
{
	if (m_pTop != nullptr)
	{// 先頭が存在している場合

		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合

		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}
}

//===========================================================
// コンストラクタ
//===========================================================
CMapObject_Light::CMapObject_Light(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
{
	if (m_pTop != nullptr)
	{// 先頭が存在している場合

		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合

		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}
}

//===========================================================
// デストラクタ
//===========================================================
CMapObject_Light::~CMapObject_Light()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMapObject_Light::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject_Light::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CMapObject_Light::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// ステートの更新
	if (m_pState != nullptr)
		m_pState->Update(this);

	// プレイヤーが攻撃をしていて、その攻撃が発生していたら
	if (pPlayer->GetState() == CPlayer::STATE_ATTACK && pPlayer->IsHitCollision() && !m_bLightFall)
	{
		// 攻撃が当たっていたら
		if (CCollision::GetInstance()->HitOBJ(pPlayer, this, 30.0f, 30.0f))
		{
			// ステートの変更：殴られた状態
			ChangeState(new CLightBeaten);
		}
	}

	CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f);
}

//===========================================================
// 描画処理
//===========================================================
void CMapObject_Light::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// 生成処理
//===========================================================
CMapObject_Light* CMapObject_Light::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_Light* pTrashBox = new CMapObject_Light(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// ステートの変更処理
//===========================================================
void CMapObject_Light::ChangeState(CLightState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//=======================================================================
// ステート
//=======================================================================
//===========================================================
// 殴られた状態の処理
//===========================================================
CLightBeaten::CLightBeaten()
{
	m_nShakeTimeCounter = 0;
	m_nFallDownCount = 0;
	m_fShakeAngle = 0.0f;
}

//===========================================================
// 殴られた状態の更新処理
//===========================================================
void CLightBeaten::Update(CMapObject_Light* pLighth)
{
	CObjectX::INFO* pInfo = pLighth->GetInfo();

	// sin を使用した揺らし座標の算出
	pInfo->move.y = 0.0f;

	pInfo->move.x = sinf(m_fShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter / SHAKE_TIME)) * 2.0f;

	pInfo->move.z = sinf(m_fShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter / SHAKE_TIME)) * 2.0f;

	// 揺らし処理に使用する sin に渡す角度の変更処理
	m_fShakeAngle += 2.0f * 1.0f;

	// 揺らす時間が経過したら揺らし処理を終了する
	m_nShakeTimeCounter += 1;
	if (m_nShakeTimeCounter >= SHAKE_TIME)
	{
		m_nShakeTimeCounter = 0;
		int nNum = pLighth->GetFallDownCount();
		nNum++;
		pLighth->SetFallDown(nNum);

		// ステートの変更：ニュートラル
		if (nNum >= FALL_DOWN)
		{
			pLighth->ChangeState(new CLightFall);

			pLighth->SetLightFall(true);
		}
		else
		{
			pLighth->ChangeState(new CLightNeutral);
		}
		
	}

	pInfo->pos += pInfo->move;

	// 移動量を更新(減衰させる)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}

//===========================================================
// 落下状態の処理
//===========================================================
CLightFall::CLightFall()
{
	
}

//===========================================================
// 落下状態の更新処理
//===========================================================
void CLightFall::Update(CMapObject_Light* pLighth)
{
	CObjectX::INFO* pInfo = pLighth->GetInfo();

	//if (m_pLightR == nullptr)
	//{
	//	m_pLightR = CObjectX::Create("data\\MODEL\\map\\light_rubble_L.x");
	//	m_pLightR->SetPosition(D3DXVECTOR3(pInfo->pos.x, 400.0f, pInfo->pos.z + 50.0f));
	//	m_pLightR->SetGravity(5.0f);
	//}

	//if (m_pLightL == nullptr)
	//{
	//	m_pLightL = CObjectX::Create("data\\MODEL\\map\\light_rubble_R.x");
	//	m_pLightL->SetPosition(D3DXVECTOR3(pInfo->pos.x, 400.0f, pInfo->pos.z - 50.0f));
	//	m_pLightL->SetGravity(5.0f);
	//}

	//if (m_pLightR != nullptr)
	//{
	//	CObjectX::INFO* pInfo = m_pLightR->GetInfo();
	//	
	//	if (pInfo->pos.y > 0.0f)
	//	{
	//		pInfo->pos.y -= 9.0f;
	//	}
	//	else
	//	{
	//		// ガラス片のパーティクルを生成
	//		CParticle::Create(pInfo->pos, CParticle::TYPE_GLASS);
	//	}
	//		
	//	pInfo->pos += pInfo->move;

	//	// 移動量を更新(減衰させる)
	//	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	//	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	//	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
	//}

	//if (m_pLightL != nullptr)
	//{
	//	CObjectX::INFO* pInfo = m_pLightL->GetInfo();

	//	if (pInfo->pos.y > 0.0f)
	//	{
	//		pInfo->pos.y -= 9.0f;
	//	}
	//	else
	//	{
	//		// ガラス片のパーティクルを生成
	//		CParticle::Create(pInfo->pos, CParticle::TYPE_GLASS);

	//		pLighth->ChangeState(new CLightNeutral);
	//	}

	//	pInfo->pos += pInfo->move;

	//	// 移動量を更新(減衰させる)
	//	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	//	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	//	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
	//}
	CMapObject_BrokenLight *pLightL = CMapObject_BrokenLight::Create("data\\MODEL\\map\\light_rubble_L.x");
	pLightL->SetPosition(D3DXVECTOR3(pInfo->pos.x + 50.0f, 400.0f, pInfo->pos.z));
	pLightL->SetRotition(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pLightL->SetIdxModel(5);
	pLightL->SetVtx();

	CMapObject_BrokenLight* pLightR = CMapObject_BrokenLight::Create("data\\MODEL\\map\\light_rubble_R.x");
	pLightR->SetPosition(D3DXVECTOR3(pInfo->pos.x - 50.0f, 400.0f, pInfo->pos.z));
	pLightR->SetRotition(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pLightL->SetIdxModel(5);
	pLightR->SetVtx();

	pInfo->vtxMax.x -= 50.0f;
	pInfo->vtxMini.x += 10.0f;
	
	pLighth->ChangeState(new CLightNeutral);
}

CLightNeutral::CLightNeutral()
{

}

void CLightNeutral::Update(CMapObject_Light* pLighth)
{

}