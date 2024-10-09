//===========================================================
//
// マップに配置れているゴミ箱処理[mapobject_trashbox.cpp]
// Author 大原怜将
//
//===========================================================
#include "mapobject_electricbox.h"
#include "player.h"
#include "collision.h"
#include "utility.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CMapObject_ElectricBox* CMapObject_ElectricBox::m_pTop = nullptr;      // 先頭のポインタ
CMapObject_ElectricBox* CMapObject_ElectricBox::m_pCur = nullptr;      // 最後尾のポインタ

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
CMapObject_ElectricBox::CMapObject_ElectricBox()
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
CMapObject_ElectricBox::CMapObject_ElectricBox(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
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
CMapObject_ElectricBox::~CMapObject_ElectricBox()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMapObject_ElectricBox::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject_ElectricBox::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CMapObject_ElectricBox::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// ステートの更新
	if (m_pState != nullptr)
		m_pState->Update(this);

	// プレイヤーが攻撃をしていて、その攻撃が発生していたら
	if (pPlayer->GetState() == CPlayer::STATE_ATTACK && pPlayer->IsHitCollision() && !m_bFallDown)
	{
		// 攻撃が当たっていたら
		if (CCollision::GetInstance()->HitOBJ(pPlayer, this, 30.0f, 30.0f))
		{
			// ステートの変更：殴られた状態
			ChangeState(new CElectrictBeaten);
		}
	}

	CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f);
}

//===========================================================
// 描画処理
//===========================================================
void CMapObject_ElectricBox::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// 生成処理
//===========================================================
CMapObject_ElectricBox* CMapObject_ElectricBox::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_ElectricBox* pTrashBox = new CMapObject_ElectricBox(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// 状態の変更
//===========================================================
void CMapObject_ElectricBox::ChangeState(CElectricBoxState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//======================================================================
// ステート
//======================================================================
//===========================================================
// ニュートラル状態
//===========================================================
CElectricNeutral::CElectricNeutral()
{

}

//===========================================================
// ニュートラル状態の更新処理
//===========================================================
void CElectricNeutral::Update(CMapObject_ElectricBox* pElectricBox)
{

}

//===========================================================
// 倒れている状態
//===========================================================
CElectricFallDown::CElectricFallDown()
{
	PlayerPos = CPlayer::GetInstance()->GetPosition();
}

//===========================================================
// 倒れている更新処理
//===========================================================
void CElectricFallDown::Update(CMapObject_ElectricBox* pElectricBox)
{
	CObjectX::INFO* pInfo = pElectricBox->GetInfo();

	if ((pInfo->rot.x >= D3DX_PI * 0.5f || pInfo->rot.x <= -D3DX_PI * 0.5f))
	{
		// ステートの変更：ニュートラル
		pElectricBox->ChangeState(new CElectricNeutral);
	}

	if (PlayerPos.x >= pInfo->pos.x && PlayerPos.z >= pInfo->pos.z)
	{
		pInfo->rot.x -= 0.1f;
	}

	if (PlayerPos.x >= pInfo->pos.x && PlayerPos.z <= pInfo->pos.z)
	{
		pInfo->rot.x -= 0.1f;
	}

	if (PlayerPos.x <= pInfo->pos.x && PlayerPos.z >= pInfo->pos.z)
	{
		pInfo->rot.x += 0.1f;
	}

	if (PlayerPos.x <= pInfo->pos.x && PlayerPos.z <= pInfo->pos.z)
	{
		pInfo->rot.x += 0.1f;
	}

	// vtxの変更
	utility::ChangeVtx(&pInfo->vtxMax, &pInfo->vtxMini, pInfo->rot);
}

//===========================================================
// 殴られた状態の処理
//===========================================================
CElectrictBeaten::CElectrictBeaten()
{
	m_nShakeTimeCounter = 0;
	m_fShakeAngle = 0.0f;
}

//===========================================================
// 殴られた状態の更新処理
//===========================================================
void CElectrictBeaten::Update(CMapObject_ElectricBox* pElectricBox)
{
	CObjectX::INFO* pInfo = pElectricBox->GetInfo();

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
		int nNum = pElectricBox->GetFallDownCount();
		nNum++;
		pElectricBox->SetFallDown(nNum);

		if (nNum >= FALL_DOWN)
		{
			// ステートの変更：倒れる
			pElectricBox->ChangeState(new CElectricFallDown);
			pElectricBox->IsFallDown(true);
		}
		else
		{
			// ステートの変更：ニュートラル
			pElectricBox->ChangeState(new CElectricNeutral);
		}
	}

	pInfo->pos += pInfo->move;

	// 移動量を更新(減衰させる)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}