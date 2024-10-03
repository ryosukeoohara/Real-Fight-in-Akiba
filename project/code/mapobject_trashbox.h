//===========================================================
//
// マップに配置れているゴミ箱処理[mapobject_trashbox.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAPOBJECT_TRANSHBOX_H__             //このマクロ定義がされてなかったら
#define _MAPOBJECT_TRANSHBOX_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "objectX.h"

// 前方宣言
class CTrashBoxState;

//===========================================================
// ゴミ箱クラス定義
//===========================================================
class CMapObject_TrashBox : public CObjectX
{
public:
	CMapObject_TrashBox();
	CMapObject_TrashBox(const char* aModelFliename, int nPriority = 3);
	~CMapObject_TrashBox();

	HRESULT Init(void);           // 初期化処理    
	void Uninit(void);            // 終了処理
	void Update(void);            // 更新処理
	void Draw(void);              // 描画処理

	static CMapObject_TrashBox* Create(const char* aModelFliename, int nPriority = 3);  //生成

private:

};

//===========================================================
// ステイト
//===========================================================
class CTrashBoxState
{
public:
	CTrashBoxState() {}
	~CTrashBoxState() {}

	virtual void Update(CMapObject_TrashBox* pTrashBox) = 0;

private:

};

// 吹き飛ぶ
class CTrashBoxBlowAway : public CTrashBoxState
{
public:
	CTrashBoxBlowAway();
	~CTrashBoxBlowAway() {}

	void Update(CMapObject_TrashBox* pTrashBox) override;

private:

};

// バウンド
class CTrashBoxBound : public CTrashBoxState
{
public:
	CTrashBoxBound();
	~CTrashBoxBound() {}

	void Update(CMapObject_TrashBox* pTrashBox) override;

private:

};

#endif