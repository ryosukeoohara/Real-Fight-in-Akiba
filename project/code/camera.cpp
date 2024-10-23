//===========================================================
//
// �J�������� [camera.cpp]
// Author : �匴�@�叫
//
//===========================================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "InputKeyBoard.h"
#include "InputMouse.h"
#include "InputJoyPad.h"
#include "title.h"
#include "game.h"
#include "player.h"
#include "enemymanager.h"
#include "appearanceUI.h"
#include "tutorial.h"
#include "enemy.h"
#include "motion.h"
#include "enemy_boss.h"
#include "sound.h"
#include "texture.h"
#include "object2D.h"
#include "speedlines.h"
#include "utility.h"
#include "fade.h"
#include "character.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CCamera* CCamera::m_pCamera = nullptr;

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const D3DXVECTOR3 HEAT_CAMERAROT = { 0.0f, -D3DX_PI * 0.75f, D3DX_PI * -0.38f };   // �ڕW�̊p�x
	const D3DXVECTOR3 ONSTAGE_POSV = { 0.0f, 150.0f, -100.0f };   // �L�����N�^�[���o�ꉉ�o�p�J�����̖ڕW�̎��_�̈ʒu
	const D3DXVECTOR3 ONSTAGE_POSR = { 0.0f, 50.0f, 0.0f };       // �L�����N�^�[���o�ꉉ�o�p�J�����̖ڕW�̒����_�̈ʒu
	const float CAMERA_DISTNCE = 200.0f;  // ����
	const float CAMERA_MOVE = 2.0f;       // �ړ���
	const float CAMERA_ROT_Y = 0.03f;     // Y���̉�]��
	const float CORRECT_TO_FACT = 0.05f;  // �␳����l
	const int SHAKE_TIME = 60;  // �����Ă���h��Ă��鎞��
	const int TIME_TO_CORRECT = 90;       // �␳���鎞��
	const int FOCUS_ENEMY_TIME = 120;          // �Q�[���X�^�[�g���ɓG�𒍎����鎞��
	const int FOCUS_COUNT = 3;            // �Q�[���X�^�[�g���ɃJ�����̈ʒu���ς���
	const int ZOOM_OUT_TIME = 120;
	const D3DXVECTOR3 CAMERA_POSV[3] =
	{
		{ -500.0f, 300.0f, 1000.0f},
		{ 300.0f, 30.0f, 120.0f},
		{ 650.0f, 300.0f, -600.0f},
	};

	const D3DXVECTOR3 CAMERA_POS[3] =
	{
		{ -740.0f, 500.0f, 1000.0f},
		{ -53.0f, 30.0f, 120.0f},
		{ 650.0f, 300.0f, -1000.0f},
	};

	const float CAMERA_ROT[3] =
	{
		D3DX_PI,
		D3DX_PI * 0.5f,
		D3DX_PI * -0.5f,
	};
}

//================================================================
// �R���X�g���N�^
//================================================================
CCamera::CCamera()
{
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mode = MODE_NONE;
	m_Info.nCounter = 0;
	m_Info.fLength = 0.0f;

	m_pEnemy = nullptr;
	m_pCamera = this;
}

//================================================================
// �f�X�g���N�^
//================================================================
CCamera::~CCamera()
{

}

