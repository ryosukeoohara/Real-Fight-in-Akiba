//===========================================================
//
//ポリゴンを出すやつ[main.h]
//Author 大原怜将
//
//===========================================================
#ifndef _MAIN_H_     //このマクロが定義されてなかったら
#define _MAIN_H_     //2重インクルードを防止のマクロを定義する

namespace
{
	const char *CLASS_NAME = "windowclass";  // ウインドウクラスの名前
	const int SCREEN_WIDTH = 1280;                   //ウインドウに幅
	const int SCREEN_HEIGHT = 720;                   //ウインドウの高さ
	const int FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	const int FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
}

// 頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;   // 位置
	float rhw;
	D3DCOLOR col;      // 色
	D3DXVECTOR2 tex;   // テクスチャの座標
} VERTEX_2D;

// 頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;    // 位置
	D3DXVECTOR3 nor;    // 法線ベクトル
	D3DCOLOR col;       // 色
	D3DXVECTOR2 tex;    // テクスチャ座標
} VERTEX_3D;




#endif