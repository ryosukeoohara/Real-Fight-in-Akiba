//===========================================================
//
// フェード処理 [fade.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "fade.h"
#include "texture.h"
#include "renderer.h"

//===========================================================
// 定数定義
//===========================================================alphaDelta
namespace
{
	const float ALPHA_DELTA = 0.015f;  // α値の変化量
}

//===========================================================
// コンストラクタ
//===========================================================
CFade::CFade()
{
	m_fade = FADE_IN;
	m_modeNext = m_modeNext;
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nIdxTexture = -1;
	m_bFade = false;
}

//===========================================================
// コンストラクタ
//===========================================================
CFade::CFade(CScene::MODE modenext)
{
	m_fade = FADE_OUT;
	m_modeNext = modenext;
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	m_nIdxTexture = -1;
	m_bFade = false;
}

//===========================================================
// デストラクタ
//===========================================================
CFade::~CFade()
{

}

//===========================================================
// 生成処理
//===========================================================
CFade *CFade::Create(CScene::MODE modenext)
{
	//地面のポインタ
	CFade *pFade = NULL;

	if (pFade == NULL)
	{//使用されていなかったら

		//オブジェクトを生成
		pFade = new CFade(modenext);

		pFade->Init();
	}
	
	return pFade;
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CFade::Init(void)
{
	//テクスチャの情報取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	m_nIdxTexture = -1;

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
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//================================================================
// 終了処理
//================================================================
void CFade::Uninit(void)
{
	//頂点バッファの破壊
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//テクスチャの処理
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//================================================================
// 更新処理
//================================================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_Color.a -= ALPHA_DELTA;

			if (m_Color.a <= 0.0f)
			{
				m_Color.a = 0.0f;
				m_fade = FADE_NONE;
				m_bFade = false;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_Color.a += ALPHA_DELTA;

			if (m_Color.a >= 1.0f)
			{
				m_Color.a = 1.0f;

				m_fade = FADE_IN;

				CManager::GetInstance()->SetMode(m_modeNext);
			}
		}
		else if (m_fade == FADE_BLACK)
		{
			m_Color.a += ALPHA_DELTA;

			if (m_Color.a >= 1.0f)
			{
				m_Color.a = 1.0f;

				m_fade = FADE_IN;
			}
		}
	}

	//頂点情報へのポインタ
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Color.a);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Color.a);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Color.a);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Color.a);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================================
// 描画処理
//================================================================
void CFade::Draw(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    //プリミティブの種類
		0,                                         //描画する最初の頂点インデックス
		2                                          //描画するプリミティブ数
	);
}

//================================================================
// 遷移処理
//================================================================
void CFade::Set(CScene::MODE modeNext)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//================================================================
// 画面を黒いポリゴンで覆う処理
//================================================================
void CFade::Set(void)
{
	if (m_bFade == false)
	{
		m_fade = FADE_BLACK;
		m_bFade = true;
		m_modeOld = m_modeNext;
		m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}