//================================================================
// ����������
//================================================================
void CCamera::Init(void)
{
	CScene *pScene = CManager::GetInstance()->GetScene();

	if (pScene->GetMode() == CScene::MODE_TITLE)
	{
		m_Info.posV = D3DXVECTOR3(0.0f, 200.0f, -500.0f);
		m_Info.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Info.posU = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// ����
	m_Info.fLength = CAMERA_DISTNCE;
}

//================================================================
// �I������
//================================================================
void CCamera::Uninit(void)
{
	m_pCamera = nullptr;
}

//================================================================
// �X�V����
//================================================================
void CCamera::Update(void)
{
	// �X�e�[�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	CManager::GetInstance()->GetDebugProc()->Print("\n�J�����̒����_�̈ʒu�F%f,%f,%f\n", m_Info.posR.x, m_Info.posR.y, m_Info.posR.z);
	CManager::GetInstance()->GetDebugProc()->Print("\n�J�����̎��_�̈ʒu  �F%f,%f,%f\n", m_Info.posV.x, m_Info.posV.y, m_Info.posV.z);
	CManager::GetInstance()->GetDebugProc()->Print("\n�J�����̌���        �F%f,%f,%f\n", m_Info.rot.x, m_Info.rot.y, m_Info.rot.z);
} 

//================================================================
// �ݒ菈��
//================================================================
void CCamera::SetCamera(void)
{
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixPerspectiveFovLH(&m_Info.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		100000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_Info.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_Info.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_Info.mtxView,
		&m_Info.posV,
		&m_Info.posR,
		&m_Info.posU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_Info.mtxView);
}

//================================================================
// �������Z�b�g
//================================================================
void CCamera::Reset(void)
{
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//================================================================
// ���[�h�ݒ�
//================================================================
void CCamera::SetMode(MODE type)
{
	m_mode = type;
}

//================================================================
// �����_�ݒ�
//================================================================
void CCamera::SetPositionR(D3DXVECTOR3 pos)
{
	m_Info.posR = pos;
}

//================================================================
// ���_�ݒ�
//================================================================
void CCamera::SetPositionV(D3DXVECTOR3 pos)
{
	m_Info.posV = pos;
}

//================================================================
// �����ݒ�
//================================================================
void CCamera::SetRotation(D3DXVECTOR3 Rot)
{
	m_Info.rot = Rot;

	m_Info.rot.y = utility::CorrectAngle(m_Info.rot.y);
}

//================================================================
// �����ݒ�
//================================================================
void CCamera::SetDistnce(float fLen)
{
	m_Info.fLength = fLen;
}

//================================================================
// �X�e�C�g�̕ύX
//================================================================
void CCamera::ChangeState(CCameraState* pBehaviour)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}
		
	m_pState = pBehaviour;
}

//================================================================
// �ڕW�̈ʒu�܂ŕ␳���鏈��
//================================================================
void CCamera::AdjustToTarget(D3DXVECTOR3 targetPosR, D3DXVECTOR3 targetPosV, float fCorrent)
{
	D3DXVECTOR3 posDestR = targetPosR - m_Info.posR;
	m_Info.posR = m_Info.posR + posDestR * fCorrent;

	D3DXVECTOR3 posDestV = targetPosV - m_Info.posV;
	m_Info.posV = m_Info.posV + posDestV * fCorrent;

	//pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	//pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
}

//================================================================
// �Ǐ]���鏈��
//================================================================
void CCamera::Follow(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot)
{
	m_Info.posV.x = m_Info.posR.x - sinf(rot.y) * -m_Info.fLength;
	m_Info.posV.z = m_Info.posR.z - cosf(rot.y) * -m_Info.fLength;

	m_Info.posV = D3DXVECTOR3(0.0f + m_Info.posV.x, 150.0f, 0.0f + m_Info.posV.z);
	m_Info.posR = D3DXVECTOR3(posR.x, 75.0f, posR.z);

	//�ڕW�̒����_��ݒ�
	m_Info.posRDest.x = posR.x;
	m_Info.posRDest.z = posR.z;

	//�J�����̈ړ���
	m_Info.move.x = m_Info.posRDest.x - m_Info.posR.x;
	m_Info.move.z = m_Info.posRDest.z - m_Info.posR.z;

	//�ʒu�Ɉړ��ʂ�ۑ�
	m_Info.posR.x += m_Info.move.x;
	m_Info.posR.z += m_Info.move.z;

	m_Info.OldposR = m_Info.posR;
	m_Info.OldposV = m_Info.posV;
	m_Info.Oldrot = m_Info.rot;
	m_Info.fOldLength = m_Info.fLength;
}

//================================================================
// ���[�h�擾
//================================================================
CCamera::MODE CCamera::GetMode(void)
{
	return m_mode;
}

//=============================================================================
// �J�����̃X�e�C�g
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
CCameraState::CCameraState()
{

}

//================================================================
// �f�X�g���N�^
//================================================================
CCameraState::~CCameraState()
{

}

//=============================================================================
// �v���C���[��Ǐ]����J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
FollowPlayerCamera::FollowPlayerCamera()
{

}

