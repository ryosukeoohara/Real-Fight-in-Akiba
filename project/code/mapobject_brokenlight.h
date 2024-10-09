//===========================================================
//
// マップに配置れている破壊された外灯処理[mapobject_brokenlight.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAPOBJECT_BROKENLIGHT_H__             //このマクロ定義がされてなかったら
#define _MAPOBJECT_BROKENLIGHT_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// 前方宣言
class CBrokenLightState;

//===========================================================
// 外灯クラス定義
//===========================================================
class CMapObject_BrokenLight : public CMapObject
{
public:
	CMapObject_BrokenLight();
	CMapObject_BrokenLight(const char* aModelFliename, int nPriority = 3);
	~CMapObject_BrokenLight();

	HRESULT Init(void);   // 初期化処理    
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理
	void Draw(void);      // 描画処理

	static CMapObject_BrokenLight* Create(const char* aModelFliename, int nPriority = 3);  //生成
	void ChangeState(CBrokenLightState* pState);  // ステートを変更

	// 取得
	CMapObject_BrokenLight* GetTop(void) { return m_pTop; }  // リストの先頭
	int GetFallDownCount(void) { return m_nFallDownCount; }

	// 設定
	void SetFallDown(int nNum) { m_nFallDownCount = nNum; }
	void SetLightFall(bool nValue) { m_bLightFall = nValue; }

private:

	CBrokenLightState* m_pState;  // ステートのポインタ

	static CMapObject_BrokenLight* m_pTop;  // 先頭のオブジェクトへのポインタ
	static CMapObject_BrokenLight* m_pCur;  // 最後尾のオブジェクトへのポインタ
	CMapObject_BrokenLight* m_pNext;        // 次のオブジェクトへのポインタ
	CMapObject_BrokenLight* m_pPrev;        // 前のオブジェクトへのポインタ

	bool m_bLightFall = false;

	int m_nFallDownCount = 0;
};

//===========================================================
// ステイト
//===========================================================
class CBrokenLightState
{
public:
	CBrokenLightState() {}
	~CBrokenLightState() {}

	virtual void Update(CMapObject_BrokenLight* pTrashBox) = 0;

private:

};

// ニュートラル
class CBrokenLightNeutral : public CBrokenLightState
{
public:
	CBrokenLightNeutral();
	~CBrokenLightNeutral() {}

	void Update(CMapObject_BrokenLight* pLighth) override;

private:

};

// バウンド
class CBrokenLightFall : public CBrokenLightState
{
public:
	CBrokenLightFall();
	~CBrokenLightFall() {}

	void Update(CMapObject_BrokenLight* pLighth) override;

private:

};

#endif