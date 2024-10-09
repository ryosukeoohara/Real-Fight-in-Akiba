//===========================================================
//
// マップに配置れているゴミ箱処理[mapobject_trashbox.cpp]
// Author 大原怜将
//
//===========================================================
#include "mapobject_trashbox.h"
#include "player.h"
#include "collision.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CMapObject_TrashBox* CMapObject_TrashBox::m_pTop = nullptr;      // 先頭のポインタ
CMapObject_TrashBox* CMapObject_TrashBox::m_pCur = nullptr;      // 最後尾のポインタ

//===========================================================
// コンストラクタ
//===========================================================
CMapObject_TrashBox::CMapObject_TrashBox()
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
CMapObject_TrashBox::CMapObject_TrashBox(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
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
CMapObject_TrashBox::~CMapObject_TrashBox()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMapObject_TrashBox::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject_TrashBox::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CMapObject_TrashBox::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// プレイヤーとの当たり判定：プレイヤーが範囲内に入った時
	if (CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f))
		ChangeState(new CTrashBoxBlowAway);

	// ステートの更新
	if (m_pState != nullptr)
		m_pState->Update(this);
}

//===========================================================
// 描画処理
//===========================================================
void CMapObject_TrashBox::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// 生成処理
//===========================================================
CMapObject_TrashBox* CMapObject_TrashBox::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_TrashBox* pTrashBox = new CMapObject_TrashBox(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// ステートの変更処理
//===========================================================
void CMapObject_TrashBox::ChangeState(CTrashBoxState* pState)
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
// 吹き飛び状態の処理
//===========================================================
CTrashBoxBlowAway::CTrashBoxBlowAway()
{

}

//===========================================================
// 吹き飛び状態の更新処理
//===========================================================
void CTrashBoxBlowAway::Update(CMapObject_TrashBox* pTrashBox)
{
	D3DXVECTOR3 Playerrot = CPlayer::GetInstance()->GetRotition();
	D3DXVECTOR3 Playermove = CPlayer::GetInstance()->GetMove();

	CObjectX::INFO* pInfo = pTrashBox->GetInfo();

	pInfo->rot.x = 1.57f;
	pInfo->rot.y = Playerrot.y;
	pInfo->rot.z = 1.57f;

	pInfo->move.x += (Playermove.x);
	pInfo->move.y += 1.0f;
	pInfo->move.z += (Playermove.z);

	if (pInfo->pos.y - pInfo->vtxMax.x >= 0.0f)
	{
		pInfo->pos.y -= 1.0f;
		pTrashBox->ChangeState(new CTrashBoxBound);
	}

	if (pInfo->pos.y - pInfo->vtxMax.x <= 0.0f)
	{
		pInfo->pos.y = pInfo->vtxMax.x;
		pTrashBox->ChangeState(new CTrashBoxBound);
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
CTrashBoxBound::CTrashBoxBound()
{

}

//===========================================================
// バウンド状態の更新処理
//===========================================================
void CTrashBoxBound::Update(CMapObject_TrashBox* pTrashBox)
{
	CObjectX::INFO* pInfo = pTrashBox->GetInfo();

	if (pInfo->pos.y > 0.0f)
	{
		pInfo->pos.y -= 1.0f;
	}

	if (pInfo->pos.y < 0.0f)
	{
		pInfo->pos.y = 0.0f;
	}

	pInfo->pos += pInfo->move;

	// 移動量を更新(減衰させる)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}