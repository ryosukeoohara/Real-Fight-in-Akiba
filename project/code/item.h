//===========================================================
//
// アイテム処理[item.h]
// Author 大原怜将
//
//===========================================================
#ifndef _ITEM_H_             //このマクロ定義がされてなかったら
#define _ITEM_H_             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "objectX.h"

//===========================================================
// 前方宣言
//===========================================================
class CObjectX;
class CBillBoard;

//===========================================================
// アイテムクラス定義
//===========================================================
class CItem : public CObjectX
{
public:

	// 種類
	enum TYPE
	{
		TYPE_REF = 0,       // 冷蔵庫
		TYPE_BIKE,          // 自転車
		TYPE_MICROWAVE,     // 電子レンジ
		TYPE_TABLE,         // テーブル
		TYPE_POSTER,        // 看板
		TYPE_MAX
	};

	CItem();   // コンストラクタ
	CItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE Type, int nIdx, const char *aModelFliename);  // コンストラクタ(オーバーロード)
	~CItem();  // デストラクタ

	HRESULT Init(void);   // 初期化処理    
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE Type, int nIdx, const char *aModelFliename);
	void Collision(D3DXVECTOR3* pos, D3DXVECTOR3* posOld, float fRadius);

	// 設定系
	void SetType(TYPE type) { m_Type = type; }
	void SetID(int id) { m_nID = id; }
	
	// 取得系
	CObjectX **GetObjectX(void) { return m_appObjectX; }
	TYPE GetType(void) { return m_Type; }
	int GetID(void) { return m_nID; }
	static CItem *GetTop(void) { return m_pTop; }
	CItem *GetNext(void) { return m_pNext; }
	float GetRadius(void) { return m_fRadius; }

private:
	CObjectX **m_appObjectX;
	CBillBoard *m_pBill;
	static CItem *m_pTop;  // 先頭のオブジェクトへのポインタ
	static CItem *m_pCur;  // 最後尾のオブジェクトへのポインタ
	CItem *m_pNext;
	CItem *m_pPrev;        // 前のオブジェクトへのポインタ
	TYPE m_Type;
	int m_nID;
	float m_fRadius;       // 半径
	static const char *m_apTexName[2];
};

#endif