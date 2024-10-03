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
#include "utility.h"

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
	const D3DXVECTOR3 ONSTAGE_POSV = { 0.0f, 150.0f, -100.0f };   // �ڕW�̊p�x
	const D3DXVECTOR3 ONSTAGE_POSR = { 0.0f, 50.0f, 0.0f };       // �ڕW�̊p�x
	const float CAMERA_DISTNCE = 200.0f;  // ����
	const float CAMERA_MOVE = 2.0f;       // �ړ���
	const float CAMERA_ROT_Y = 0.03f;     // Y���̉�]��
	const float CORRECT_TO_FACT = 0.05f;  // �␳����l
	const int TIME_TO_CORRECT = 90;       // �␳���鎞��
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
	

	if (m_pState != nullptr)
		m_pState->Update(this);
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

//================================================================
// �R���X�g���N�^
//================================================================
FollowPlayerCamera::FollowPlayerCamera()
{// �Ǐ]����J����

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

//================================================================
// �R���X�g���N�^
//================================================================
FixedCamera::FixedCamera()
{// �Œ�J����

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

//================================================================
// �R���X�g���N�^
//================================================================
CutSceneCamera::CutSceneCamera()
{// �L�����N�^�[�o�ꉉ�o�p�J����

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
		D3DXVECTOR3 posDestR = ONSTAGE_POSR - pCameraInfo->posR;
		pCamera->SetPositionR(pCameraInfo->posR + posDestR * CORRECT_TO_FACT);

		D3DXVECTOR3 posDestV = ONSTAGE_POSV - pCameraInfo->posV;
		pCamera->SetPositionV(pCameraInfo->posV + posDestV * CORRECT_TO_FACT);

		pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
		pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

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

//================================================================
// �R���X�g���N�^
//================================================================
ReturnPlayerBehindCamera::ReturnPlayerBehindCamera()
{// �v���C���[�̔w��ɃJ�������߂��Ă���

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

//================================================================
// �R���X�g���N�^
//================================================================
HeatActionCamera::HeatActionCamera()
{// �q�[�g�A�N�V�����J����

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

//================================================================
// �R���X�g���N�^
//================================================================
EditCamera::EditCamera()
{// �q�[�g�A�N�V�����J����

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
		pCameraInfo->rot.y += MousePos.x * 0.005f;

	pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

}