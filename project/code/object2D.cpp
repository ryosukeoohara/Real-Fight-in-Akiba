//===========================================================
//
// 2Dオブジェクト処理[object2D.cpp]
// Author 大原怜将
//
//===========================================================
#include "object.h"
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "game.h"

//===========================================================
// コンストラクタ
//===========================================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	//値をクリア
	m_nIdxTexture = -1;
	m_pVtxBuff = NULL;  //頂点情報を格納
	m_nCounterAnimPlayer = 0;
	m_nPatternAnimPlayer = 0;
	m_Number = 0;
	m_bDraw = false;
}

//===========================================================
// コンストラクタ(オーバーロード)
//===========================================================
CObject2D::CObject2D(D3DXVECTOR3 pos, int nPriority)
{
	// 値をクリア
	m_pos = pos;
	m_nIdxTexture = -1;
	m_pVtxBuff = NULL;
	m_nCounterAnimPlayer = 0;
	m_nPatternAnimPlayer = 0;
	m_Number = 0;
	m_bDraw = false;
}

//===========================================================
// デストラクタ
//===========================================================
CObject2D::~CObject2D()
{

}

//===========================================================
// 生成処理
//===========================================================
CObject2D * CObject2D::Create(int nPriority)
{
	// オブジェクト2Dのポインタ
	CObject2D *pObject2D = nullptr;

	if (pObject2D == nullptr)
	{
		// オブジェクト2Dの生成
		pObject2D = new CObject2D();

		// 初期化処理
		pObject2D->Init();
	}
	
	return pObject2D;
}

//===========================================================
// 生成処理
//===========================================================
CObject2D *CObject2D::Create(D3DXVECTOR3 pos, int nPriority)
{
	// オブジェクト2Dのポインタ
	CObject2D *pObject2D = nullptr;

	if (pObject2D == NULL)
	{
		// オブジェクト2Dの生成
		pObject2D = new CObject2D(pos, nPriority);

		// 初期化処理
		pObject2D->Init();
	}
	return pObject2D;
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CObject2D::Init(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetInstance()->GetRenderer();
	
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CObject2D::Uninit(void)
{
	//頂点バッファの破壊
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//===========================================================
// 更新処理
//===========================================================
void CObject2D::Update(void)
{
	
}

//===========================================================
// 描画処理
//===========================================================
void CObject2D::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetInstance()->GetRenderer();
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_bDraw == true)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    //プリミティブの種類
			0,                                         //描画する最初の頂点インデックス
			2                                          //描画するプリミティブ数
		);
	}
}

//===========================================================
// 色設定処理
//===========================================================
void CObject2D::SetColor(D3DXCOLOR col)
{
	m_col = col;

	VERTEX_2D *pVtx;

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

//===========================================================
// サイズ設定処理
//===========================================================
void CObject2D::SetSize(float fWidth, float fHeight)
{
	m_fHeight = fHeight;
	m_fWidth = fWidth;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x - m_fWidth;
	pVtx[0].pos.y = m_pos.y - m_fHeight;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + m_fWidth;
	pVtx[1].pos.y = m_pos.y - m_fHeight;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x - m_fWidth;
	pVtx[2].pos.y = m_pos.y + m_fHeight;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + m_fWidth;
	pVtx[3].pos.y = m_pos.y + m_fHeight;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================================
// サイズ設定処理:中心が左端
//===========================================================
void CObject2D::SetEdgeCenter(float fWidth, float fHeight)
{
	m_fHeight = fHeight;
	m_fWidth = fWidth;

	VERTEX_2D *pVtx;

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
//===========================================================
// サイズ設定処理:中心が左端
//===========================================================
void CObject2D::SetEdgeCenterTex(float ftex)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x;
	pVtx[0].pos.y = m_pos.y;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + ftex;
	pVtx[1].pos.y = m_pos.y;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x;
	pVtx[2].pos.y = m_pos.y + m_fHeight;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + ftex;
	pVtx[3].pos.y = m_pos.y + m_fHeight;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================================
// 描画処理
//===========================================================
void CObject2D::SetTex(float fTex)
{
	m_Number = (int)fTex;

	if (m_Number >= 10)
	{
		m_Number = 0;
	}

	if (m_Number < 0)
	{
		m_Number = 9;
	}

	VERTEX_2D * pVtx;     //頂点情報へのポインタ
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);        //頂点バッファをロックし、頂点情報へポインタを取得

	pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * m_Number, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * m_Number, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * m_Number, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * m_Number, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================================
// 透明度設定
//===========================================================
void CObject2D::SetColorA(float fCola)
{
	m_col.a += fCola;

	if (m_col.a >= 1.0f || m_col.a < 0.3f)
	{
		m_col.a *= -1;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
	pVtx[1].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
	pVtx[2].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
	pVtx[3].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}