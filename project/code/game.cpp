//===========================================================
//
// ゲーム管理[game.cpp]
// Author 大原怜将
//
//===========================================================
#include "game.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "object.h"
#include "debugproc.h"
#include "fade.h"
#include "time.h"
#include "score.h"
#include "pause.h"
#include "player.h"
#include "field.h"
#include "texture.h"
#include "enemy.h"
#include "enemymanager.h"
#include "collision.h"
#include "itemmanager.h"
#include "map.h"
#include "audience.h"
#include "motion.h"
#include "edit.h"
#include "mapobject.h"
#include "audiencemanager.h"
#include "InputKeyBoard.h"
#include "InputJoyPad.h"
#include "ripples.h"
#include "MyEffekseer.h"

//================================================================
// 静的メンバ変数
//================================================================
CGame* CGame::m_pInstance = nullptr;
CPause *CGame::m_pPause = nullptr;
CPlayer *CGame::m_pPlayer = nullptr;
CEnemy *CGame::m_pEnemy = nullptr;
CEnemyManager *CGame::m_pEnemyManager = nullptr;
CCollision *CGame::m_Collision = nullptr;
CItemManager *CGame::m_pItemManager = nullptr;
CMap *CGame::m_pMap = nullptr;
CGame::WAVE CGame::m_Wave = WAVE_00;
bool CGame::m_bPause = false;
bool CGame::m_bOnStage = false;

//===========================================================
// コンストラクタ
//===========================================================
CGame::CGame()
{
	m_bUse = false;
	m_bPause = false;
	m_bOnStage = false;
	m_nOnStageCounter = 0;
	m_pEdit = nullptr;
	m_bFinish = false;

	m_pInstance = this;
}

//===========================================================
// コンストラクタ
//===========================================================
CGame::CGame(CScene::MODE mode)
{
	m_bUse = false;
	m_bPause = false;
	m_bOnStage = false;
	m_nOnStageCounter = 0;
	m_pEdit = nullptr;
	m_bFinish = false;

	m_pInstance = this;
}

//===========================================================
//　デストラクタ
//===========================================================
CGame::~CGame()
{

}

//===========================================================
// 生成
//===========================================================
CGame *CGame::Create(void)
{
	CGame *pGame = NULL;

	if (pGame == NULL)
	{
		pGame = new CGame;

		pGame->Init();
	}

	return pGame;
}

//===========================================================
// ウェーブ管理
//===========================================================
void CGame::WaveControll(void)
{
	//フェードの情報を取得
	CFade *pFade = CManager::GetInstance()->GetFade();

	switch (m_Wave)
	{
	case CGame::WAVE_00:

		CManager::GetInstance()->GetCamera()->Reset();

			//m_pEnemyManager->SetBossEnemy();

			if (m_pPlayer != nullptr)
			{
				m_pPlayer->SetLife(200);
				m_pPlayer->SetImmobile();
				m_pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				m_pPlayer->SetUseMicroCount(0);
				
				if (m_pPlayer->GetMotion() != nullptr)
					m_pPlayer->GetMotion()->Set(CPlayer::TYPE_NEUTRAL);

				CManager::GetInstance()->GetCamera()->ChangeState(new CameraZoomOut);

			}

			m_Wave = WAVE_01;
		
		
		break;

	case CGame::WAVE_01:

		break;

	case CGame::MAVE_CLEAR:

		break;

	case CGame::WAVE_MAX:

		break;

	default:
		break;
	}
}

//===========================================================
//　初期化処理
//===========================================================
HRESULT CGame::Init(void)
{
	m_Wave = WAVE_00;
	m_bPause = false;
	CField *pField = new CField;
	pField->Init();
	pField->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\Field002.jpg"));
	pField->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pField->SetSize(5000.0f, 5000.0f);
	pField->SetDraw(true);

	// マップの生成
	if (m_pMap == nullptr)
	{
		m_pMap = CMap::Create();
	}

	// 当たり判定
	if (m_Collision == nullptr) 
	{
		m_Collision = CCollision::Create();
	}

	// アイテムマネージャの生成
	if (m_pItemManager == nullptr)
	{
		m_pItemManager = CItemManager::Create("data\\TEXT\\itemset_game.txt");
	}

	// 敵マネージャの生成
	if (m_pEnemyManager == nullptr)
	{
		m_pEnemyManager = CEnemyManager::Create();
	}

	// プレイヤーの生成
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 500.0f));
	}

	// ポーズの生成
	if (m_pPause == nullptr)
	{
		m_pPause = CPause::Create();
	}

	CAudienceManager* pAudienceManager = CAudienceManager::Create();

	CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//===========================================================
