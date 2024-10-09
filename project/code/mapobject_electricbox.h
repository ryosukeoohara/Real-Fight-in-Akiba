//===========================================================
//
// マップに配置れている電機ボックス処理[mapobject_electricbox.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAPOBJECT_ELECTRICBOX_H__             //このマクロ定義がされてなかったら
#define _MAPOBJECT_ELECTRICBOX_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "mapobject.h"

// 前方宣言
class CElectricBoxState;

//===========================================================
// ゴミ箱クラス定義
//===========================================================
class CMapObject_ElectricBox : public CMapObject
{
public:
	CMapObject_ElectricBox();
	CMapObject_ElectricBox(const char* aModelFliename, int nPriority = 3);
	~CMapObject_ElectricBox();

	HRESULT Init(void);           // 初期化処理    
	void Uninit(void);            // 終了処理
	void Update(void);            // 更新処理
	void Draw(void);              // 描画処理

	static CMapObject_ElectricBox* Create(const char* aModelFliename, int nPriority = 3);  //生成
	void ChangeState(CElectricBoxState* pState);  // ステイトを変更

	// 取得
	CMapObject_ElectricBox* GetTop(void) { return m_pTop; }  // リストの先頭
	int GetFallDownCount(void) { return m_nFallDownCount; }

	// 設定
	void SetFallDown(int nNum) { m_nFallDownCount = nNum; }
	void IsFallDown(bool bValue) { m_bFallDown = bValue; }

private:

	CElectricBoxState* m_pState;  // ステイトのポインタ

	static CMapObject_ElectricBox* m_pTop;  // 先頭のオブジェクトへのポインタ
	static CMapObject_ElectricBox* m_pCur;  // 最後尾のオブジェクトへのポインタ
	CMapObject_ElectricBox* m_pNext;        // 次のオブジェクトへのポインタ
	CMapObject_ElectricBox* m_pPrev;        // 前のオブジェクトへのポインタ

	int m_nFallDownCount = 0;
	bool m_bFallDown = false;
};

//===========================================================
// ステイト
//===========================================================
class CElectricBoxState
{
public:
	CElectricBoxState() {}
	~CElectricBoxState() {}

	virtual void Update(CMapObject_ElectricBox* pElectricBox) = 0;

private:

};

// ニュートラル
class CElectricNeutral : public CElectricBoxState
{
public:
	CElectricNeutral();
	~CElectricNeutral() {}

	void Update(CMapObject_ElectricBox* pElectricBox) override;

private:

};

// 倒れる
class CElectricFallDown : public CElectricBoxState
{
public:
	CElectricFallDown();
	~CElectricFallDown() {}

	void Update(CMapObject_ElectricBox* pElectricBox) override;

private:

	D3DXVECTOR3 PlayerPos;

};

// 殴られた
class CElectrictBeaten : public CElectricBoxState
{
public:
	CElectrictBeaten();
	~CElectrictBeaten() {}

	void Update(CMapObject_ElectricBox* pElectricBox) override;

private:

	int m_nShakeTimeCounter;
	float m_fShakeAngle;
};

#endif