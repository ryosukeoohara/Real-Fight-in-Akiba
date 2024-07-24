//===========================================================
//
// 敵登場時のUI[userrankUI.h]
// Author 大原怜将
//
//===========================================================
#ifndef _USERRANKUI_H_
#define _USERRANKUI_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "object.h"

//===========================================================
// 前方宣言
//===========================================================
class CObject2D;
class CNumber;

//*==========================================================
// クラス定義
//*==========================================================
class CUserRankUI : CObject
{
public:

	//種類
	enum TYPE
	{
		TYPE_NOEN = 0, // なんもない
		TYPE_TOP,      // 最高
		TYPE_MIDDLE,   // 中間
		TYPE_UNDER,    // 下っ端
		TYPE_WEAKEST,  // 最弱
		TYPE_MAX
	};

public:

	CUserRankUI();                  // コンストラクタ
	CUserRankUI(TYPE type, int nPriority = 7);         // コンストラクタ
	~CUserRankUI();                 // デストラクタ

	HRESULT Init(void);             // 初期化処理
	void Uninit(void);              // 終了処理
	void Update(void);              // 更新処理
	void Draw(void);                // 描画処理

	static CUserRankUI *Create(TYPE type); // 生成処理

private:
	TYPE m_type;
	CObject2D *m_pObject;

};

#endif