//================================================================
// �f�X�g���N�^
//================================================================
FollowPlayerCamera::~FollowPlayerCamera()
{

}

//================================================================
// �X�V
//================================================================
void FollowPlayerCamera::Update(CCamera* pCamera)
{
	if (pCamera == nullptr)
		return;

	// �J�����̏��擾
	CCamera::Info *pCameraInfo = pCamera->GetInfo();

	//�}�E�X�̏����擾
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	//�}�E�X�̈ʒu���擾
	D3DXVECTOR2 MousePos = pInputMouse->GetMouseMove();

	//�Q�[���p�b�h���擾
	CInputJoyPad* pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	if (pInputJoyPad->GetRXStick(CInputJoyPad::STICK_RX, 0) > 0)
	{
		pCameraInfo->rot.y += 0.05f;
	}
	else if (pInputJoyPad->GetRXStick(CInputJoyPad::STICK_RX, 0) < 0)
	{
		pCameraInfo->rot.y -= 0.05f;
	}

	if (pPlayer->GetMobility() == CPlayer::Mobile)
		pCameraInfo->rot.y += MousePos.x * 0.005f;

	// �p�x�̒l���C������
	pCameraInfo->rot.y = utility::CorrectAngle(pCameraInfo->rot.y);

	pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

	D3DXVECTOR3 pos = pPlayer->GetPosition();

	pCameraInfo->posV = D3DXVECTOR3(0.0f + pCameraInfo->posV.x, 150.0f, 0.0f + pCameraInfo->posV.z);
	pCameraInfo->posR = D3DXVECTOR3(pos.x, 75.0f, pos.z);

	//�ڕW�̒����_��ݒ�
	pCameraInfo->posRDest.x = pos.x;
	pCameraInfo->posRDest.z = pos.z;

	//�J�����̈ړ���
	pCameraInfo->move.x = pCameraInfo->posRDest.x - pCameraInfo->posR.x;
	pCameraInfo->move.z = pCameraInfo->posRDest.z - pCameraInfo->posR.z;

	//�ʒu�Ɉړ��ʂ�ۑ�
	pCameraInfo->posR.x += pCameraInfo->move.x;
	pCameraInfo->posR.z += pCameraInfo->move.z;

	pCameraInfo->OldposR = pCameraInfo->posR;
	pCameraInfo->OldposV = pCameraInfo->posV;
	pCameraInfo->Oldrot = pCameraInfo->rot;
	pCameraInfo->fOldLength = pCameraInfo->fLength;
}

//=============================================================================
// �G��Ǐ]����J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
FollowEnemyCamera::FollowEnemyCamera()
{
	
}

//================================================================
// �f�X�g���N�^
//================================================================
FollowEnemyCamera::~FollowEnemyCamera()
{

}

//================================================================
// �X�V
//================================================================
void FollowEnemyCamera::Update(CCamera* pCamera)
{
	CEnemy* pEnemy = CEnemy::GetTop();

	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	if (pEnemy != nullptr)
	{
		CEnemy::INFO* pEnemyInfo = pEnemy->GetInfo();

		// �Ǐ]����
		pCamera->Follow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), pEnemyInfo->pos, pCameraInfo->rot);
	}
}

//=============================================================================
// �Q�[���X�^�[�g���ɓG��l�X�Ȋp�x���猩��J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
CameraEnemyOverview::CameraEnemyOverview()
{
	// �G�̐擪���擾
	m_pEnemy = CEnemy::GetTop();

	// �J�����̃C���X�^���X���擾
	CCamera* pCamera = CCamera::GetInstance();

	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	// �G�̈ʒu���擾
	D3DXVECTOR3 posEnemy = m_pEnemy->GetPosition();

	pCameraInfo->posR = D3DXVECTOR3(posEnemy.x, posEnemy.y + 50.0f, posEnemy.z);
}

