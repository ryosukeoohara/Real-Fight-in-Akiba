//===========================================================
//
// スコア処理[score.cpp]
// Author 大原怜将
//
//===========================================================
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "score.h"
#include "input.h"
#include "input.h"
#include "texture.h"
#include "game.h"
#include "time.h"

namespace
{
	const int NUM_SCORE = 8;  // スコアの桁数
}

//===========================================================
// 静的メンバ変数宣言
//===========================================================
CNumber *CScore::m_apNumber[NUM_SCORE] = {};
int CScore::m_nScore = 0;

//===========================================================
// コンストラクタ
//===========================================================
CScore::CScore()
{
	m_nScore = 0;  //スコア
}

//===========================================================
// コンストラクタ
//===========================================================
CScore::CScore(D3DXVECTOR3 pos)
{
	m_pos = pos;  //位置
	SetPosition(pos);
	m_nScore = 0;  //スコア
}

//===========================================================
// デストラクタ
//===========================================================
CScore::~CScore()
{

}

//===========================================================
// 生成処理
//===========================================================
CScore *CScore::Create(void)
{
	// スコアのポインタ
	CScore *pScore = nullptr;

	if (pScore == nullptr)
	{
		// 生成
		pScore = new CScore();

		//初期化処理
		pScore->Init();
	}
	
	return pScore;
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CScore::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	for (int nCount = 0; nCount < NUM_SCORE; nCount++)
	{
		if (m_apNumber[nCount] == NULL)
		{
			m_apNumber[nCount] = CNumber::Create({ 800.0f + 50.0f * nCount, 50.0f, 0.0f });

			m_apNumber[nCount]->SetNumberType(TYPENUMBER_SCORE);

			//初期化処理
			m_apNumber[nCount]->Init();
		}
	}

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CScore::Uninit(void)
{
	for (int nCount = 0; nCount < NUM_SCORE; nCount++)
	{
		// 終了処理
		if (m_apNumber[nCount] != nullptr)
		{
			m_apNumber[nCount]->Uninit();
			m_apNumber[nCount] = nullptr;
		}
	}

	Release();
}

//===========================================================
// 更新処理
//===========================================================
void CScore::Update(void)
{
	for (int nCount = 0; nCount < NUM_SCORE; nCount++)
	{
		// 更新処理
		if (m_apNumber[nCount] != nullptr)
			m_apNumber[nCount]->Update();
		
	}
}

//===========================================================
// 描画処理
//===========================================================
void CScore::Draw(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	for (int nCount = 0; nCount < NUM_SCORE; nCount++)
	{
		// 描画処理
		if (m_apNumber[nCount] != nullptr)
			m_apNumber[nCount]->Draw();
	}
}

//===========================================================
// スコア加算処理
//===========================================================
void CScore::AddScore(int nScore)
{
	m_nScore += nScore;

	m_apNumber[0]->m_Number = m_nScore % 100000000 / 10000000;
	m_apNumber[1]->m_Number = m_nScore % 10000000 / 1000000;
	m_apNumber[2]->m_Number = m_nScore % 1000000 / 100000;
	m_apNumber[3]->m_Number = m_nScore % 100000 / 10000;
	m_apNumber[4]->m_Number = m_nScore % 10000 / 1000;
	m_apNumber[5]->m_Number = m_nScore % 1000 / 100;
	m_apNumber[6]->m_Number = m_nScore % 100 / 10;
	m_apNumber[7]->m_Number = m_nScore % 10 / 1;
	
	for (int nCount = 0; nCount < NUM_SCORE; nCount++)
	{
		if(m_apNumber[nCount] != nullptr)
		m_apNumber[nCount]->SetNumber(m_apNumber[nCount]->m_Number);
	}
}