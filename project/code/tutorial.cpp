//===========================================================
//
// チュートリアル処理[tutorial.cpp]
// Author 大原怜将
//
//===========================================================
#include "tutorial.h"
#include "input.h"
#include "InputKeyBoard.h"
#include "InputJoyPad.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "debugproc.h"
#include "renderer.h"
#include "camera.h"
#include "field.h"
#include "map.h"
#include "player.h"
#include "enemymanager.h"
#include "enemy_weak.h"
#include "itemmanager.h"
#include "collision.h"
#include "tutorialUI.h"
#include "MyEffekseer.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CPlayer *CTutorial::m_pPlayer = nullptr;
CEnemyManager *CTutorial::m_pEnemyManager = nullptr;
CItemManager* CTutorial::m_pItemManager = nullptr;
CMap *CTutorial::m_pMap = nullptr;
CTutorialUI *CTutorial::m_pUI = nullptr;

//===========================================================
// コンストラクタ
//===========================================================
CTutorial::CTutorial()
{
	m_pField = nullptr;
	m_pEnemy = nullptr;
	m_pItemManager = nullptr;
	m_pStart = nullptr;
}

//===========================================================
// デストラクタ
//===========================================================
CTutorial::~CTutorial()
{
	
}

//===========================================================
// 生成処理
//===========================================================
CTutorial * CTutorial::Create(void)
{
	CTutorial *pTutorial = NULL;

	if (pTutorial == NULL)
	{
		pTutorial = new CTutorial;

		pTutorial->Init();
	}

	return pTutorial;
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CTutorial::Init(void)
{
	// フィールドの生成
	if (m_pField == nullptr)
	{
		m_pField = CField::Create();
		m_pField->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\Field002.jpg"));
		m_pField->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pField->SetSize(5000.0f, 5000.0f);
		m_pField->SetDraw(true);
	}

	// 当たり判定
	if (m_Collision == nullptr)
	{
		m_Collision = CCollision::Create();
	}

	// マップの生成
	if (m_pMap == nullptr)
	{
		m_pMap = CMap::Create();
	}

	// 敵マネージャの生成
	if (m_pEnemyManager == nullptr)
	{
		m_pEnemyManager = CEnemyManager::Create();
	}

	if (m_pItemManager == nullptr)
	{
		m_pItemManager = CItemManager::Create("data\\TEXT\\itemset_tutorial.txt");
	}

	if (m_pUI == nullptr)
	{
		m_pUI = CTutorialUI::Create();
	}
	
	// プレイヤーの生成
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = CPlayer::Create();
		m_pPlayer->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 500.0f));
		m_pPlayer->SetMobile();
	}

	if (m_pStart == nullptr)
	{
		m_pStart = CObject2D::Create(7);
		m_pStart->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\start.png"));
		m_pStart->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.9f, 0.0f));
		m_pStart->SetSize(200.0f, 50.0f);
		m_pStart->SetDraw(true);
	}

	CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CTutorial::Uninit(void)
{
	CManager::GetInstance()->GetSound()->Stop();

	// 破棄
	// フィールド
	if (m_pField != nullptr)
	{
		m_pField->Uninit();
		m_pField = nullptr;
	}

	// 背景
	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	// マップ
	if (m_pMap != nullptr)
	{
		m_pMap->Uninit();
		m_pMap = nullptr;
	}

	if (m_pEnemy != nullptr)
	{
		m_pEnemy->Uninit();
		m_pEnemy = nullptr;
	}

	// 敵の破棄
	if (m_pEnemyManager != nullptr)
	{
		m_pEnemyManager->Uninit();
		m_pEnemyManager = nullptr;
	}

	// アイテムの破棄
	if (m_pItemManager != nullptr)
	{
		m_pItemManager->Uninit();
		m_pItemManager = nullptr;
	}

	if (m_pUI != nullptr)
	{
		m_pUI->Uninit();
		m_pUI = nullptr;
	}

	// プレイヤー
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	CObject::ReleaseAll();
}

//===========================================================
// 更新処理
//===========================================================
void CTutorial::Update(void)
{
	//キーボードを取得
	CInputKeyboard *InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//ゲームパッドを取得
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();

	//フェードの情報を取得
	CFade *pFade = CManager::GetInstance()->GetFade();

	if (InputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoyPad->GetTrigger(CInputJoyPad::BUTTON_START, 0) == true)
	{//ENTERキーを押したかつシーンがタイトルのとき

		if (pFade->Get() != pFade->FADE_OUT)
		{
			//シーンをゲームに遷移
			pFade->Set(CScene::MODE_GAME);
		}
	}

	if (InputKeyboard->GetTrigger(DIK_K) == true)
	{
		MyEffekseer::EffectCreate(CMyEffekseer::TYPE_IMPACT, true, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (m_pEnemyManager != nullptr)
	{
		m_pEnemyManager->Update();

		if (CEnemyManager::GetNum() <= 0)
		{
			m_pEnemyManager = nullptr;

			m_pEnemyManager = CEnemyManager::Create();
		}
	}
	
	// すべて更新
	CObject::UpdateAll();
}

//===========================================================
// 描画処理
//===========================================================
void CTutorial::Draw(void)
{
	
}