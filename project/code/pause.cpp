//===========================================================
//
// ポーズ処理[pause.cpp]
// Author 大原怜将
//
//===========================================================
#include "pause.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "InputKeyBoard.h"
#include "InputJoyPad.h"
#include "fade.h"
#include "game.h"

//===========================================================
// 定数定義
//===========================================================
namespace 
{
	const D3DXVECTOR3 TEXPOS[3] =   // 位置
	{
		D3DXVECTOR3(640.0f, 350.0f, 0.0f),
		D3DXVECTOR3(640.0f, 500.0f, 0.0f),
		D3DXVECTOR3(640.0f, 650.0f, 0.0f),
	};

	const D3DXVECTOR2 TEX_SIZE = { 140.0f, 45.0f };  // サイズ
}

//===========================================================
// テクスチャネーム
//===========================================================
const char *CPause::m_apTexName[TYPE_MAX] =
{
	"data\\TEXTURE\\pause000.png",     // 続ける
	"data\\TEXTURE\\pause001.png",     // やり直す
	"data\\TEXTURE\\pause002.png",     // タイトル画面に戻る
	"data\\TEXTURE\\iron000.jpg",      // 黒い背景
	"data\\TEXTURE\\fream.png",		   // 枠
};

//===========================================================
// コンストラクタ
//===========================================================
CPause::CPause()
{
	for (int nCount = 0; nCount < MENU_MAX; nCount++)
	{
		m_aMenu[nCount].Object2D = nullptr;
		m_aMenu[nCount].col = D3DXCOLOR(0.0, 0.0f, 0.0f, 0.0f);
	}

	m_pBg = nullptr;
	m_pFrame = nullptr;
	m_nSelect = 0;
}

//===========================================================
// デストラクタ
//===========================================================
CPause::~CPause()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CPause::Init(void)
{
	if (m_pFrame == nullptr)
	{
		m_pFrame = CObject2D::Create();
		m_pFrame->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(m_apTexName[TYPE_FRAME]));
		m_pFrame->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		m_pFrame->SetSize(270.0f, 270.0f);
		m_pFrame->SetDraw(false);
	}

	for (int nCount = 0; nCount < MENU_MAX; nCount++)
	{
		if (m_aMenu[nCount].Object2D == nullptr)
		{
			m_aMenu[nCount].Object2D = CObject2D::Create();
			m_aMenu[nCount].Object2D->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(m_apTexName[nCount]));
			m_aMenu[nCount].Object2D->SetPosition(D3DXVECTOR3(640.0f, 250.0f + nCount * 150.0f, 0.0f));
			m_aMenu[nCount].Object2D->SetSize(TEX_SIZE.x, TEX_SIZE.y);
			m_aMenu[nCount].Object2D->SetDraw(false);
		}
	}

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CPause::Uninit(void)
{
	
}

//===========================================================
// 更新処理
//===========================================================
void CPause::Update(void)
{
	//キーボードを取得
	CInputKeyboard *InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//ゲームパッドを取得
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();

	//サウンドを取得
	CSound *pSound = CManager::GetInstance()->GetSound();

	//フェードの情報を取得
	CFade *pFade = CManager::GetInstance()->GetFade();

	//上に移動----------------------------------------------
	if (InputKeyboard->GetTrigger(DIK_S) == true || pInputJoyPad->GetTrigger(CInputJoyPad::BUTTON_DOWN, 0) == true)
	{//Wキーが押された

		m_nSelect = (m_nSelect + 1) % MENU_MAX;
		SetCol();
	}
	else if (InputKeyboard->GetTrigger(DIK_W) == true || pInputJoyPad->GetTrigger(CInputJoyPad::BUTTON_UP, 0) == true)
	{
		m_nSelect = (m_nSelect - 1 + MENU_MAX) % MENU_MAX;
		SetCol();
	}

	//上に移動----------------------------------------------
	if (InputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoyPad->GetTrigger(CInputJoyPad::BUTTON_A, 0) == true)
	{//Wキーが押された

		switch (m_nSelect)
		{
		case MENU_COUTINUE:
			CGame::SetbPause(false);
			break;

		case MENU_RETRY:

			if (pFade->Get() != pFade->FADE_OUT)
			{
				//シーンをゲームに遷移
				pFade->Set(CScene::MODE_GAME);
			}
			break;

		case MENU_QUIT:

			if (pFade->Get() != pFade->FADE_OUT)
			{
				//シーンをタイトル遷移
				pFade->Set(CScene::MODE_TITLE);
			}
			break;

		default:
			break;
		}
	}

	SetCol();
}

//===========================================================
// 描画処理
//===========================================================
void CPause::Draw(void)
{
	
}

//===========================================================
// 生成処理
//===========================================================
CPause * CPause::Create(void)
{
	// ポーズのポインタ
	CPause *pPause = nullptr;

	pPause = new CPause;

	if (pPause != nullptr)
	{//	使用されていたら

		// 初期化処理
		pPause->Init();
	}
	else
	{// 使用されていなかったらnullを返す

		return nullptr;
	}

	return pPause;
}

//===========================================================
// 描画するかどうか
//===========================================================
void CPause::SetDraw(bool bPause)
{
	if (m_pBg != nullptr)
	{
		m_pBg->SetDraw(bPause);
	}

	if (m_pFrame != nullptr)
	{
		m_pFrame->SetDraw(bPause);
	}

	for (int nCount = 0; nCount < MENU_MAX; nCount++)
	{
		if (m_aMenu[nCount].Object2D != nullptr)
		{
			m_aMenu[nCount].Object2D->SetDraw(bPause);
		}
	}
}

//===========================================================
// 描画処理
//===========================================================
void CPause::SetCol(void)
{
	for (int nCount = 0; nCount < MENU_MAX; nCount++)
	{
		if (m_nSelect != nCount)
		{
			m_aMenu[nCount].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			m_aMenu[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		if (m_aMenu[nCount].Object2D != nullptr)
		{
			m_aMenu[nCount].Object2D->SetColor(m_aMenu[nCount].col);
		}
	}
}