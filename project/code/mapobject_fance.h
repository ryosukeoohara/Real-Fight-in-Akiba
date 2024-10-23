//===========================================================
//
// マップに配置れている柵処理[mapobject_fance.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAPOBJECT_FANCE_H__             //このマクロ定義がされてなかったら
#define _MAPOBJECT_FANCE_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// 前方宣言
class CFanceState;

//===========================================================
// 柵クラス定義
//===========================================================
class CMapObject_Fance : public CMapObject
{
public:
	CMapObject_Fance();
	CMapObject_Fance(const char* aModelFliename, int nPriority = 3);
	~CMapObject_Fance();

	HRESULT Init(void);           // 初期化処理    
	void Uninit(void);            // 終了処理
	void Update(void);            // 更新処理
	void Draw(void);              // 描画処理

	static CMapObject_Fance* Create(const char* aModelFliename, int nPriority = 3);  //生成
	void ChangeState(CFanceState* pState);  // ステートの変更

	// 取得
	static CMapObject_Fance* GetTop(void) { return m_pTop; }  // リストの先頭

private:

	CFanceState* m_pState;  // ステートのポインタ

	static CMapObject_Fance* m_pTop;  // 先頭のオブジェクトへのポインタ
	static CMapObject_Fance* m_pCur;  // 最後尾のオブジェクトへのポインタ
	CMapObject_Fance* m_pNext;        // 次のオブジェクトへのポインタ
	CMapObject_Fance* m_pPrev;        // 前のオブジェクトへのポインタ

	bool m_bHit;  // 当たったかどうか
};

//===========================================================
// ステート
//===========================================================
class CFanceState
{
public:
	CFanceState() {}
	~CFanceState() {}

	virtual void Update(CMapObject_Fance* pFance) = 0;

private:

};

// 吹き飛ぶ
class CFanceBlowAway : public CFanceState
{
public:
	CFanceBlowAway();
	~CFanceBlowAway() {}

	void Update(CMapObject_Fance* pFance) override;

private:

	float m_fFallDownSpeed = 0.0f;

};

// バウンド
class CFanceNeutral : public CFanceState
{
public:
	CFanceNeutral();
	~CFanceNeutral() {}

	void Update(CMapObject_Fance* pFance) override;

private:

};

#endif