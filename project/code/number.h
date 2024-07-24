//===========================================================
//
// 数字関連の処理[number.h]
// Author 大原怜将
//
//===========================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "main.h"
#include "object2D.h"

//===========================================================
// 数字関連クラス定義
//===========================================================
class CNumber : public CObject2D 
{
public:		//誰でもアクセスできる
	CNumber();                         // コンストラクタ
	CNumber(D3DXVECTOR3 pos);          // コンストラクタ
	~CNumber();                        // デストラクタ
									   
	enum TYPE						   
	{								   
		TYPENUMBER_NONE = 0,		   
		TYPENUMBER_TIME,               // 時間
		TYPENUMBER_SCORE,              // スコア
		TYPENUMBER_DESTROYCOUNTER,     // 敵を倒した数
		TYPENUMBER_MAX				   
	} ;								   
									   
	HRESULT Init(void);                // 初期化処理
	void Uninit(void);                 // 終了処理
	void Update(void);                 // 更新処理
	void Draw(void);                   // 描画処理

	// 設定
	void SetPosition(D3DXVECTOR3 pos);                                 // 位置
	void SetVtxCounter(D3DXVECTOR3 pos, float fWidth, float fHeight);  // 
	void SetNumberType(TYPE type) { m_ntype = type; }                  // 種類
	void SetNumber(int nNumber);                                       // 数字

	// 取得
	D3DXVECTOR3 GetPosition(void);                                     // 位置
	TYPE GetNumberType(void) { return m_ntype; }                       // 種類
	int GetNumber(void) { return m_Number; }                           // 数字

	void BindTexture(LPDIRECT3DTEXTURE9 m_Texture);                    // テクスチャをバインド
	static CNumber *Create(D3DXVECTOR3 pos);                           // 生成処理
	int m_Number;

private:	//自分だけがアクセスできる
	LPDIRECT3DTEXTURE9 m_pTexture;        // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   // 頂点情報を格納
	D3DXVECTOR3 m_pos;                    // 位置
	TYPE m_ntype;                         // 種類
	static CObject2D *m_apObject2D;       // オブジェクト2Dのポインタ
	int m_nTimerCount;                    // 時間のカウンター
	int m_nIdxTexture;                    // テクスチャのインデックス番号

};

#endif // !1