//===========================================================
//
// 3Dオブジェクト処理[object3D.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "main.h"
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "debugproc.h"

//===========================================================
// コンストラクタ
//===========================================================
CObject3D::CObject3D(D3DXVECTOR3 texpos, int nPriority) : CObject(nPriority)
{
	//値をクリア
	m_pTexture = NULL;  //テクスチャへのポインタ
	m_pVtxBuff = NULL;  //頂点情報を格納
	m_pCurrent = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);                       
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     
	m_Texpos = D3DXVECTOR3(texpos.x, texpos.y, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nIdxTexture = -1;                        
	m_fHeight = 0.0f;                           
	m_fWidth = 0.0f;                            
	m_bDraw = true;                              
}

//===========================================================
// コンストラクタ(オーバーロード)
//===========================================================
CObject3D::CObject3D(D3DXVECTOR3 pos)
{
	//値をクリア
	m_pTexture = NULL;  //テクスチャへのポインタ
	m_pVtxBuff = NULL;  //頂点情報を格納
	m_pCurrent = nullptr;
	m_pos = pos;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Texpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nIdxTexture = -1;
	m_fHeight = 0.0f;
	m_fWidth = 0.0f;
	m_bDraw = true;
}

//===========================================================
// デストラクタ
//===========================================================
CObject3D::~CObject3D()
{

}

//===========================================================
// 生成処理
//===========================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 texpos, int nPriority)
{
	// オブジェクト3Dのポインタ
	CObject3D *pObject3D = nullptr;

	if (pObject3D == nullptr)
	{
		// 生成
		pObject3D = new CObject3D(texpos, nPriority);

		// 初期化処理
		pObject3D->Init();
	}
	
	return pObject3D;
}

//===========================================================
// コンストラクタ(オーバーロード)
//===========================================================
//CObject3D * CObject3D::Create(D3DXVECTOR3 pos)
//{
//	// オブジェクト3Dのポインタ
//	CObject3D *pObject3D = nullptr;
//
//	if (pObject3D == nullptr)
//	{
//		// オブジェクト3Dの生成
//		pObject3D = new CObject3D(pos);
//
//		// 初期化処理
//		pObject3D->Init();
//	}
//	
//	return pObject3D;
//}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CObject3D::Init(void)
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
	pVtx[1].tex = D3DXVECTOR2(m_Texpos.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_Texpos.y);
	pVtx[3].tex = D3DXVECTOR2(m_Texpos.x, m_Texpos.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CObject3D::Uninit(void)
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
void CObject3D::Update(void)
{
	//SetSize(m_fHeight, m_fWidth);
}

//===========================================================
// 描画処理
//===========================================================
void CObject3D::Draw(void)
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

	if (m_bSubBlend)
	{
		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

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

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAddress(m_nIdxTexture));

	//ポリゴンの描画  D3DPT_LINESTRIP
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    //プリミティブの種類
		0,                                         //描画する最初の頂点インデックス
		2                                          //描画するプリミティブ数
	);					

	//zテストを有効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 160);

	if (m_bSubBlend)
	{
		//通常の合成に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//===========================================================
// テクスチャ取得
//===========================================================
void CObject3D::BindTexture(LPDIRECT3DTEXTURE9 m_Texture)
{
	m_pTexture = m_Texture;
}

//===========================================================
// サイズ設定処理
//===========================================================
void CObject3D::EaseToTarget(D3DXVECTOR2 current, float adjustSpeed)
{
	if (adjustSpeed > 1.0f)
		adjustSpeed = 1.0f;

	if (adjustSpeed < 0.0f)
		adjustSpeed = 0.01f;

	m_fHeight = m_fHeight + (current.x - m_fHeight) * adjustSpeed;
	m_fWidth = m_fWidth + (current.x - m_fWidth) * adjustSpeed;

	// サイズ設定
	SetSize(m_fHeight, m_fWidth);
}

//===========================================================
// α値を下げる処理
//===========================================================
void CObject3D::GradualFadeOut(float cola)
{
	VERTEX_3D* pVtx;     //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_col.a = m_col.a - cola;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_col.a);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_col.a);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_col.a);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_col.a);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================================
// サイズ設定処理
//===========================================================
void CObject3D::SetSize(float fHeight, float fWidth)
{
	m_fHeight = fHeight;
	m_fWidth = fWidth;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x =  -m_fHeight;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = m_fWidth;
	pVtx[1].pos.x = m_fHeight;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = m_fWidth;
	pVtx[2].pos.x =  -m_fHeight;
	pVtx[2].pos.y = 0.0f;
	pVtx[2].pos.z = -m_fWidth;
	pVtx[3].pos.x = m_fHeight;
	pVtx[3].pos.y = 0.0f;
	pVtx[3].pos.z =  -m_fWidth;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================================
// サイズ設定処理:中心が左端
//===========================================================
void CObject3D::SetEdgeCenter(float fWidth, float fHeight)
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