//================================================================
// �X�V����
//================================================================
void CameraEnemyOverview::Update(CCamera* pCamera)
{
	// �ǂ��炩���g�p����Ă��Ȃ��Ƃ������𔲂���
	if (pCamera == nullptr || m_pEnemy == nullptr)
		return;

	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	pCamera->AdjustToTarget(pCameraInfo->posR, CAMERA_POSV[m_nLookCount], 0.005f);
	
	// �J�E���g�A�b�v
	m_nFocusCounter++;

	// �K��̎��Ԃ�������
	if (m_nFocusCounter >= FOCUS_ENEMY_TIME)
	{
		m_pEnemy = m_pEnemy->GetNext();

		//m_pEnemy = pNext;
		m_nFocusCounter = 0;
		m_nLookCount++;

		if (m_pEnemy != nullptr)
		{
			// �G�̈ʒu���擾
			D3DXVECTOR3 posEnemy = m_pEnemy->GetPosition();

			pCameraInfo->posR = D3DXVECTOR3(posEnemy.x, posEnemy.y + 50.0f, posEnemy.z);
			pCameraInfo->posV = CAMERA_POS[m_nLookCount];
		}
		else
		{
			pCamera->ChangeState(new CutSceneCamera);
		}
	}

	// 3��G�𒍎�������A�J�����̃X�e�[�g��ύX����
	if (m_nLookCount >= FOCUS_COUNT)
		pCamera->ChangeState(new CutSceneCamera);

}

//=============================================================================
// �G�ɒǏ]���Ȃ���l�X�Ȋp�x���猩��J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
FollowEnemyOverviewCamera::FollowEnemyOverviewCamera()
{
	// �G�̐擪���擾
	m_pEnemy = CEnemy::GetTop();

	// �J�����̃C���X�^���X���擾
	CCamera* pCamera = CCamera::GetInstance();

	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	// �G�̈ʒu���擾
	D3DXVECTOR3 posEnemy = m_pEnemy->GetPosition();

	CPlayer* pPlayer = CPlayer::GetInstance();

	pCameraInfo->posR = D3DXVECTOR3(posEnemy.x, posEnemy.y + 50.0f, posEnemy.z);

	pCameraInfo->rot.y = utility::MoveToPosition(m_pEnemy->GetPosition(), pPlayer->GetPosition(), pCameraInfo->rot.y);

	pCameraInfo->rot.y += D3DX_PI;

	pCameraInfo->rot.y = utility::CorrectAngle(pCameraInfo->rot.y);
}

//================================================================
// �X�V����
//================================================================
void FollowEnemyOverviewCamera::Update(CCamera* pCamera)
{
	// �ǂ��炩���g�p����Ă��Ȃ��Ƃ������𔲂���
	if (pCamera == nullptr || m_pEnemy == nullptr)
		return;

	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();
	
	pCamera->Follow(pCameraInfo->posV, m_pEnemy->GetPosition(), pCameraInfo->rot);

	// �J�E���g�A�b�v
	m_nFocusCounter++;

	// �K��̎��Ԃ�������
	if (m_nFocusCounter >= FOCUS_ENEMY_TIME)
	{
		m_nFocusCounter = 0;
		m_nLookCount++;

		if (m_pEnemy != nullptr)
		{
			// �G�̈ʒu���擾
			D3DXVECTOR3 rotEnemy = m_pEnemy->GetRotition();
			pCameraInfo->rot.y += rotEnemy.y * -0.7f;
			pCameraInfo->rot.y = utility::CorrectAngle(pCameraInfo->rot.y);
		}
	}

	// 3��G�𒍎�������A�J�����̃X�e�[�g��ύX����
	//if (m_nLookCount >= FOCUS_COUNT)
		//pCamera->ChangeState(new CutSceneCamera);

}

//=============================================================================
// �����Ă��J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
CameraZoomOut::CameraZoomOut()
{
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = CCamera::GetInstance()->GetInfo();

	TargetPosV.x = pCameraInfo->posV.x + pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * 400.0f;
	TargetPosV.y = pCameraInfo->posV.y;
	TargetPosV.z = pCameraInfo->posV.z + pCameraInfo->posR.z - sinf(pCameraInfo->rot.y) * 400.0f;

	CPlayer::GetInstance()->SetbDash(false);
}

