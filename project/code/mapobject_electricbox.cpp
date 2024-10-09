//===========================================================
//
// �}�b�v�ɔz�u��Ă���S�~������[mapobject_trashbox.cpp]
// Author �匴�叫
//
//===========================================================
#include "mapobject_electricbox.h"
#include "player.h"
#include "collision.h"
#include "utility.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CMapObject_ElectricBox* CMapObject_ElectricBox::m_pTop = nullptr;      // �擪�̃|�C���^
CMapObject_ElectricBox* CMapObject_ElectricBox::m_pCur = nullptr;      // �Ō���̃|�C���^

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
CMapObject_ElectricBox::CMapObject_ElectricBox()
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
CMapObject_ElectricBox::CMapObject_ElectricBox(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
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
CMapObject_ElectricBox::~CMapObject_ElectricBox()
{

}

//===========================================================
// ����������
//===========================================================
HRESULT CMapObject_ElectricBox::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject_ElectricBox::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// �X�V����
//===========================================================
void CMapObject_ElectricBox::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �X�e�[�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	// �v���C���[���U�������Ă��āA���̍U�����������Ă�����
	if (pPlayer->GetState() == CPlayer::STATE_ATTACK && pPlayer->IsHitCollision() && !m_bFallDown)
	{
		// �U�����������Ă�����
		if (CCollision::GetInstance()->HitOBJ(pPlayer, this, 30.0f, 30.0f))
		{
			// �X�e�[�g�̕ύX�F����ꂽ���
			ChangeState(new CElectrictBeaten);
		}
	}

	CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f);
}

//===========================================================
// �`�揈��
//===========================================================
void CMapObject_ElectricBox::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// ��������
//===========================================================
CMapObject_ElectricBox* CMapObject_ElectricBox::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_ElectricBox* pTrashBox = new CMapObject_ElectricBox(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// ��Ԃ̕ύX
//===========================================================
void CMapObject_ElectricBox::ChangeState(CElectricBoxState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//======================================================================
// �X�e�[�g
//======================================================================
//===========================================================
// �j���[�g�������
//===========================================================
CElectricNeutral::CElectricNeutral()
{

}

//===========================================================
// �j���[�g������Ԃ̍X�V����
//===========================================================
void CElectricNeutral::Update(CMapObject_ElectricBox* pElectricBox)
{

}

//===========================================================
// �|��Ă�����
//===========================================================
CElectricFallDown::CElectricFallDown()
{
	PlayerPos = CPlayer::GetInstance()->GetPosition();
}

//===========================================================
// �|��Ă���X�V����
//===========================================================
void CElectricFallDown::Update(CMapObject_ElectricBox* pElectricBox)
{
	CObjectX::INFO* pInfo = pElectricBox->GetInfo();

	if ((pInfo->rot.x >= D3DX_PI * 0.5f || pInfo->rot.x <= -D3DX_PI * 0.5f))
	{
		// �X�e�[�g�̕ύX�F�j���[�g����
		pElectricBox->ChangeState(new CElectricNeutral);
	}

	if (PlayerPos.x >= pInfo->pos.x && PlayerPos.z >= pInfo->pos.z)
	{
		pInfo->rot.x -= 0.1f;
	}

	if (PlayerPos.x >= pInfo->pos.x && PlayerPos.z <= pInfo->pos.z)
	{
		pInfo->rot.x -= 0.1f;
	}

	if (PlayerPos.x <= pInfo->pos.x && PlayerPos.z >= pInfo->pos.z)
	{
		pInfo->rot.x += 0.1f;
	}

	if (PlayerPos.x <= pInfo->pos.x && PlayerPos.z <= pInfo->pos.z)
	{
		pInfo->rot.x += 0.1f;
	}

	// vtx�̕ύX
	utility::ChangeVtx(&pInfo->vtxMax, &pInfo->vtxMini, pInfo->rot);
}

//===========================================================
// ����ꂽ��Ԃ̏���
//===========================================================
CElectrictBeaten::CElectrictBeaten()
{
	m_nShakeTimeCounter = 0;
	m_fShakeAngle = 0.0f;
}

//===========================================================
// ����ꂽ��Ԃ̍X�V����
//===========================================================
void CElectrictBeaten::Update(CMapObject_ElectricBox* pElectricBox)
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
		int nNum = pElectricBox->GetFallDownCount();
		nNum++;
		pElectricBox->SetFallDown(nNum);

		if (nNum >= FALL_DOWN)
		{
			// �X�e�[�g�̕ύX�F�|���
			pElectricBox->ChangeState(new CElectricFallDown);
			pElectricBox->IsFallDown(true);
		}
		else
		{
			// �X�e�[�g�̕ύX�F�j���[�g����
			pElectricBox->ChangeState(new CElectricNeutral);
		}
	}

	pInfo->pos += pInfo->move;

	// �ړ��ʂ��X�V(����������)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}