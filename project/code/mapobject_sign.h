//===========================================================
//
// マップに配置れている標識処理[mapobject_sign.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAPOBJECT_SIGN_H__             //このマクロ定義がされてなかったら
#define _MAPOBJECT_SIGN_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// 前方宣言
class CSignState;

//===========================================================
// 標識クラス定義
//===========================================================
class CMapObject_Sign : public CMapObject
{
public:
	CMapObject_Sign();
	CMapObject_Sign(const char* aModelFliename, int nPriority = 3);
	~CMapObject_Sign();

	HRESULT Init(void);           // 初期化処理    
	void Uninit(void);            // 終了処理
	void Update(void);            // 更新処理
	void Draw(void);              // 描画処理

	static CMapObject_Sign* Create(const char* aModelFliename, int nPriority = 3);  //生成
	void ChangeState(CSignState* pState);  // ステートを変更

	// 取得
	CMapObject_Sign* GetTop(void) { return m_pTop; }  // リストの先頭

private:

	CSignState* m_pState;  // ステートのポインタ

	static CMapObject_Sign* m_pTop;  // 先頭のオブジェクトへのポインタ
	static CMapObject_Sign* m_pCur;  // 最後尾のオブジェクトへのポインタ
	CMapObject_Sign* m_pNext;        // 次のオブジェクトへのポインタ
	CMapObject_Sign* m_pPrev;        // 前のオブジェクトへのポインタ

	bool m_bShake = false;

};

//===========================================================
// ステイト
//===========================================================
class CSignState
{
public:
	CSignState() {}
	~CSignState() {}

	virtual void Update(CMapObject_Sign* pTrashBox) = 0;

private:

};

// ニュートラル
class CSignNeutral : public CSignState
{
public:
	CSignNeutral();
	~CSignNeutral() {}

	void Update(CMapObject_Sign* pTrashBox) override;

private:

};

// 殴られた
class CSignBeaten : public CSignState
{
public:
	CSignBeaten();
	~CSignBeaten() {}

	void Update(CMapObject_Sign* pElectricBox) override;

private:

	int m_nShakeTimeCounter;
	int m_nFallDownCount;
	float m_fShakeAngle;
};

// 吹き飛ぶ
class CSignBlowAway : public CSignState
{
public:
	CSignBlowAway();
	~CSignBlowAway() {}

	void Update(CMapObject_Sign* pTrashBox) override;

private:

};

// バウンド
class CSignBound : public CSignState
{
public:
	CSignBound();
	~CSignBound() {}

	void Update(CMapObject_Sign* pTrashBox) override;

private:

};

#endif
