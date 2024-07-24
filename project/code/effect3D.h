//===========================================================
//
// モデルのエフェクト[effect3D.h]
// Author 大原怜将
//
//===========================================================
#ifndef _EFFECT3D_H__             //このマクロ定義がされてなかったら
#define _EFFECT3D_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "objectX.h"

//===========================================================
// エフェクト3Dクラス定義
//===========================================================
class CEffect3D : public CObjectX
{
public:

	enum TYPE
	{
		TYPE_NONE = 0,   // なんもない
		TYPE_BIKE,       // 自転車
		TYPE_GLASS,      // ガラスの破片
		TYPE_MAX
	};

	CEffect3D();  // コンストラクタ
	CEffect3D(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, TYPE type, const char *modelfilename);  // コンストラクタ
	~CEffect3D();  // デストラクタ

				 // エフェクトの構造体
	struct Info
	{
		D3DXVECTOR3 pos;        // 位置
		D3DXVECTOR3 rot;        // 向き
		D3DXVECTOR3 move;       // 移動量
		D3DXCOLOR col;          // 色
		float fRadius;          // 半径(大きさ)
		int nLife;              // 寿命(表示時間)
	} ;

	HRESULT Init(void);           // 初期化処理    
	void Uninit(void);            // 終了処理
	void Update(void);            // 更新処理
	void Draw(void);              // 描画処理
	 
	void Bike(void);              // 自転車
	void Glass(void);             // ガラスの破片

	static CEffect3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, TYPE type, const char *modelfilename);  // 生成
	D3DXCOLOR m_col;          // 色
	float m_fRadius;          // 半径(大きさ)
	int m_nLife;              // 寿命(表示時間)
	D3DXVECTOR3 m_move;
	Info m_Info;

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;  // テクスチャへのポインタ
	TYPE m_type;                           // 種類
	int m_nIdxTexture;                     // テクスチャのインデックス番号

};

#endif