//================================================================
// �X�V
//================================================================
void CameraZoomOut::Update(CCamera* pCamera)
{
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	pCamera->AdjustToTarget(pCameraInfo->posR, TargetPosV, 0.005f);
	m_nFocusCount++;
	if (m_nFocusCount >= ZOOM_OUT_TIME)
	{
		CEnemyManager::GetInstance()->SetBossEnemy();
		pCamera->ChangeState(new CameraTargetFocus);
	}
		
}

//=============================================================================
// �ڕW�̈ʒu�������J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
CameraTargetFocus::CameraTargetFocus()
{
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = CCamera::GetInstance()->GetInfo();

	CEnemy* pEnemy = CEnemy::GetTop();

	if (pEnemy == nullptr)
		return;

	D3DXVECTOR3 posEnemy = pEnemy->GetPosition();

	TargetPosV.x = sinf(pCameraInfo->rot.y) + (posEnemy.x * 0.8f);
	TargetPosV.y = posEnemy.y + 100.0f;
	TargetPosV.z = cosf(pCameraInfo->rot.y) + (posEnemy.z * 0.8f);
}

//================================================================
// �X�V
//================================================================
void CameraTargetFocus::Update(CCamera* pCamera)
{
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	CEnemy* pEnemy = CEnemy::GetTop();

	if (pEnemy == nullptr)
		return;

	D3DXVECTOR3 posEnemy = pEnemy->GetPosition();

	pCamera->AdjustToTarget(D3DXVECTOR3(posEnemy.x, posEnemy.y + 100.0f, posEnemy.z), TargetPosV, 0.05f);
	m_nFocusCount++;

	if (m_nFocusCount >= FOCUS_ENEMY_TIME)
		pCamera->ChangeState(new FollowEnemyOverviewCamera);

}

//=============================================================================
// �ʒu���Œ�̃J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
FixedCamera::FixedCamera()
{

}

//================================================================
// �f�X�g���N�^
//================================================================
FixedCamera::~FixedCamera()
{

}

//================================================================
// �X�V
//================================================================
void FixedCamera::Update(CCamera* pCamera)
{
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

	pCameraInfo->posV = D3DXVECTOR3(100.0f, 50.0f, 100.0f);
	pCameraInfo->posR = D3DXVECTOR3(50.0f, 50.0f, 10.0f);
}

//=============================================================================
// �L�����N�^�[���o�ꂷ��Ƃ��̃J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
CutSceneCamera::CutSceneCamera()
{
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = CCamera::GetInstance()->GetInfo();

	CEnemy* pEnemy = CEnemy::GetTop();
	CPlayer* pPlayer = CPlayer::GetInstance();

	float EnemyrotY = pEnemy->GetRotition().y;
	pCameraInfo->rot.y = EnemyrotY;
	pCameraInfo->rot.y -= D3DX_PI;
	pCameraInfo->rot.y = utility::CorrectAngle(pCameraInfo->rot.y);

	pCameraInfo->posV.x = sinf(pCameraInfo->rot.y) * 400.0f;
	//pCameraInfo->posV.y = 100.0f;
	pCameraInfo->posV.z = cosf(pCameraInfo->rot.y) * 400.0f;

	TargetPosV.x = sinf(pCameraInfo->rot.y) * 100.0f;
	TargetPosV.y = 75.0f;										   
	TargetPosV.z = cosf(pCameraInfo->rot.y) * 100.0f;

	pCameraInfo->posR = D3DXVECTOR3(pEnemy->GetPosition().x, 75.0f, pEnemy->GetPosition().z);
}

//================================================================
// �f�X�g���N�^
//================================================================
CutSceneCamera::~CutSceneCamera()
{

}

//================================================================
// �X�V
//================================================================
void CutSceneCamera::Update(CCamera* pCamera)
{
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	if (pCameraInfo->nCounter <= TIME_TO_CORRECT)
	{
 		pCamera->AdjustToTarget(pCameraInfo->posR, TargetPosV, CORRECT_TO_FACT);

		pCameraInfo->nCounter++;
	}
	else
	{
		// �J�E���^�[�����Z�b�g
		pCameraInfo->nCounter = 0;

		if (CGame::GetWave() == CGame::WAVE_00)
		{
			CAppearanceUI::Create(CAppearanceUI::TYPE_WEAKNAME);
		}
		else if (CGame::GetWave() == CGame::WAVE_01)
		{
			CAppearanceUI::Create(CAppearanceUI::TYPE_BOSSNAME);
		}

		// �J�������[�h��ύX
		pCamera->ChangeState(new ReturnPlayerBehindCamera);
	}
}

