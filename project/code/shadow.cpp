//=============================================================================
//
// シャドウ処理 [shadow.cpp]
// Author : 大原　怜将
//
//=============================================================================
//
//#include "main.h"
//#include "shadow.h"
//#include "input.h"
//#include "player.h"
//
////マクロ定義
//#define MAX_SHADOW      (128)
//
////グローバル変数
//LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;       //テクスチャへのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;        //頂点バッファへのポインタ
//D3DXMATRIX g_mtxWorldShadow;    //ワールドマトリックス
//SHADOW g_Shadow;
//
//
////================================================================
////シャドウの初期化処理
////================================================================
//void InitShadow(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	PLAYER *pPlayer = GetPlayer();
//
//	////テクスチャの読み込み1
//	D3DXCreateTextureFromFile(pDevice,
//		"data\\TEXTURE\\shadow000.jpg",
//		&g_pTextureShadow
//	);
//
//	//頂点バッファを生成
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_3D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffShadow,
//		NULL);
//
//	//位置、向きを初期化
//	
//		g_Shadow.pos = D3DXVECTOR3(0.0f, 0.01f, 0.0f);
//		g_Shadow.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	
//	
//
//	VERTEX_3D * pVtx;     //頂点情報へのポインタ
//
//	//頂点バッファをロックし、頂点情報へポインタを取得
//	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
//
//	
//		//頂点座標の設定
//		pVtx[0].pos = D3DXVECTOR3(-5.0f, 0.0f, 5.0f);
//		pVtx[1].pos = D3DXVECTOR3(5.0f, 0.0f, 5.0f);
//		pVtx[2].pos = D3DXVECTOR3(-5.0f, 0.0f, -5.0f);
//		pVtx[3].pos = D3DXVECTOR3(5.0f, 0.0f, -5.0f);
//
//		//法線ベクトルの設定
//		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//		//頂点カラー
//		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//
//		//テクスチャ座標の設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//		//頂点データのポインタを4つ分進める
//		pVtx += 4;
//	
//
//	//頂点バッファをアンロックする
//	g_pVtxBuffShadow->Unlock();
//}
//
////================================================================
////シャドウの終了処理
////================================================================
//void UninitShadow(void)
//{
//	//テクスチャの処理
//	if (g_pTextureShadow != NULL)
//	{
//		g_pTextureShadow->Release();
//		g_pTextureShadow = NULL;
//	}
//
//	//頂点バッファの破壊
//	if (g_pVtxBuffShadow != NULL)
//	{
//		g_pVtxBuffShadow->Release();
//		g_pVtxBuffShadow = NULL;
//	}
//}
//
////================================================================
////シャドウの更新処理
////================================================================
//void UpdateShadow(void)
//{
//
//}
//
////================================================================
////シャドウの描画処理
////================================================================
//void DrawShadow(void)
//{
//	//デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	//計算用マトリックス
//	D3DXMATRIX mtxRot, mtxTrans;
//
//	//減算合成の設定
//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//
//	//ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&g_mtxWorldShadow);
//
//	//向きを反映
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow.rot.y, g_Shadow.rot.x, g_Shadow.rot.z);
//
//	D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);
//
//	//位置を反映
//	D3DXMatrixTranslation(&mtxTrans, g_Shadow.pos.x, g_Shadow.pos.y, g_Shadow.pos.z);
//
//	D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);
//
//	//ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);
//
//	//頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));
//
//	//頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	//テクスチャの設定
//	pDevice->SetTexture(0, g_pTextureShadow);
//
//	//ポリゴンの描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    //プリミティブの種類
//		0,                      //描画する最初の頂点インデックス
//		2                       //描画するプリミティブ数
//	);
//
//	//通常の合成に戻す
//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//}
//
//int SetShadow(void)
//{
//	PLAYER *pPlayer = GetPlayer();
//	SHADOW *pShadow = GetShadow();
//	Bullet *pBullet = GetBullet();
//
//	int nCntShadow = -1;
//
//	for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
//	{
//		pShadow->bUse = true;
//
// 		if (nCntShadow == -1)
//		{
//			pShadow->pos = pPlayer->pos;
//		}
//
//		else if (nCntShadow == 0)
//		{
//			pShadow->pos = pBullet->pos;
//		}
//
//		nCntShadow = nCount;
//
//	}
//
//	return nCntShadow;
//}
//
//void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
//{
//	PLAYER *pPlayer = GetPlayer();
//	SHADOW *pShadow = GetShadow();
//	Bullet *pBullet = GetBullet();
//
//	if (pShadow->type == SHADOWTYPE_PLAYER)
//	{
//		pShadow->pos = pPlayer->pos;
//	}
//
//	else if (pShadow->type == SHADOWTYPE_BULLET)
//	{
//		pShadow->pos = pBullet->pos;
//	}
//	
//
//	//pShadow->pos = pBullet->pos;
//}
//
//
//SHADOW *GetShadow(void)
//{
//	return &g_Shadow;
//}
//
//CShadow::CShadow()
//{
//}
//
//CShadow::~CShadow()
//{
//}
//
//HRESULT CShadow::Init(void)
//{
//	return E_NOTIMPL;
//}
//
//void CShadow::Uninit(void)
//{
//}
