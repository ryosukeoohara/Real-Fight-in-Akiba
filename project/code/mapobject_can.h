//===========================================================
//
// マップに配置れている缶処理[mapobject_can.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAPOBJECT_CAN_H__             //このマクロ定義がされてなかったら
#define _MAPOBJECT_CAN_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// 前方宣言
class CCanState;

//===========================================================
// 缶クラス定義
//===========================================================
class CMapObject_Can : public CMapObject
{
public:
	CMapObject_Can();
	CMapObject_Can(const char* aModelFliename, int nPriority = 3);
	~CMapObject_Can();

	HRESULT Init(void);           // 初期化処理    
	void Uninit(void);            // 終了処理
	void Update(void);            // 更新処理
	void Draw(void);              // 描画処理

	static CMapObject_Can* Create(const char* aModelFliename, int nPriority = 3);  //生成
	void ChangeState(CCanState* pState);
	static CMapObject_Can* GetInstance(void) { return m_pInstance; }  // 自分自身

	// 取得
	CMapObject_Can* GetTop(void) { return m_pTop; }  // リストの先頭

private:

	static CMapObject_Can* m_pInstance;
	CCanState* m_pState;

	static CMapObject_Can* m_pTop;  // 先頭のオブジェクトへのポインタ
	static CMapObject_Can* m_pCur;  // 最後尾のオブジェクトへのポインタ
	CMapObject_Can* m_pNext;        // 次のオブジェクトへのポインタ
	CMapObject_Can* m_pPrev;        // 前のオブジェクトへのポインタ

};

//===========================================================
// ステイト
//===========================================================
class CCanState
{
public:
	CCanState() {}
	~CCanState() {}

	virtual void Update(CMapObject_Can* pCan) = 0;

private:

};

// 吹き飛ぶ
class CCanBlowAway : public CCanState
{
public:
	CCanBlowAway();
	~CCanBlowAway() {}

	void Update(CMapObject_Can* pCan) override;

private:

	D3DXVECTOR3 m_move;

};

// バウンド
class CCanBound : public CCanState
{
public:
	CCanBound();
	~CCanBound() {}

	void Update(CMapObject_Can* pCan) override;

private:

};

#endif