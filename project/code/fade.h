//===========================================================
//
// フェード処理 [fade.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _FADE_H_             //このマクロ定義がされてなかったら
#define _FADE_H_             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "object2D.h"
#include "manager.h"

//===========================================================
// フェードクラス定義
//===========================================================
class CFade
{
public:

	// 種類
	enum FADE
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
		FADE_BLACK,
		FADE_MAX
	};

	CFade();                                   // コンストラクタ
	CFade(CScene::MODE modenext);              // コンストラクタ
	~CFade();                                  // デストラクタ
												  
	HRESULT Init(void);                        // 初期化処理
	void Uninit(void);                         // 終了処理
	void Update(void);                         // 更新処理
	void Draw(void);                           // 描画処理
												  
	// 設定系									   
	void Set(CScene::MODE modenext);           // 設定処理
	void Set(void);

	// 取得系
	FADE Get(void) { return m_fade; }          // フェード取得
	float GetCol(void) { return m_Color.a; }   // 色取得

	static CFade *Create(CScene::MODE modenext);                // 生成

private:
	LPDIRECT3DTEXTURE9 m_pTexture;              // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;         // 頂点情報を格納
	FADE m_fade;                                // フェードの状態
	CScene::MODE m_modeNext;                    // 次の画面
	CScene::MODE m_modeOld;                     // 次の画面
	D3DXCOLOR m_Color;                          // ポリゴンの色
	int m_nIdxTexture;                          // テクスチャのインデックス番号
	bool m_bFade;                               // フェード中かどうか
};

#endif