//===========================================================
//
// �}�b�v�ɔz�u��Ă���W��	����[mapobject_sign.cpp]
// Author �匴�叫
//
//===========================================================
#include "mapobject_sign.h"
#include "player.h"
#include "collision.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CMapObject_Sign* CMapObject_Sign::m_pTop = nullptr;      // �擪�̃|�C���^
CMapObject_Sign* CMapObject_Sign::m_pCur = nullptr;      // �Ō���̃|�C���^

//===========================================================
// �萔��`
//===========================================================
namespace
{
	int FALL_DOWN = 3;  // �R�񉣂�ꂽ��|���
	int SHAKE_TIME = 60;  // �����Ă���h��Ă��鎞��
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CMapObject_Sign::CMapObject_Sign()
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
CMapObject_Sign::CMapObject_Sign(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
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
CMapObject_Sign::~CMapObject_Sign()
{

}

//===========================================================
// ����������
//===========================================================
HRESULT CMapObject_Sign::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject_Sign::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// �X�V����
//===========================================================
void CMapObject_Sign::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �X�e�[�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	// �v���C���[�Ƃ̓����蔻��F�v���C���[���͈͓��ɓ�������
	CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f);

	// �v���C���[���U�������Ă��āA���̍U�����������Ă�����
	if (pPlayer->GetState() == CPlayer::STATE_ATTACK && pPlayer->IsHitCollision())
	{
		// �U�����������Ă�����
		if (CCollision::GetInstance()->AttackHitCheck(pPlayer, this, 30.0f, 30.0f))
		{
			// �X�e�[�g�̕ύX�F����ꂽ���
			ChangeState(new CSignBeaten);
		}
	}
}

//===========================================================
// �`�揈��
//===========================================================
void CMapObject_Sign::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// ��������
//===========================================================
CMapObject_Sign* CMapObject_Sign::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_Sign* pTrashBox = new CMapObject_Sign(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// �X�e�[�g�̕ύX����
//===========================================================
void CMapObject_Sign::ChangeState(CSignState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//=======================================================================
// �X�e�[�g
//=======================================================================
CSignBeaten::CSignBeaten()
{
	m_nShakeTimeCounter = 0;
	m_nFallDownCount = 0;
	m_fShakeAngle = 0.0f;
}

void CSignBeaten::Update(CMapObject_Sign* pElectricBox)
{
	CObjectX::INFO* pInfo = pElectricBox->GetInfo();

	// sin ���g�p�����h�炵���W�̎Z�o
	pInfo->move.y = 0.0f;

	pInfo->move.x = sinf(m_fShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter / SHAKE_TIME)) * 2.0f;

	pInfo->move.z = sinf(m_fShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter / SHAKE_TIME)) * 2.0f;

	// �h�炵�����Ɏg�p���� sin �ɓn���p�x�̕ύX����
	m_fShakeAngle += 2.0f * 1.0f;

	// �h�炷���Ԃ��o�߂�����h�炵�������I������
	m_nShakeTimeCounter += 1;
	if (m_nShakeTimeCounter >= SHAKE_TIME)
	{
		m_nShakeTimeCounter = 0;
		m_nFallDownCount++;

		// �X�e�[�g�̕ύX�F�j���[�g����
		pElectricBox->ChangeState(nullptr);
	}

	pInfo->pos += pInfo->move;

	// �ړ��ʂ��X�V(����������)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}