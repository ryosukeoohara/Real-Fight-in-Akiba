//===========================================================
//
// マップに配置れている破壊された外灯処理[mapobject_brokenlight.cpp]
// Author 大原怜将
//
//===========================================================
#include "mapobject_brokenlight.h"
#include "player.h"
#include "collision.h"
#include "particle.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CMapObject_BrokenLight* CMapObject_BrokenLight::m_pTop = nullptr;      // 先頭のポインタ
CMapObject_BrokenLight* CMapObject_BrokenLight::m_pCur = nullptr;      // 最後尾のポインタ

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
CMapObject_BrokenLight::CMapObject_BrokenLight()
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
CMapObject_BrokenLight::CMapObject_BrokenLight(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
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
CMapObject_BrokenLight::~CMapObject_BrokenLight()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMapObject_BrokenLight::Init(void)
{
	CMapObject::Init();

	ChangeState(new CBrokenLightFall);

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject_BrokenLight::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CMapObject_BrokenLight::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// ステートの更新
	if (m_pState != nullptr)
		m_pState->Update(this);

	CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f);
}

//===========================================================
// 描画処理
//===========================================================
void CMapObject_BrokenLight::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// 生成処理
//===========================================================
CMapObject_BrokenLight* CMapObject_BrokenLight::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_BrokenLight* pTrashBox = new CMapObject_BrokenLight(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// ステートの変更処理
//===========================================================
void CMapObject_BrokenLight::ChangeState(CBrokenLightState* pState)
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
// ニュートラル状態の処理
//===========================================================
CBrokenLightNeutral::CBrokenLightNeutral()
{

}

//===========================================================
// ニュートラル状態の更新処理
//===========================================================
void CBrokenLightNeutral::Update(CMapObject_BrokenLight* pLighth)
{

}

//===========================================================
// 落下状態の処理
//===========================================================
CBrokenLightFall::CBrokenLightFall()
{

}

//===========================================================
// 落下状態の更新処理
//===========================================================
void CBrokenLightFall::Update(CMapObject_BrokenLight* pLighth)
{
	CObjectX::INFO* pInfo = pLighth->GetInfo();

	if (pInfo->pos.y > 0.0f)
	{
		pInfo->move.y -= 1.5f;
	}
	else
	{
		// ガラス片のパーティクルを生成
		CParticle::Create(pInfo->pos, CParticle::TYPE_GLASS);

		pLighth->ChangeState(new CBrokenLightNeutral);
	}

	pInfo->pos += pInfo->move;

	// 移動量を更新(減衰させる)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}