//=============================================================================
// �v���C���[�̔w��ɖ߂��Ă���J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
ReturnPlayerBehindCamera::ReturnPlayerBehindCamera()
{

}

//================================================================
// �f�X�g���N�^
//================================================================
ReturnPlayerBehindCamera::~ReturnPlayerBehindCamera()
{

}

//================================================================
// �X�V
//================================================================
void ReturnPlayerBehindCamera::Update(CCamera* pCamera)
{
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	if (pCameraInfo->nCounter <= TIME_TO_CORRECT)
	{// �J�E���^�[�����ɂȂ�܂�

		// �J������ڕW�̌����܂ŕ␳����
		D3DXVECTOR3 rotDest = pCameraInfo->Oldrot - pCameraInfo->rot;
		pCamera->SetRotation(pCameraInfo->rot + rotDest * CORRECT_TO_FACT);

		// �J������ڕW�̈ʒu�܂ŕ␳����
		D3DXVECTOR3 posDestR = pCameraInfo->OldposR - pCameraInfo->posR;
		pCamera->SetPositionR(pCameraInfo->posR + posDestR * CORRECT_TO_FACT);

		D3DXVECTOR3 posDestV = pCameraInfo->OldposV - pCameraInfo->posV;
		pCamera->SetPositionV(pCameraInfo->posV + posDestV * CORRECT_TO_FACT);

		// �J������ڕW�̋����܂ŕ␳����
		float fLenDest = pCameraInfo->fOldLength - pCameraInfo->fLength;
		pCamera->SetDistnce(pCameraInfo->fLength + fLenDest * CORRECT_TO_FACT);

		// �J�E���^�[���Z
		pCameraInfo->nCounter++;
	}
	else
	{
		// �J�E���^�[�����Z�b�g
		pCameraInfo->nCounter = 0;

		// �J�������[�h��ύX
		pCamera->ChangeState(new FollowPlayerCamera);

		// �v���C���[���s���\�ɂ���
		CPlayer::GetInstance()->SetMobile();

		if (CEnemyManager::GetInstance() == nullptr)
			return;

		// �^�[�Q�b�g�ȊO�̓G�̕`����ĊJ
		CEnemyManager::GetInstance()->RestartDrawing();

		// �G���s���\�ɂ���
		CEnemyManager::GetInstance()->SetMobility();
	}
}

//=============================================================================
// �{�X���j���o�p�̃J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
FinalBlowCamera::FinalBlowCamera()
{
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = CCamera::GetInstance()->GetInfo();
	m_pEnemy = CEnemy::GetTop();
	D3DXMATRIX* mtx = m_pEnemy->GetCharcter()[0]->GetMtxWorld();
	D3DXVECTOR3 pos = m_pEnemy->GetPosition();
	pCameraInfo->posR = D3DXVECTOR3(mtx->_41, mtx->_42/* + 75.0f*/, mtx->_43);
	pCameraInfo->rot.y = CAMERA_ROT[m_nLookCount];
	m_fShankeX = m_fShankeZ = sinf(m_fShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter / SHAKE_TIME)) * 0.5f;

	CSound* pSound = CManager::GetInstance()->GetSound();

	if (pSound != nullptr)
		pSound->Play(CSound::SOUND_LABEL_SE_KO);

	m_pLines = CSpeedLines::Create();
}

