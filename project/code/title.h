//===========================================================
//
// タイトル処理[title.h]
// Author 大原怜将
//
//===========================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "manager.h"

//===========================================================
// 前方宣言
//===========================================================
class CField;
class CObject2D;
class CMap;
class CPlayer;

//===========================================================
// タイトルクラス定義
//===========================================================
class CTitle : public CScene
{
public:
	CTitle();             // コンストラクタ
	~CTitle();            // デストラクタ

	HRESULT Init(void);   // 初期化処理
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理
	void Draw(void);      // 描画処理

	static CTitle *Create(void);  // 生成処理
	static CPlayer *GetPlayer(void) { return m_pPlayer; }

private:
	CField *m_pField;
	CObject2D *m_pBg;
	CObject2D *m_pStart;
	CMap *m_pMap;
	static CPlayer *m_pPlayer;
	int m_nCnt;
	int m_nCntPos;

};
#endif // !_TITLE_H_