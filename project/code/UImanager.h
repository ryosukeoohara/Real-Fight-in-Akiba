//===========================================================
//
// UIマネージャ[UImanager.h]
// Author 大原怜将
//
//===========================================================
#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "object2D.h"

//===========================================================
// UIマネージャクラス定義
//===========================================================
class CUIManager : CObject2D
{
public:
	//種類
	enum TYPE
	{
		TYPE_NONE = 0,                              // なんもない
		TYPE_TITLE,                                 // タイトル
		TYPE_TUTORIAL,                              // チュートリアル
		TYPE_RESULT,                                // リザルト
		TYPE_WEAKNAME,                              // ザコ
		TYPE_BOSSNAME,                              // ボス
		TYPE_LINE,                                  // 名前の下の線
		TYPE_MAX
	};

private:

	// 情報
	struct INFO
	{
		D3DXVECTOR3 pos;  // 位置
		D3DXVECTOR3 rot;  // 向き	
		D3DXVECTOR3 move; // 移動
		D3DCOLOR col;     // 色
		TYPE type;        // 種類
	};

	INFO m_Info;          // 情報

public:

	CUIManager();                                   // コンストラクタ
	CUIManager(D3DXVECTOR3 pos, TYPE type);         // コンストラクタ
	~CUIManager();                                  // デストラクタ
													   
	HRESULT Init(void);                             // 初期化処理
	void Uninit(void);                              // 終了処理
	void Update(void);                              // 更新処理
	void Draw(void);                                // 描画処理

	static CUIManager *Create(D3DXVECTOR3 pos, TYPE type); // 生成処理

private:

	// メンバ関数
	void LineMove(void);    // 線の移動
	void ShrinkSize(void);  // ちっちゃくなる
	
	// メンバ変数
	int m_nIdxTexture;                              // テクスチャのインデックス番号
};

#endif // !_UIMANAGER_H_