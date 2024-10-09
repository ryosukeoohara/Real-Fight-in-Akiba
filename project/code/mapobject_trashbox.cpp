//===========================================================
//
// �}�b�v�ɔz�u��Ă���S�~������[mapobject_trashbox.cpp]
// Author �匴�叫
//
//===========================================================
#include "mapobject_trashbox.h"
#include "player.h"
#include "collision.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CMapObject_TrashBox* CMapObject_TrashBox::m_pTop = nullptr;      // �擪�̃|�C���^
CMapObject_TrashBox* CMapObject_TrashBox::m_pCur = nullptr;      // �Ō���̃|�C���^

//===========================================================
// �R���X�g���N�^
//===========================================================
CMapObject_TrashBox::CMapObject_TrashBox()
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
CMapObject_TrashBox::CMapObject_TrashBox(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
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
CMapObject_TrashBox::~CMapObject_TrashBox()
{

}

//===========================================================
// ����������
//===========================================================
HRESULT CMapObject_TrashBox::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject_TrashBox::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// �X�V����
//===========================================================
void CMapObject_TrashBox::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �v���C���[�Ƃ̓����蔻��F�v���C���[���͈͓��ɓ�������
	if (CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f))
		ChangeState(new CTrashBoxBlowAway);

	// �X�e�[�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);
}

//===========================================================
// �`�揈��
//===========================================================
void CMapObject_TrashBox::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// ��������
//===========================================================
CMapObject_TrashBox* CMapObject_TrashBox::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_TrashBox* pTrashBox = new CMapObject_TrashBox(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// �X�e�[�g�̕ύX����
//===========================================================
void CMapObject_TrashBox::ChangeState(CTrashBoxState* pState)
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
//===========================================================
// ������я�Ԃ̏���
//===========================================================
CTrashBoxBlowAway::CTrashBoxBlowAway()
{

}

//===========================================================
// ������я�Ԃ̍X�V����
//===========================================================
void CTrashBoxBlowAway::Update(CMapObject_TrashBox* pTrashBox)
{
	D3DXVECTOR3 Playerrot = CPlayer::GetInstance()->GetRotition();
	D3DXVECTOR3 Playermove = CPlayer::GetInstance()->GetMove();

	CObjectX::INFO* pInfo = pTrashBox->GetInfo();

	pInfo->rot.x = 1.57f;
	pInfo->rot.y = Playerrot.y;
	pInfo->rot.z = 1.57f;

	pInfo->move.x += (Playermove.x);
	pInfo->move.y += 1.0f;
	pInfo->move.z += (Playermove.z);

	if (pInfo->pos.y - pInfo->vtxMax.x >= 0.0f)
	{
		pInfo->pos.y -= 1.0f;
		pTrashBox->ChangeState(new CTrashBoxBound);
	}

	if (pInfo->pos.y - pInfo->vtxMax.x <= 0.0f)
	{
		pInfo->pos.y = pInfo->vtxMax.x;
		pTrashBox->ChangeState(new CTrashBoxBound);
	}

	pInfo->pos += pInfo->move;

	// �ړ��ʂ��X�V(����������)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}

//===========================================================
// �o�E���h��Ԃ̏���
//===========================================================
CTrashBoxBound::CTrashBoxBound()
{

}

//===========================================================
// �o�E���h��Ԃ̍X�V����
//===========================================================
void CTrashBoxBound::Update(CMapObject_TrashBox* pTrashBox)
{
	CObjectX::INFO* pInfo = pTrashBox->GetInfo();

	if (pInfo->pos.y > 0.0f)
	{
		pInfo->pos.y -= 1.0f;
	}

	if (pInfo->pos.y < 0.0f)
	{
		pInfo->pos.y = 0.0f;
	}

	pInfo->pos += pInfo->move;

	// �ړ��ʂ��X�V(����������)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}