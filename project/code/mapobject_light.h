//===========================================================
//
// マップに配置れている外灯処理[mapobject_light.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAPOBJECT_LIGHT_H__             //このマクロ定義がされてなかったら
#define _MAPOBJECT_LIGHT_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// 前方宣言
class CLightState;

//===========================================================
// 外灯クラス定義
//===========================================================
class CMapObject_Light : public CMapObject
{
public:
	CMapObject_Light();
	CMapObject_Light(const char* aModelFliename, int nPriority = 3);
	~CMapObject_Light();

	HRESULT Init(void);   // 初期化処理    
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理
	void Draw(void);      // 描画処理

	static CMapObject_Light* Create(const char* aModelFliename, int nPriority = 3);  //生成
	void ChangeState(CLightState* pState);  // ステートを変更

	// 取得
	CMapObject_Light* GetTop(void) { return m_pTop; }  // リストの先頭
	int GetFallDownCount(void) { return m_nFallDownCount; }

	// 設定
	void SetFallDown(int nNum) { m_nFallDownCount = nNum; }
	void SetLightFall(bool nValue) { m_bLightFall = nValue; }

private:

	CLightState* m_pState;  // ステートのポインタ

	static CMapObject_Light* m_pTop;  // 先頭のオブジェクトへのポインタ
	static CMapObject_Light* m_pCur;  // 最後尾のオブジェクトへのポインタ
	CMapObject_Light* m_pNext;        // 次のオブジェクトへのポインタ
	CMapObject_Light* m_pPrev;        // 前のオブジェクトへのポインタ

	bool m_bLightFall = false;

	int m_nFallDownCount = 0;
};

//===========================================================
// ステイト
//===========================================================
class CLightState
{
public:
	CLightState() {}
	~CLightState() {}

	virtual void Update(CMapObject_Light* pTrashBox) = 0;

private:

};

// ニュートラル
class CLightNeutral : public CLightState
{
public:
	CLightNeutral();
	~CLightNeutral() {}

	void Update(CMapObject_Light* pLighth) override;

private:

};

// 吹き飛ぶ
class CLightFall : public CLightState
{
public:
	CLightFall();
	~CLightFall() {}

	void Update(CMapObject_Light* pLighth) override;

private:

	CObjectX* m_pLightR = nullptr;
	CObjectX* m_pLightL = nullptr;

};

// バウンド
class CLightBound : public CLightState
{
public:
	CLightBound();
	~CLightBound() {}

	void Update(CMapObject_Light* pLighth) override;

private:

};

// 殴られた
class CLightBeaten : public CLightState
{
public:
	CLightBeaten();
	~CLightBeaten() {}

	void Update(CMapObject_Light* pLighth) override;

private:

	int m_nShakeTimeCounter;
	int m_nFallDownCount;
	float m_fShakeAngle;
};


#endif