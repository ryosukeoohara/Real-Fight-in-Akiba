//===========================================================
//
// �}�b�v�ɔz�u��Ă���򏈗�[mapobject_fance.cpp]
// Author �匴�叫
//
//===========================================================
#include "mapobject_fance.h"
#include "player.h"
#include "collision.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CMapObject_Fance* CMapObject_Fance::m_pTop = nullptr;      // �擪�̃|�C���^
CMapObject_Fance* CMapObject_Fance::m_pCur = nullptr;      // �Ō���̃|�C���^

//===========================================================
// �R���X�g���N�^
//===========================================================
CMapObject_Fance::CMapObject_Fance()
{
	if (m_pTop != nullptr)
	{// �擪�����݂��Ă���ꍇ

		m_pCur->m_pNext = this;	// ���ݍŌ���̃I�u�W�F�N�g�̃|�C���^�ɂȂ���
		m_pPrev = m_pCur;
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
	else
	{// ���݂��Ȃ��ꍇ

		m_pTop = this;	// �������g���擪�ɂȂ�
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CMapObject_Fance::CMapObject_Fance(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
{
	if (m_pTop != nullptr)
	{// �擪�����݂��Ă���ꍇ

		m_pCur->m_pNext = this;	// ���ݍŌ���̃I�u�W�F�N�g�̃|�C���^�ɂȂ���
		m_pPrev = m_pCur;
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
	else
	{// ���݂��Ȃ��ꍇ

		m_pTop = this;	// �������g���擪�ɂȂ�
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CMapObject_Fance::~CMapObject_Fance()
{

}

//===========================================================
// ����������
//===========================================================
HRESULT CMapObject_Fance::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject_Fance::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// �X�V����
//===========================================================
void CMapObject_Fance::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �v���C���[�Ƃ̓����蔻��F�v���C���[���͈͓��ɓ�������
	if (CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f))
		ChangeState(new CFanceBlowAway);  // �X�e�[�g�̕ύX�F�������

	// �X�e�[�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);
}

//===========================================================
// �`�揈��
//===========================================================
void CMapObject_Fance::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// ��������
//===========================================================
CMapObject_Fance* CMapObject_Fance::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_Fance* pFance = new CMapObject_Fance(aModelFliename, nPriority);

	if (pFance != nullptr)
		pFance->Init();

	return pFance;
}

//===========================================================
// �X�e�[�g�̕ύX����
//===========================================================
void CMapObject_Fance::ChangeState(CFanceState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//==================================================================
// �X�e�[�g
//==================================================================
//===========================================================
// ������я�Ԃ̏���
//===========================================================
CFanceBlowAway::CFanceBlowAway()
{

}

//===========================================================
// ������я�Ԃ̍X�V����
//===========================================================
void CFanceBlowAway::Update(CMapObject_Fance* pFance)
{
	CObjectX::INFO* pInfo = pFance->GetInfo();

	if (pInfo->rot.x >= 1.3f || pInfo->rot.x <= -1.3f)
	{
		pFance->ChangeState(new CFanceNeutral);

		m_fFallDownSpeed = 0.0f;
	}

	D3DXVECTOR3 move = {};

	move = CPlayer::GetInstance()->GetMove();

	m_fFallDownSpeed = (move.x + move.x) - (move.z + move.z);
	
	if (m_fFallDownSpeed >= 8.0f || m_fFallDownSpeed <= -8.0f)
	{// �v���C���[�̑��x������葬��������

		move = CPlayer::GetInstance()->GetMove();
		pInfo->move.x += move.x;
		pInfo->move.z += move.z;
	}

	pInfo->rot.x -= (m_fFallDownSpeed * 0.01f);
}

//===========================================================
// �j���[�g������Ԃ̏���
//===========================================================
CFanceNeutral::CFanceNeutral()
{

}

//===========================================================
// �j���[�g������Ԃ̍X�V����
//===========================================================
void CFanceNeutral::Update(CMapObject_Fance* pFance)
{

}