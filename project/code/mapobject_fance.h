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


// 前方宣言
class CFanceState;

//===========================================================
// 柵クラス定義
//===========================================================
class CMapObject_Fance : public CObjectX
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

private:

};

//===========================================================
// ステイト
//===========================================================
class CFanceState
{
public:
	CFanceState() {}
	~CFanceState() {}

	virtual void Update(CFanceState* pFance) = 0;

private:

};

// 吹き飛ぶ
class CFanceBlowAway : public CFanceState
{
public:
	CFanceBlowAway();
	~CFanceBlowAway() {}

	void Update(CFanceState* pFance) override;

private:

};

// バウンド
class CFanceFallDown : public CFanceState
{
public:
	CFanceFallDown();
	~CFanceFallDown() {}

	void Update(CFanceState* pFance) override;

private:

};

#endif