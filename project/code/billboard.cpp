//=============================================================================
//
// ビルボード処理 [billboard.cpp]
// Author : 大原　怜将
//
//=============================================================================
#include "main.h"
#include "billboard.h"
#include "renderer.h"
#include "manager.h"
#include "object2D.h"
#include "texture.h"
#include "debugproc.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const D3DXVECTOR2 XYPATTERN[CBillBoard::TYPE_MAX] = 
	{
		D3DXVECTOR2(1.0f, 1.0f), // なんもない
		D3DXVECTOR2(4.0f, 3.0f), // 攻撃ヒット
	}; // 分割数

	const int NUMPATTERN[CBillBoard::TYPE_MAX] =
	{
		0,  // なんもない
		10, // 攻撃ヒット
	}; // パターンの総数
}

//================================================================
//コンストラクタ
//================================================================
CBillBoard::CBillBoard(int nPriority) : CObject(nPriority)
{
	//値をクリア
	m_pTexture = NULL;  //テクスチャへのポインタ
	m_pVtxBuff = NULL;  //頂点情報を格納
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_nIdxTexture = -1;
	m_nNumPattern = 0;
	m_nNowPattern = 0;
	m_nCounterAnim = 0;
	m_nHeight = 0;
	m_nWidth = 0;
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
	m_bDraw = true;
	m_Type = TYPE_NONE;
}

//================================================================
//デストラクタ
//================================================================
CBillBoard::~CBillBoard()
{

}

//================================================================
//生成処理
//================================================================
CBillBoard * CBillBoard::Create(int nPriority)
{
	//オブジェクト2Dのポインタ
	CBillBoard *pBillBoard = nullptr;

	if (pBillBoard == nullptr)
	{
		//オブジェクト2Dの生成
		pBillBoard = new CBillBoard(nPriority);

		//初期化処理
		pBillBoard->Init();
	}

	return pBillBoard;
}

//================================================================
//生成処理
//================================================================
void CBillBoard::SetSize(float fHeight, float fWidth)
{
	m_fHeight = fHeight;
	m_fWidth = fWidth;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fWidth, fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fWidth, fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-fWidth, -fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fWidth, -fHeight, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================================
// サイズ設定処理:中心が左端
//================================================================
void CBillBoard::SetEdgeCenter(float fWidth, float fHeight)
{
	m_fHeight = fHeight;
	m_fWidth = fWidth;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = 0.0f;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_fWidth;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = 0.0f;
	pVtx[2].pos.y = -m_fHeight;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_fWidth;
	pVtx[3].pos.y = -m_fHeight;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================================
// 生成処理
//================================================================
void CBillBoard::SetTex(float fTex)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(50.0f,             + fTex, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(50.0f + m_fHeight, + fTex, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(50.0f,             0.0f,  0.0f);
	pVtx[3].pos = D3DXVECTOR3(50.0f + m_fHeight, 0.0f,  0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================================
// アニメーションするよ
//================================================================
void CBillBoard::SetAnim(void)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_nNowPattern++;

	if (m_nNowPattern % 1 == 0)
	{
		m_nCounterAnim = (m_nCounterAnim + 1) % m_nNumPattern;

		float fx = m_nCounterAnim % m_nWidth * (1.0f / XYPATTERN[m_Type].x);
		float fy = m_nCounterAnim / m_nHeight * (1.0f / XYPATTERN[m_Type].y);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + fx,                         0.0f + fy);
		pVtx[1].tex = D3DXVECTOR2((1.0f / XYPATTERN[m_Type].x) + fx, 0.0f + fy);
		pVtx[2].tex = D3DXVECTOR2(0.0f + fx,                         (1.0f / XYPATTERN[m_Type].y) + fy);
		pVtx[3].tex = D3DXVECTOR2((1.0f / XYPATTERN[m_Type].x) + fx, (1.0f / XYPATTERN[m_Type].y) + fy);

		m_nNowPattern = 0;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	if (m_nCounterAnim + 1 >= m_nNumPattern)
	{
		Uninit();
	}
}

//================================================================
// 色設定処理
//================================================================
void CBillBoard::SetColor(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================================
//壁の初期化処理
//===============================================================
HRESULT CBillBoard::Init(void)
{
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D * pVtx;     //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / XYPATTERN[m_Type].x), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / XYPATTERN[m_Type].y));
	pVtx[3].tex = D3DXVECTOR2((1.0f / XYPATTERN[m_Type].x), (1.0f / XYPATTERN[m_Type].y));

	m_nHeight = (int)XYPATTERN[m_Type].x;
	m_nWidth = (int)XYPATTERN[m_Type].y;

	m_nNumPattern = NUMPATTERN[m_Type];

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//================================================================
//壁の終了処理
//================================================================
void CBillBoard::Uninit(void)
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

//================================================================
//壁の壁の更新処理
//================================================================
void CBillBoard::Update(void)
{
	
}

//================================================================
//壁の描画処理
//================================================================
void CBillBoard::Draw(void)
{
	if (m_bDraw == true)
	{
		//デバイスの取得
		CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		//zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//αテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 160);

		D3DXMATRIX m_mtxTrans;           // 計算用マトリックス

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &m_mtxView);

		//ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&m_mtxWorld, NULL, &m_mtxView);
		m_mtxWorld._41 = 0.0f;
		m_mtxWorld._42 = 0.0f;
		m_mtxWorld._43 = 0.0f;

		if (m_pCurrent == nullptr)
		{
			//位置を反映
			D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);

			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);
		}
		else
		{
			//位置を反映
			D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);

			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);

			// マトリックスと親のマトリックスをかけ合わせる
			D3DXMatrixMultiply(&m_mtxWorld,
				&m_mtxWorld, m_pCurrent);
		}

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAddress(m_nIdxTexture));

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    //プリミティブの種類
			0,                      //描画する最初の頂点インデックス
			2                       //描画するプリミティブ数
		);

		//zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		//αテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 160);
	}
}