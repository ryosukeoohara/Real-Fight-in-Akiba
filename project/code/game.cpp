//===========================================================
//
// �Q�[���Ǘ�[game.cpp]
// Author �匴�叫
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
#include "audiencemanager.h"
#include "InputKeyBoard.h"
#include "InputJoyPad.h"

//================================================================
// �ÓI�����o�ϐ�
//================================================================
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
// �R���X�g���N�^
//===========================================================
CGame::CGame()
{
	m_bUse = false;
	m_bPause = false;
	m_bOnStage = false;
	m_nOnStageCounter = 0;
	m_pEdit = nullptr;
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CGame::CGame(CScene::MODE mode)
{
	m_bUse = false;
	m_bPause = false;
	m_bOnStage = false;
	m_nOnStageCounter = 0;
	m_pEdit = nullptr;
}

//===========================================================
//�@�f�X�g���N�^
//===========================================================
CGame::~CGame()
{

}

//===========================================================
// ����
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
// �E�F�[�u�Ǘ�
//===========================================================
void CGame::WaveControll(void)
{
	//�t�F�[�h�̏����擾
	CFade *pFade = CManager::GetInstance()->GetFade();

	switch (m_Wave)
	{
	case CGame::WAVE_00:

		CManager::GetInstance()->GetCamera()->Reset();
		m_bOnStage = false;

		if (pFade->Get() != pFade->FADE_BLACK)
			pFade->Set();

		if (pFade->GetCol() >= 1.0f)
		{
			m_pEnemyManager->SetBossEnemy();

			if (m_pPlayer != nullptr)
			{
				m_pPlayer->SetLife(200);
				m_pPlayer->SetImmobile();
				m_pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				m_pPlayer->SetUseMicroCount(0);
				m_pPlayer->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 500.0f));
				m_pPlayer->SetRotition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				if (m_pPlayer->GetMotion() != nullptr)
					m_pPlayer->GetMotion()->Set(CPlayer::TYPE_NEUTRAL);

			}

			m_Wave = WAVE_01;
		}
		
		break;

	case CGame::WAVE_01:

		if (pFade->Get() != pFade->FADE_OUT)
			pFade->Set(CScene::MODE_RESULT);
	
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
//�@����������
//===========================================================
HRESULT CGame::Init(void)
{
	m_Wave = WAVE_00;
	m_bPause = false;
	CField *pField = new CField;
	pField->Init();
	pField->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\FIELD\\concreat.png"));
	pField->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pField->SetSize(5000.0f, 5000.0f);
	pField->SetDraw(true);

	// �}�b�v�̐���
	if (m_pMap == nullptr)
	{
		m_pMap = CMap::Create();
	}

	// �����蔻��
	if (m_Collision == nullptr) 
	{
		m_Collision = CCollision::Create();
	}

	// �A�C�e���}�l�[�W���̐���
	if (m_pItemManager == nullptr)
	{
		m_pItemManager = CItemManager::Create("data\\TEXT\\itemset_game.txt");
	}

	// �G�}�l�[�W���̐���
	if (m_pEnemyManager == nullptr)
	{
		m_pEnemyManager = CEnemyManager::Create();
	}

	// �v���C���[�̐���
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 500.0f));
	}

	// �|�[�Y�̐���
	if (m_pPause == nullptr)
	{
		m_pPause = CPause::Create();
	}

	CAudienceManager* pAudienceManager = CAudienceManager::Create();

	CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME);

	return S_OK;
}

//===========================================================
//�@�I������
//===========================================================
void CGame::Uninit(void)
{
	// �G��|�������擾
	int m = CPlayer::GetInstance()->GetDefeat();
	CManager::GetInstance()->SetDefeat(CPlayer::GetInstance()->GetDefeat());

	CManager::GetInstance()->GetSound()->Stop();

	// �|�[�Y�̔j��
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		m_pPause = nullptr;
	}

	// �v���C���[�̔j��
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}

	// �A�C�e���̔j��
	if (m_pItemManager != nullptr)
	{
		m_pItemManager->Uninit();
		m_pItemManager = nullptr;
	}

	// �G�̔j��
	if (m_pEnemyManager != nullptr)
	{
		m_pEnemyManager->Uninit();
		m_pEnemyManager = nullptr;
	}

	// �}�b�v�̔j��
	if (m_pMap != nullptr)
	{
		m_pMap->Uninit();
		m_pMap = nullptr;
	}

	CObject::ReleaseAll();
}

//===========================================================
//�@�X�V����
//===========================================================
void CGame::Update(void)
{
	//�L�[�{�[�h���擾
	CInputKeyboard *InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//�}�E�X���擾
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

	//�Q�[���p�b�h���擾
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();

	//�t�F�[�h�̏����擾
	CFade *pFade = CManager::GetInstance()->GetFade();

	if (CManager::GetInstance()->GetKeyBoard()->GetTrigger(DIK_P) == true || CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoyPad::BUTTON_START, 0) == true)
	{
		m_bPause = m_bPause ? false : true;

		if (m_pPause != nullptr)
			m_pPause->SetDraw(m_bPause);

	}

	if (InputKeyboard->GetTrigger(DIK_F7) == true)
		m_bUse = m_bUse ? false : true;

	if (m_bPause == true)
	{
		if (m_pPause != nullptr)
		{
			m_pPause->Update();
			m_pPause->SetDraw(m_bPause);
		}

		return;
	}

	if (pFade->Get() != pFade->FADE_BLACK &&  pFade->GetCol() == 0.0f && m_bOnStage == false && CManager::GetInstance()->GetCamera()->GetMode() == CCamera::MODE_GAME)
	{
		CManager::GetInstance()->GetCamera()->SetMode(CCamera::MODE_ONSTAGE);
		m_bOnStage = true;
	}

	//if (InputKeyboard->GetTrigger(DIK_RETURN) == true)
	//{//ENTER�L�[�����������V�[�����^�C�g���̂Ƃ�

	//	if (pFade->Get() != pFade->FADE_OUT)
	//	{
	//		m_Wave = WAVE_00;
	//		//�V�[�����Q�[���ɑJ��
	//		pFade->Set(CScene::MODE_RESULT);

	//		return;
	//	}
	//}

	if (InputKeyboard->GetTrigger(DIK_DELETE) == true)
	{//ENTER�L�[�����������V�[�����^�C�g���̂Ƃ�

		if (m_pEnemyManager != nullptr)
			m_pEnemyManager->AllDelete();
	}

	if (m_pEnemyManager != nullptr && m_pEnemyManager->GetNum() <= 0)
		WaveControll();

	//���ׂĂ̍X�V����
	CObject::UpdateAll();
	
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	pDebugProc->Print("\n�V�[���F�Q�[��");
}

//===========================================================
//�@�`�揈��
//===========================================================
void CGame::Draw(void)
{
	
}