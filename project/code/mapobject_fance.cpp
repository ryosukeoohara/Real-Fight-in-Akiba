//===========================================================
//
// マップに配置れている柵処理[mapobject_fance.cpp]
// Author 大原怜将
//
//===========================================================
#include "mapobject_fance.h"
#include "player.h"
#include "collision.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CMapObject_Fance* CMapObject_Fance::m_pTop = nullptr;      // 先頭のポインタ
CMapObject_Fance* CMapObject_Fance::m_pCur = nullptr;      // 最後尾のポインタ

//===========================================================
// コンストラクタ
//===========================================================
CMapObject_Fance::CMapObject_Fance()
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
CMapObject_Fance::CMapObject_Fance(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
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
CMapObject_Fance::~CMapObject_Fance()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMapObject_Fance::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject_Fance::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CMapObject_Fance::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// プレイヤーとの当たり判定：プレイヤーが範囲内に入った時
	if (CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f))
		ChangeState(new CFanceBlowAway);  // ステートの変更：吹き飛び

	// ステートの更新
	if (m_pState != nullptr)
		m_pState->Update(this);
}

//===========================================================
// 描画処理
//===========================================================
void CMapObject_Fance::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// 生成処理
//===========================================================
CMapObject_Fance* CMapObject_Fance::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_Fance* pFance = new CMapObject_Fance(aModelFliename, nPriority);

	if (pFance != nullptr)
		pFance->Init();

	return pFance;
}

//===========================================================
// ステートの変更処理
//===========================================================
void CMapObject_Fance::ChangeState(CFanceState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//==================================================================
// ステート
//==================================================================
//===========================================================
// 吹き飛び状態の処理
//===========================================================
CFanceBlowAway::CFanceBlowAway()
{

}

//===========================================================
// 吹き飛び状態の更新処理
//===========================================================
void CFanceBlowAway::Update(CMapObject_Fance* pFance)
{
	CObjectX::INFO* pInfo = pFance->GetInfo();

	if (pInfo->rot.x >= 1.3f || pInfo->rot.x <= -1.3f)
	{
		pFance->ChangeState(new CFanceNeutral);

		m_fFallDownSpeed = 0.0f;
	}

	D3DXVECTOR3 move = {};

	move = CPlayer::GetInstance()->GetMove();

	m_fFallDownSpeed = (move.x + move.x) - (move.z + move.z);
	
	if (m_fFallDownSpeed >= 8.0f || m_fFallDownSpeed <= -8.0f)
	{// プレイヤーの速度が一定より速かったら

		move = CPlayer::GetInstance()->GetMove();
		pInfo->move.x += move.x;
		pInfo->move.z += move.z;
	}

	pInfo->rot.x -= (m_fFallDownSpeed * 0.01f);
}

//===========================================================
// ニュートラル状態の処理
//===========================================================
CFanceNeutral::CFanceNeutral()
{

}

//===========================================================
// ニュートラル状態の更新処理
//===========================================================
void CFanceNeutral::Update(CMapObject_Fance* pFance)
{

}