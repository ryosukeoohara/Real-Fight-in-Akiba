//===========================================================
//
// タイトル処理[title.cpp]
// Author 大原怜将
//
//===========================================================
#include "title.h"
#include "texture.h"
#include "game.h"
#include "debugproc.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "input.h"
#include "fade.h"
#include "field.h"
#include "map.h"
#include "player.h"
#include "InputKeyBoard.h"
#include "InputJoyPad.h"
#include "speedlines.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const D3DXVECTOR3 TEXPOS[3] =   // 位置
	{
		D3DXVECTOR3(640.0f, 0.0f, 350.0f),
		D3DXVECTOR3(640.0f, 0.0f, 550.0f),
		D3DXVECTOR3(640.0f, 0.0f, 750.0f),
	};

	const D3DXVECTOR3 CAMERAPOS[3] =   // 位置
	{
		D3DXVECTOR3(640.0f, 0.0f, -350.0f),
		D3DXVECTOR3(640.0f, 0.0f, -550.0f),
		D3DXVECTOR3(640.0f, 0.0f, -750.0f),
	};
}

//===========================================================
//　静的メンバ変数
//===========================================================
CPlayer *CTitle::m_pPlayer = nullptr;

//===========================================================
//コンストラクタ
//===========================================================
CTitle::CTitle()
{
	m_pField = nullptr;
	m_pBg = nullptr;
	m_pMap = nullptr;
	m_pStart = nullptr;
	m_nCnt = 0;
	m_nCntPos = 0;
}

//===========================================================
//デストラクタ
//===========================================================
CTitle::~CTitle()
{

}

//===========================================================
//生成
//===========================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle = nullptr;

	if (pTitle == nullptr)
	{
		pTitle = new CTitle;

		pTitle->Init();
	}

	return pTitle;
}

//===========================================================
//初期化処理
//===========================================================
HRESULT CTitle::Init(void)
{
	CField *pField = new CField;

	if (pField != nullptr)
	{
		pField->Init();
		pField->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\Field002.jpg"));
		pField->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		pField->SetSize(5000.0f, 5000.0f);
		pField->SetDraw(true);
	}

	// マップの生成
	if (m_pMap == nullptr)
	{
		m_pMap = CMap::Create();
	}

	if (m_pPlayer == nullptr)
	{
		m_pPlayer = CPlayer::Create();
		m_pPlayer->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pPlayer->SetRotition(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}

	if (m_pStart == nullptr)
	{
		m_pStart = CObject2D::Create();
		m_pStart->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\any.png"));
		m_pStart->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.7f, 0.0f));
		m_pStart->SetSize(200.0f, 50.0f);
		m_pStart->SetDraw(true);
	}

	if (m_pBg == nullptr)
	{
		m_pBg = CObject2D::Create();
		m_pBg->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\aaa.png"));
		m_pBg->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, 0.0f));
		m_pBg->SetSize(300.0f, 200.0f);
		m_pBg->SetDraw(true);
	}

	CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_BGM_TITLE);
	
	return S_OK;
}

//===========================================================
//終了処理
//===========================================================
void CTitle::Uninit(void)
{
	CManager::GetInstance()->GetSound()->Stop();

	if (m_pField != nullptr)
	{
		m_pField->Uninit();
		m_pField = nullptr;
	}

	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	// マップの破棄
	if (m_pMap != nullptr)
	{
		m_pMap->Uninit();
		m_pMap = nullptr;
	}

	if (m_pStart != nullptr)
	{
		m_pStart->Uninit();
		m_pStart = nullptr;
	}

	// プレイヤーの破棄
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}
	
	CObject::ReleaseAll();
}

//===========================================================
//更新処理
//===========================================================
void CTitle::Update(void)
{
	//キーボードを取得
	CInputKeyboard *InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//マウスを取得
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

	//ゲームパッドを取得
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();

	//フェードの情報を取得
	CFade *pFade = CManager::GetInstance()->GetFade();

	if (InputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoyPad->GetTrigger(CInputJoyPad::BUTTON_A, 0) == true || pInputJoyPad->GetTrigger(CInputJoyPad::BUTTON_START, 0) == true)
	{//ENTERキーを押したかつシーンがタイトルのとき

		if (pFade->Get() != pFade->FADE_OUT)
		{
			//シーンをゲームに遷移
			pFade->Set(CScene::MODE_TUTORIAL);
		}
	}

	if (m_pPlayer != nullptr)
	{
		m_pPlayer->TitleWalk();
	}

	CManager::GetInstance()->GetDebugProc()->Print("現在のシーン：タイトル");
}

//===========================================================
//描画処理
//===========================================================
void CTitle::Draw(void)
{
	
}