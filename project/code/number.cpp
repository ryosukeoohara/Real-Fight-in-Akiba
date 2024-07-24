//===========================================================
//
// 数字関連の処理[number.cpp]
// Author 大原怜将
//
//===========================================================
#include "number.h"
#include "object.h"
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "time.h"
#include "score.h"
#include "input.h"
#include "camera.h"
#include "game.h"

//===========================================================
// コンストラクタ
//===========================================================
CNumber::CNumber()
{
	//値をクリア
	m_pTexture = NULL;  //テクスチャへのポインタ
	m_pVtxBuff = NULL;  //頂点情報を格納
	m_nTimerCount = 0;
	m_Number = 0;
	m_nIdxTexture = -1;
}

//===========================================================
// コンストラクタ(オーバーロード)
//===========================================================
CNumber::CNumber(D3DXVECTOR3 pos)
{
	//値をクリア
	m_pTexture = NULL;  //テクスチャへのポインタ
	m_pVtxBuff = NULL;  //頂点情報を格納
	m_nTimerCount = 0;
	m_Number = 0;
	m_nIdxTexture = -1;
	m_pos = pos;
}

//===========================================================
// デストラクタ
//===========================================================
CNumber::~CNumber()
{

}

//===========================================================
// 生成処理
//===========================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos)
{
	//オブジェクト2Dのポインタ
	CNumber *pNumber = nullptr;

	if (pNumber == nullptr)
	{
		//オブジェクト2Dの生成
		pNumber = new CNumber(pos);

		//初期化処理
		pNumber->Init();
	}
	
	return pNumber;
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CNumber::Init(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetInstance()->GetRenderer();

	//テクスチャの情報取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 pos = GetPosition();

	//頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//テクスチャ読み込み
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\number002.png");

	//頂点情報へのポインタ
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - 30.0f, pos.y - 30.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + 30.0f, pos.y - 30.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - 30.0f, pos.y + 30.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + 30.0f, pos.y + 30.0f, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.9f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.9f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

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

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CNumber::Uninit(void)
{
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
void CNumber::Update(void)
{
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
// 描画処理
//===========================================================
void CNumber::Draw(void)
{
	//レンダラーの情報を取得
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//テクスチャの情報取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//デバイスの情報を取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    //プリミティブの種類
		0,                   //描画する最初の頂点インデックス
		2                    //描画するプリミティブ数
	);
	
}

//===========================================================
// 数字の設定
//===========================================================
void CNumber::SetNumber(int nNumber)
{
	m_Number = nNumber;

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
// 位置設定
//===========================================================
void CNumber::SetVtxCounter(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_2D * pVtx;     //頂点情報へのポインタ
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);        //頂点バッファをロックし、頂点情報へポインタを取得

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//===========================================================
// 位置設定
//===========================================================
void CNumber::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;  //位置を代入
}

//===========================================================
// 位置取得
//===========================================================
D3DXVECTOR3 CNumber::GetPosition(void)
{
	return m_pos;
}

//===========================================================
// テクスチャ取得
//===========================================================
void CNumber::BindTexture(LPDIRECT3DTEXTURE9 m_Texture)
{
	m_pTexture = m_Texture;
}