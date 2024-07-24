//===========================================================
//
// ポーズ処理 [pause.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _PAUSE_H_             //このマクロ定義がされてなかったら
#define _PAUSE_H_             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "object2D.h"

//===========================================================
// ポーズクラス定義
//===========================================================
class CPause
{
private:

	// テクスチャ			            
	enum TYPE
	{
		TYPE_COUTINUE = 0,                     // ゲームに戻る
		TYPE_RETRY,                            // ゲームをやり直す
		TYPE_QUIT,                             // タイトル画面に戻る
		TYPE_BACK,                             // 後ろの黒い背景
		TYPE_FRAME,                            // 枠
		TYPE_MAX
	};

	// 項目	
	enum MENU
	{
		MENU_COUTINUE = 0,                     // ゲームに戻る
		MENU_RETRY,                            // ゲームをやり直す
		MENU_QUIT,                             // タイトル画面に戻る
		MENU_MAX
	};

	//構造体
	struct PAUSE
	{
		CObject2D *Object2D;                   // 2Dオブジェクトのポインタ
		D3DXCOLOR col;                         // 色
	};

public:
	CPause();                                  // コンストラクタ
	~CPause();                                 // デストラクタ
								               
	HRESULT Init(void);                        // ポーズ初期化処理    
	void Uninit(void);                         // ポーズ終了処理
	void Update(void);                         // ポーズ更新処理
	void Draw(void);                           // ポーズ描画処理

	static CPause *Create(void);               // 生成処理

	void SetDraw(bool bPause);                 // 描画するかどうか
	void SetCol(void);
							                   
private:
	CObject2D *m_pBg;                           // 黒い背景用のポインタ
	CObject2D *m_pFrame;                        // 枠用のポインタ
	PAUSE m_aMenu[MENU_MAX];                    // メニュー
	int m_nSelect;                              // 選択している番号
	static const char *m_apTexName[TYPE_MAX];   // テクスチャのお名前
};

#endif