//===========================================================
//
// マップに配置れているドラム缶処理[mapobject_dramcan.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAPOBJECT_DRAMCAN_H__             //このマクロ定義がされてなかったら
#define _MAPOBJECT_DRAMCAN_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// 前方宣言
class CDramcanState;
class CObject3D;

//===========================================================
// ドラム缶クラス定義
//===========================================================
class CMapObject_Dramcan : public CMapObject
{
public:
	CMapObject_Dramcan();
	CMapObject_Dramcan(const char* aModelFliename, int nPriority = 3);
	~CMapObject_Dramcan();

	HRESULT Init(void);           // 初期化処理    
	void Uninit(void);            // 終了処理
	void Update(void);            // 更新処理
	void Draw(void);              // 描画処理

	static CMapObject_Dramcan* Create(const char* aModelFliename, int nPriority = 3);  //生成
	void ChangeState(CDramcanState* pState);  // ステイトを変更

	// 取得
	//static CMapObject_Dramcan* GetInstance(void) { return m_pDramcan; }
	CMapObject_Dramcan* GetTop(void) { return m_pTop; }  // リストの先頭

private:

	//static CMapObject_Dramcan* m_pDramcan;
	CDramcanState* m_pState;  // ステイトのポインタ

	static CMapObject_Dramcan* m_pTop;  // 先頭のオブジェクトへのポインタ
	static CMapObject_Dramcan* m_pCur;  // 最後尾のオブジェクトへのポインタ
	CMapObject_Dramcan* m_pNext;        // 次のオブジェクトへのポインタ
	CMapObject_Dramcan* m_pPrev;        // 前のオブジェクトへのポインタ
};

//===========================================================
// ステイト
//===========================================================
class CDramcanState
{
public:
	CDramcanState() {}
	~CDramcanState() {}

	virtual void Update(CMapObject_Dramcan* pDramcan) = 0;

private:

};

// ニュートラル
class CDramcanNeutral : public CDramcanState
{
public:
	CDramcanNeutral();
	~CDramcanNeutral() {}

	void Update(CMapObject_Dramcan* pDramcan) override;

private:

};

// 吹き飛ぶ
class CDramcanBlowAway : public CDramcanState
{
public:
	CDramcanBlowAway();
	~CDramcanBlowAway() {}

	void Update(CMapObject_Dramcan* pDramcan) override;

private:

};

// ドラム缶から水がもれる
class CDramcanWaterLeak : public CDramcanState
{
public:
	CDramcanWaterLeak();
	~CDramcanWaterLeak() {}

	void Update(CMapObject_Dramcan* pDramcan) override;

private:

	CObject3D* m_pPuddle = nullptr;

};

#endif