//　終了処理
//===========================================================
void CGame::Uninit(void)
{
	CManager::GetInstance()->GetSound()->Stop();

	// ポーズの破棄
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		m_pPause = nullptr;
	}

	// プレイヤーの破棄
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	// アイテムの破棄
	if (m_pItemManager != nullptr)
	{
		m_pItemManager->Uninit();
		m_pItemManager = nullptr;
	}

	// 敵の破棄
	if (m_pEnemyManager != nullptr)
	{
		m_pEnemyManager->Uninit();
		m_pEnemyManager = nullptr;
	}

	// マップの破棄
	if (m_pMap != nullptr)
	{
		m_pMap->Uninit();
		m_pMap = nullptr;
	}

	CObject::ReleaseAll();
}

//===========================================================
//　更新処理
//===========================================================
void CGame::Update(void)
{
	//キーボードを取得
	CInputKeyboard *InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//マウスを取得
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

	//ゲームパッドを取得
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();

	//フェードの情報を取得
	CFade *pFade = CManager::GetInstance()->GetFade();

	if (CManager::GetInstance()->GetKeyBoard()->GetTrigger(DIK_P) == true || CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoyPad::BUTTON_START, 0) == true)
	{
		m_bPause = m_bPause ? false : true;

		if (m_pPause != nullptr)
			m_pPause->SetDraw(m_bPause);

	}

	if (InputKeyboard->GetTrigger(DIK_F7) == true)
	{
		m_bUse = m_bUse ? false : true;
	}
		
	// エディットモード中
	if (m_bUse)
	{
		if (m_pEdit == nullptr)
		{
			m_pEdit = new CEdit;
			m_pEdit->Init();
			CManager::GetInstance()->GetCamera()->ChangeState(new EditCamera);
		}

		if(m_pEdit != nullptr)
		   m_pEdit->Update();
			
		return;
	}

	if (!m_bUse)
	{
		if (m_pEdit != nullptr)
		{
			m_pEdit->Uninit();
			delete m_pEdit;
			m_pEdit = nullptr;
			CManager::GetInstance()->GetCamera()->ChangeState(new FollowPlayerCamera);
		}
	}

	// ポーズ中
	if (m_bPause)
	{
		if (m_pPause != nullptr)
		{
			m_pPause->Update();
			m_pPause->SetDraw(m_bPause);
		}

		return;
	}

	if (pFade->Get() != pFade->FADE_BLACK &&  pFade->GetCol() <= 0.8f && m_bOnStage == false)
	{
		CManager::GetInstance()->GetCamera()->ChangeState(new CameraEnemyOverview);
		m_bOnStage = true;
	}

	if (m_bFinish)
		return;

	if (InputKeyboard->GetTrigger(DIK_V))
		CRipples::Create(m_pPlayer->GetPosition());

	//if (InputKeyboard->GetTrigger(DIK_RETURN) == true)
	//{//ENTERキーを押したかつシーンがタイトルのとき

	//	if (pFade->Get() != pFade->FADE_OUT)
	//	{
	//		m_Wave = WAVE_00;
	//		//シーンをゲームに遷移
	//		pFade->Set(CScene::MODE_RESULT);

	//		return;
	//	}
	//}

	if (InputKeyboard->GetTrigger(DIK_DELETE) == true)
	{//ENTERキーを押したかつシーンがタイトルのとき

		if (m_pEnemyManager != nullptr)
			m_pEnemyManager->AllDelete();
	}

	if (m_pEnemyManager != nullptr && m_pEnemyManager->GetNum() <= 0)
		WaveControll();

	//すべての更新処理
	CObject::UpdateAll();
	
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	pDebugProc->Print("\nシーン：ゲーム");
}

//===========================================================
//　描画処理
//===========================================================
void CGame::Draw(void)
{
	if (m_bUse)
	{
		if (m_pEdit != nullptr)
			m_pEdit->Draw();

	}

	/*if (m_pMap != nullptr)
		m_pMap->Draw();*/
}