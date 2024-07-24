//===========================================================
//
// テクスチャ処理 [texture.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include<string.h>

//===========================================================
// 静的メンバ変数
//===========================================================
int CTexture::m_nNumAll = 0;

//===========================================================
// コンストラクタ
//===========================================================
CTexture::CTexture()
{
	
}

//===========================================================
// デストラクタ
//===========================================================
CTexture::~CTexture()
{
	
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CTexture::Load(void)
{
	for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
	{
		m_aTexture[nCount].m_pTexture = NULL;
	}

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CTexture::UnLoad(void)
{
	for (int nCount = 0; nCount < MAX_TEXTURE; nCount++)
	{
		if (m_aTexture[nCount].m_pTexture != NULL)
		{
			m_aTexture[nCount].m_pTexture = NULL;
		}
	}

	m_nNumAll = 0;
}

//===========================================================
// 指定のテクスチャ読み込み
//===========================================================
int CTexture::Regist(const char *pFilename)
{
	//デバイスを取得
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	int nIdx = m_nNumAll;

	for (int nCount = 0; nCount < m_nNumAll; nCount++)
	{
		if (strcmp(pFilename, m_aTexture[nCount].aName) == 0)
		{
			return nCount;
		}
	}

	if (m_aTexture[nIdx].m_pTexture == NULL)
	{//使用されていなかったら

		strcpy(m_aTexture[nIdx].aName, pFilename);

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, m_aTexture[nIdx].aName, &m_aTexture[nIdx].m_pTexture);

		m_nNumAll++;  //テクスチャの総数をカウントアップ
	}
	
	return nIdx;
}

//===========================================================
// 指定した番号のテクスチャのアドレス取得
//===========================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (nIdx >= 0)
	{
		return m_aTexture[nIdx].m_pTexture;
	}
	else
	{
		return NULL;
	}
}