//================================================================
// �X�V
//================================================================
void FinalBlowCamera::Update(CCamera* pCamera)
{
	// �ǂ��炩���g�p����Ă��Ȃ��Ƃ������𔲂���
	if (pCamera == nullptr || m_pEnemy == nullptr)
		return;

	if(m_pLines != nullptr)
	   m_pLines->Update();
  
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	pCameraInfo->posV.x = pCameraInfo->posR.x + sinf(pCameraInfo->rot.y) * 200.0f;
	pCameraInfo->posV.z = pCameraInfo->posR.z + cosf(pCameraInfo->rot.y) * 200.0f;

	// �J�E���g�A�b�v
	m_nFocusCounter++;

	// �K��̎��Ԃ�������
	if (m_nFocusCounter >= FOCUS_ENEMY_TIME)
	{
		m_nFocusCounter = 0;
		m_nLookCount++;
		
		if (m_nLookCount >= 3)
		{
			if (m_pLines != nullptr)
			{
				m_pLines->Uninit();
				m_pLines = nullptr;
			}

			CGame::GetInstance()->SetbFinish(false);
			
			CPlayer* pPlayer = CPlayer::GetInstance();

			// ���S�t���O�������Ă�����
			if (m_pEnemy->GetbDeathFlag() && m_pEnemy->GetMotion()->IsFinish())
			{
				//�t�F�[�h�̏����擾
				CFade* pFade = CManager::GetInstance()->GetFade();

				if (pFade->Get() != pFade->FADE_OUT)
					pFade->Set(CScene::MODE_RESULT);

				return;
				
			}

			return;
		}

		pCameraInfo->rot.y = CAMERA_ROT[m_nLookCount];

		CSound* pSound = CManager::GetInstance()->GetSound();

		if (pSound != nullptr)
			pSound->Play(CSound::SOUND_LABEL_SE_KO);

	}
}

//=============================================================================
// �q�[�g�A�N�V�����̃J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
HeatActionCamera::HeatActionCamera()
{

}

//================================================================
// �f�X�g���N�^
//================================================================
HeatActionCamera::~HeatActionCamera()
{

}

//================================================================
// �X�V
//================================================================
void HeatActionCamera::Update(CCamera* pCamera)
{
	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

	if (CPlayer::GetInstance() == nullptr)
		return;

	D3DXVECTOR3 pos = CPlayer::GetInstance()->GetPosition();

	pCameraInfo->posV = D3DXVECTOR3(pCameraInfo->posV.x, 150.0f, 30.0f + pCameraInfo->posV.z);
	pCameraInfo->posR = D3DXVECTOR3(pos.x, 50.0f, pos.z + 10.0f);

	//�ڕW�̒����_��ݒ�
	pCameraInfo->posRDest.x = pos.x;
	pCameraInfo->posRDest.z = pos.z;

	//�J�����̈ړ���
	pCameraInfo->move.x = pCameraInfo->posRDest.x - pCameraInfo->posR.x;
	pCameraInfo->move.z = pCameraInfo->posRDest.z - pCameraInfo->posR.z;

	//�ʒu�Ɉړ��ʂ�ۑ�
	pCameraInfo->posR.x += pCameraInfo->move.x;
	pCameraInfo->posR.z += pCameraInfo->move.z;
}

//=============================================================================
// �G�f�B�b�g�p�̃J����
//=============================================================================
//================================================================
// �R���X�g���N�^
//================================================================
EditCamera::EditCamera()
{

}

//================================================================
// �f�X�g���N�^
//================================================================
EditCamera::~EditCamera()
{

}

//================================================================
// �X�V
//================================================================
void EditCamera::Update(CCamera* pCamera)
{
	CInputKeyboard *pInput = CManager::GetInstance()->GetKeyBoard();

	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	D3DXVECTOR2 MousePos = pInputMouse->GetMouseMove();

	// �J�����̏��擾
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	if (pInputMouse->GetWheel() == true)
	{
		pCameraInfo->posV.x += MousePos.x;
		pCameraInfo->posR.x += MousePos.x;
		pCameraInfo->posV.z -= MousePos.y;
		pCameraInfo->posR.z -= MousePos.y;
	}

	if (pInputMouse->GetLButton() == true)
	{
		pCameraInfo->rot.x += MousePos.y * 0.005f;
		pCameraInfo->rot.y += MousePos.x * 0.005f;
		//pCameraInfo->rot.y += MousePos.x * 0.005f;
	}
		

	if (pInput->GetPress(DIK_J) == true)
	{
		pCameraInfo->posV.y += 1.0f;
		pCameraInfo->posR.y += 1.0f;
	}

	if (pInput->GetPress(DIK_K) == true)
	{
		pCameraInfo->posV.y -= 1.0f;
		pCameraInfo->posR.y -= 1.0f;
	}

	pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

}