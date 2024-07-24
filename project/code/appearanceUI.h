//===========================================================
//
// 敵登場時のUI[appearanceUI.h]
// Author 大原怜将
//
//===========================================================
#ifndef _APPEARANCEUI_H_
#define _APPEARANCEUI_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "object.h"

//===========================================================
// 前方宣言
//===========================================================
class CObject2D;
class CNumber;

//===========================================================
// 敵登場時のUIクラス定義
//===========================================================
class CAppearanceUI : CObject
{
public:

	//種類
	enum TYPE
	{
		TYPE_NONE = 0,   // なんもない
		TYPE_WEAKNAME,   // ザコ
		TYPE_BOSSNAME,   // ボス
		TYPE_NUMBER,     // リザルト
		TYPE_MAX
	};

public:

	CAppearanceUI();                                   // コンストラクタ
	CAppearanceUI(TYPE type, int nPriority = 3);         // コンストラクタ
	~CAppearanceUI();                                  // デストラクタ

	HRESULT Init(void);                             // 初期化処理
	void Uninit(void);                              // 終了処理
	void Update(void);                              // 更新処理
	void Draw(void);                                // 描画処理

	static CAppearanceUI *Create(TYPE type, int nPriority = 3); // 生成処理

private:
	TYPE m_type;
	CObject2D *m_pFrontObj2D;  // 前に出る色が濃い奴
	CObject2D *m_pBackObj2D;   // 後ろに出る色が薄いやつ
	CObject2D *m_pNumber;
	bool m_bSound;
	
};

#endif