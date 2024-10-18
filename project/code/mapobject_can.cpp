//===========================================================
//
// マップに配置れている缶処理[mapobject_can.cpp]
// Author 大原怜将
//
//===========================================================
#include "mapobject_can.h"
#include "player.h"
#include "collision.h"
#include "utility.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CMapObject_Can* CMapObject_Can::m_pInstance = nullptr;
CMapObject_Can* CMapObject_Can::m_pTop = nullptr;
CMapObject_Can* CMapObject_Can::m_pCur = nullptr;

//===========================================================
// コンストラクタ
//===========================================================
CMapObject_Can::CMapObject_Can()
{
	m_pInstance = this;

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
CMapObject_Can::CMapObject_Can(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
{
	m_pInstance = this;

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
CMapObject_Can::~CMapObject_Can()
{
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMapObject_Can::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject_Can::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CMapObject_Can::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// プレイヤーとの当たり判定：プレイヤーが範囲内に入った時
	if (CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f) && !m_b)
	{
		ChangeState(new CCanBlowAway);
		m_b = true;
	}
		

	// ステートの更新
	if (m_pState != nullptr)
		m_pState->Update(this);
}

//===========================================================
// 描画処理
//===========================================================
void CMapObject_Can::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// ステートの変更処理
//===========================================================
void CMapObject_Can::ChangeState(CCanState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//===========================================================
// 生成処理
//===========================================================
CMapObject_Can* CMapObject_Can::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_Can* pCan = new CMapObject_Can(aModelFliename, nPriority);

	if (pCan != nullptr)
	{
		pCan->Init();
	}

	return pCan;
}

//=======================================================================
// ステート
//=======================================================================
//===========================================================
// 吹き飛ぶ状態
//===========================================================
CCanBlowAway::CCanBlowAway()
{
	
}

//===========================================================
// 吹き飛ぶ状態の更新処理
//===========================================================
void CCanBlowAway::Update(CMapObject_Can* pCan)
{
	D3DXVECTOR3 Playerrot = CPlayer::GetInstance()->GetRotition();
	D3DXVECTOR3 Playermove = CPlayer::GetInstance()->GetMove();
	
	CObjectX::INFO *pInfo = pCan->GetInfo();

	pInfo->rot.x = D3DX_PI * 0.5f;
	pInfo->rot.y = Playerrot.y;
	pInfo->rot.z = D3DX_PI * 0.5f;

	pInfo->move.x += (Playermove.x * 1.2f);
	pInfo->move.y += 1.0f;
	pInfo->move.z += (Playermove.z * 1.2f);

	if (pInfo->pos.y - pInfo->vtxMax.x > 0.0f)
	{
		pInfo->move.y -= 1.0f;
		pCan->ChangeState(new CCanBound);
	}

	if (pInfo->pos.y < 0.0f)
	{
		pInfo->pos.y = 0.0f;
		pInfo->move.y = 0.0f;
		pCan->ChangeState(new CCanBound);
	}

	pInfo->pos += pInfo->move;

	// 移動量を更新(減衰させる)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}

//===========================================================
// バウンド状態の処理
//===========================================================
CCanBound::CCanBound()
{
	m_fTime = 1.0f;
}

//===========================================================
// バウンド状態の更新処理
//===========================================================
void CCanBound::Update(CMapObject_Can* pCan)
{
	CObjectX::INFO* pInfo = pCan->GetInfo();

	//if (pInfo->pos.y > 0.0f)
	//{
	//	pInfo->pos.y -= 1.0f;
	//}

	//if (pInfo->pos.y < 0.0f)
	//{
	//	pInfo->pos.y = 0.0f;
	//}

	pInfo->pos += pInfo->move;

	// 移動量を更新(減衰させる)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;

	pInfo->pos.y = easing::EaseInBounce(m_fTime);

	if (m_fTime > 0.0f)
		m_fTime -= 0.1f;
}

//===========================================================
// 投げられた状態の処理
//===========================================================
CCanThrow::CCanThrow()
{

}

//===========================================================
// 投げられた状態の更新処理
//===========================================================
void CCanThrow::Update(CMapObject_Can* pCan)
{
	CObjectX::INFO* pInfo = pCan->GetInfo();

	pInfo->move.x -= sinf(pInfo->rot.y);
	pInfo->move.y -= 1.0f;
	pInfo->move.z -= cosf(pInfo->rot.y);

	pInfo->pos += pInfo->move;

	if (pInfo->pos.y <= 0.0f)
		pCan->ChangeState(new CCanBound);

	// 移動量を更新(減衰させる)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}