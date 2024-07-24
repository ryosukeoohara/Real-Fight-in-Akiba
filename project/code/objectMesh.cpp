//===========================================================
//
// メッシュオブジェクト処理[objectMesh.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "main.h"
#include "objectMesh.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const int HEIGHT = 2;  // 縦幅
	const int WIDHT = 2;   // 横幅
}

//===========================================================
// コンストラクタ
//===========================================================
CObjectMesh::CObjectMesh()
{
	//値をクリア
	m_pTexture = nullptr;  //テクスチャへのポインタ
	m_pVtxBuff = nullptr;  //頂点情報を格納
	m_pIdxBuff = nullptr;
	m_pCurrent = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nIdxTexture = -1;
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
	m_bDraw = true;
}

//===========================================================
// コンストラクタ
//===========================================================
CObjectMesh::CObjectMesh(D3DXVECTOR3 pos)
{
	//値をクリア
	m_pTexture = nullptr;  //テクスチャへのポインタ
	m_pVtxBuff = nullptr;  //頂点情報を格納
	m_pCurrent = nullptr;
	m_pIdxBuff = nullptr;
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nIdxTexture = -1;
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
	m_bDraw = true;
}

//===========================================================
// デストラクタ
//===========================================================
CObjectMesh::~CObjectMesh()
{

}

//===========================================================
// 生成処理
//===========================================================
CObjectMesh *CObjectMesh::Create(void)
{
	// オブジェクト3Dのポインタ
	CObjectMesh *pObject3D = NULL;

	if (pObject3D == NULL)
	{
		// オブジェクト3Dの生成
		pObject3D = new CObjectMesh();

		// 初期化処理
		pObject3D->Init();
	}
	
	return pObject3D;
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CObjectMesh::Init(void)
{
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 13,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;     //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nHeight = 0; nHeight < 2; nHeight++)
	{
		for (int nWidth = 0; nWidth < 2; nWidth++)
		{
			pVtx[0].pos = D3DXVECTOR3((100.0f * nWidth) - 100.0f, 0.0f, (100.0f * nHeight) - 100.0f);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}

	////頂点座標の設定
	//pVtx[0].pos = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);
	//pVtx[1].pos = D3DXVECTOR3(-100.0f, 0.0f, 100.0f);
	//pVtx[2].pos = D3DXVECTOR3(100.0f, 0.0f, -100.0f);
	//pVtx[3].pos = D3DXVECTOR3(100.0f, 0.0f, 100.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//頂点バッファを生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 9,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	//インデックス情報へのポインタ
	WORD * pIdx;

	//インデックスバッファをロックし、頂点情報へポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	
	//頂点番号データの設定
	pIdx[0] = 1;
	pIdx[1] = 3;
	pIdx[2] = 0;
	pIdx[3] = 2;

	// 頂点バッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CObjectMesh::Uninit(void)
{
	//テクスチャの処理
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//頂点バッファの破壊
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//===========================================================
// 更新処理
//===========================================================
void CObjectMesh::Update(void)
{

}

//===========================================================
// 描画処理
//===========================================================
void CObjectMesh::Draw(void)
{
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);

	D3DXMATRIX m_mtxRot, m_mtxTrans;           // 計算用マトリックス

											   //ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_pCurrent == nullptr)
	{
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);
	}
	else
	{
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);

		// マトリックスと親のマトリックスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld, m_pCurrent);
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	//pDevice->SetTexture(0, m_pTexture);

	////ポリゴンの描画  D3DPT_LINESTRIP
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    //プリミティブの種類
	//	0,                                         //描画する最初の頂点インデックス
	//	8                                          //描画するプリミティブ数
	//);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 13, 0, 9);

	//zテストを有効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);
}

//===========================================================
// テクスチャ取得
//===========================================================
void CObjectMesh::BindTexture(LPDIRECT3DTEXTURE9 m_Texture)
{
	m_pTexture = m_Texture;
}

//===========================================================
// サイズ設定処理
//===========================================================
void CObjectMesh::SetSize(float fHeight, float fWidth)
{
	m_fHeight = fHeight;
	m_fWidth = fWidth;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	/*pVtx[0].pos.x = m_pos.x - m_fHeight;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = m_pos.z + m_fWidth;
	pVtx[1].pos.x = m_pos.x + m_fHeight;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = m_pos.z + m_fWidth;
	pVtx[2].pos.x = m_pos.x - m_fHeight;
	pVtx[2].pos.y = 0.0f;
	pVtx[2].pos.z = m_pos.z - m_fWidth;
	pVtx[3].pos.x = m_pos.x + m_fHeight;
	pVtx[3].pos.y = 0.0f;
	pVtx[3].pos.z = m_pos.z - m_fWidth;*/

	/*for (int nHeight = 0; nHeight < HEI + 1; nHeight++)
	{
		for (int nWidth = 0; nWidth < WID + 1; nWidth++)
		{
			pVtx[0].pos.x = m_pos.x + (m_fWidth * nWidth) - ((m_fWidth * WID) * 0.5f);
			pVtx[0].pos.y = 0.0f;
			pVtx[0].pos.z = m_pos.z + -((m_fHeight * nHeight) - ((m_fHeight * HEI) * 0.5f));

			pVtx += 1;
		}
	}*/

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	int n = 0;

	//インデックス情報へのポインタ
	WORD * pIdx;

	//インデックスバッファをロックし、頂点情報へポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//for (int nHeight = 0; nHeight < HEI + 1; nHeight++)
	//{
	//	for (int nWidth = 0; nWidth < WID + 1; nWidth++)
	//	{
	//		/*pIdx[0] = (WID + ((nWidth + nHeight) * 2) + 2 / HEI + nWidth) +
	//			     -(HEI + (nHeight * 2) + 1 / WID + nHeight + nWidth);*/
	//		int i = (nHeight + 1) + nWidth  * WID - (nHeight * HEI);

	//		pIdx += 1;

	//		n++;
	//	}
	//}

	// 頂点バッファをアンロックする
	m_pIdxBuff->Unlock();
}

//===========================================================
// サイズ設定処理:中心が左端
//===========================================================
void CObjectMesh::SetEdgeCenter(float fWidth, float fHeight)
{
	m_fHeight = fHeight;
	m_fWidth = fWidth;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x;
	pVtx[0].pos.y = m_pos.y;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + m_fWidth;
	pVtx[1].pos.y = m_pos.y;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x;
	pVtx[2].pos.y = m_pos.y + m_fHeight;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + m_fWidth;
	pVtx[3].pos.y = m_pos.y + m_fHeight;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}