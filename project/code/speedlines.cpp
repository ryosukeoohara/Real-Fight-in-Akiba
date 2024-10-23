//===========================================================
//
// 集中線[speedlines.cpp]
// Author 大原怜将
//
//===========================================================
#include "speedlines.h"
#include "manager.h"
#include "texture.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	// テクスチャのパス
	const char* TEX_PATH = { "data\\TEXTURE\\speedlines.png" };
	D3DXVECTOR2 TEX_DIVISION = D3DXVECTOR2(5, 2);
	const int TEX_DIVISION_X = 5;
	const int TEX_DIVISION_Y = 2;
}

//===========================================================
// コンストラクタ
//===========================================================
CSpeedLines::CSpeedLines()
{
	m_nChangeCount = 0;
	m_nTexCount = 1;
}

//===========================================================
// デストラクタ
//===========================================================
CSpeedLines::~CSpeedLines()
{
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CSpeedLines::Init(void)
{
	CObject2D::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CSpeedLines::Uninit(void)
{
	CObject2D::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CSpeedLines::Update(void)
{
	CObject2D::Update();

	CObject2D::SetAnimation();
}

//===========================================================
// 描画処理
//===========================================================
void CSpeedLines::Draw(void)
{
	CObject2D::Draw();
}

//===========================================================
// 生成処理
//===========================================================
CSpeedLines* CSpeedLines::Create(void)
{
	CSpeedLines* pSpeedLines = nullptr;

	if(pSpeedLines == nullptr)
	{
		pSpeedLines = new CSpeedLines;

		pSpeedLines->Init();

		pSpeedLines->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\speedlines.png"));
		pSpeedLines->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		pSpeedLines->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
		pSpeedLines->SetTexPosX(TEX_DIVISION_X);
		pSpeedLines->SetTexPosY(TEX_DIVISION_Y);
		pSpeedLines->SetDraw();
		pSpeedLines->SetAnimation();
	}

	return pSpeedLines;
}
