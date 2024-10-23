//===========================================================
//
// アイテム処理[item.cpp]
// Author 大原怜将
//
//===========================================================
#include "renderer.h"
#include "manager.h"
#include "item.h"
#include "objectX.h"
#include "debugproc.h"
#include "object.h"
#include "itemmanager.h"
#include "game.h"
#include "collision.h"
#include "player.h"
#include "billboard.h"
#include "texture.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CItem *CItem::m_pTop = nullptr;
CItem *CItem::m_pCur = nullptr;

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const D3DXVECTOR2 TEX_SIZE = { 15.0f, 15.0f };           // UIのサイズ
	const float RADIUS = 40.0f;                                    // 半径
	const char *ITEM_TEXT = "data\\TEXT\\item.txt";          // アイテムの配置
	const char* GRAP_BOTTON = "data\\TEXTURE\\Ybutton.png";  // アイテムを掴めるときに出すUIのテクスチャパス
}

//================================================================
// コンストラクタ
//================================================================
CItem::CItem()
{
	m_pBill = nullptr;

	if (m_pTop != nullptr)
	{// 先頭が存在している場合

		m_pCur->m_pNext = this;
		m_pPrev = m_pCur;
		m_pCur = this;
	}
	else
	{// 存在しない場合

		m_pTop = this;
		m_pCur = this;
	}
}

//================================================================
// コンストラクタ(オーバーロード)
//================================================================
CItem::CItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE Type, int nIdx, const char *aModelFliename) : CObjectX(aModelFliename)
{
	SetPosition(pos);
	SetRotition(rot);
	m_pBill = nullptr;

	if (m_pTop != nullptr)
	{// 先頭が存在している場合

		m_pCur->m_pNext = this;
		m_pPrev = m_pCur;
		m_pCur = this;
	}
	else
	{// 存在しない場合

		m_pTop = this;
		m_pCur = this;
	}
}

//================================================================
// デストラクタ
//================================================================
CItem::~CItem()
{

}

//================================================================
// 生成処理
//================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type, int nIdx, const char *aModelFliename)
{
	// アイテムのポインタ
	CItem *pItem = nullptr;

	if (pItem == nullptr)
	{
		// 生成処理
		pItem = new CItem(pos, rot, type, nIdx, aModelFliename);

		// 初期化処理
		pItem->Init();
		pItem->SetType(type);
		pItem->SetID(nIdx);
	}
	
	return pItem;
}

//================================================================
// デーブルとの当たり判定
//================================================================
void CItem::Collision(D3DXVECTOR3* pos, D3DXVECTOR3* posOld, float fRadius)
{
	if (m_Type == TYPE_TABLE || m_Type == TYPE_BIKE)
	{
		D3DXVECTOR3 vtxMax = GetVtxMax();
		D3DXVECTOR3 vtxMin = GetVtxMin();

		D3DXVECTOR3 Itempos = GetPosition();

		if (pos->z + fRadius > Itempos.z + vtxMin.z
			&& pos->z + -fRadius < Itempos.z + vtxMax.z)
		{
			//ブロックの右側面==================================
			if (pos->x + -fRadius <= Itempos.x + vtxMax.x
				&& posOld->x + -fRadius >= Itempos.x + vtxMax.x)
			{
				pos->x = (Itempos.x + vtxMax.x) - -fRadius;
			}
			//ブロックの左側面==================================
			if (pos->x + fRadius >= Itempos.x + vtxMin.x
				&& posOld->x + fRadius <= Itempos.x + vtxMin.x)
			{
				pos->x = (Itempos.x + vtxMin.x) - fRadius;
			}
		}

		if (pos->x + fRadius > Itempos.x + vtxMin.x
			&& pos->x + -fRadius < Itempos.x + vtxMax.x)
		{
			//ブロックの上======================================
			if (pos->z - fRadius - fRadius <= Itempos.z + vtxMax.z
				&& posOld->z - fRadius >= Itempos.z + vtxMax.z)
			{
				pos->z = (Itempos.z + vtxMax.z) + fRadius + fRadius;
			}

			//ブロックの下======================================
			if (pos->z + fRadius >= Itempos.z + vtxMin.z
				&& posOld->z + fRadius <= Itempos.z + vtxMin.z)
			{
				pos->z = (Itempos.z + vtxMin.z) - fRadius;
			}
		}
	}
}

//================================================================
// 初期化処理
//================================================================
HRESULT CItem::Init(void)
{
	CObjectX::Init();

	return S_OK;
}

//================================================================
// 終了処理
//================================================================
void CItem::Uninit(void)
{
	// リストから自分自身を削除する
	if (m_pTop == this)
	{// 自身が先頭

		if (m_pNext != nullptr)
		{// 次が存在している

			m_pTop = m_pNext;	// 次を先頭にする
			m_pNext->m_pPrev = nullptr;	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない

			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == this)
	{// 自身が最後尾

		if (m_pPrev != nullptr)
		{// 次が存在している

			m_pCur = m_pPrev;			// 前を最後尾にする
			m_pPrev->m_pNext = nullptr;	// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない

			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else
	{
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
		}
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
		}
	}

	CObjectX::Uninit();
}

//================================================================
// 更新処理
//================================================================
void CItem::Update(void)
{
	CObjectX::Update();

	if (CPlayer::GetInstance() == nullptr)
		return;

	if (CGame::GetCollision()->Circle(GetPosition(), CPlayer::GetInstance()->GetPosition(), RADIUS, 40.0f) == true)
	{// 範囲内

		if (m_Type != TYPE_MICROWAVE && m_Type != TYPE_TABLE && m_Type != TYPE_POSTER && CPlayer::GetInstance()->GetGrapItem() == nullptr 
		 && CPlayer::GetInstance()->GetMobility() != CPlayer::Immobile && m_pBill == nullptr)
		{// 種類が電子レンジかつテーブル以外

			// 生成して位置、サイズ、描画するかどうか、使用するテクスチャ設定
			m_pBill = CBillBoard::Create();
			m_pBill->SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y + 40.0f, GetPosition().z));
			m_pBill->SetSize(TEX_SIZE.x, TEX_SIZE.y);
			m_pBill->SetDraw();
			m_pBill->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(GRAP_BOTTON));
		}
	}
	
	if((CGame::GetCollision()->Circle(GetPosition(), CPlayer::GetInstance()->GetPosition(), 40.0f, 40.0f) != true || CPlayer::GetInstance()->GetGrapItem() != nullptr)
	 && m_pBill != nullptr)
	{// 範囲外

		// 終了
		m_pBill->Uninit();
		m_pBill = nullptr;
	}
}