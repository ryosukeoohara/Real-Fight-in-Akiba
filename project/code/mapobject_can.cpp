//===========================================================
//
// �}�b�v�ɔz�u��Ă���ʏ���[mapobject_can.cpp]
// Author �匴�叫
//
//===========================================================
#include "mapobject_can.h"
#include "player.h"
#include "collision.h"
#include "utility.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CMapObject_Can* CMapObject_Can::m_pInstance = nullptr;
CMapObject_Can* CMapObject_Can::m_pTop = nullptr;
CMapObject_Can* CMapObject_Can::m_pCur = nullptr;

//===========================================================
// �R���X�g���N�^
//===========================================================
CMapObject_Can::CMapObject_Can()
{
	m_pInstance = this;

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
CMapObject_Can::CMapObject_Can(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
{
	m_pInstance = this;

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
CMapObject_Can::~CMapObject_Can()
{
}

//===========================================================
// ����������
//===========================================================
HRESULT CMapObject_Can::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject_Can::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// �X�V����
//===========================================================
void CMapObject_Can::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �v���C���[�Ƃ̓����蔻��F�v���C���[���͈͓��ɓ�������
	if (CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f) && !m_b)
	{
		ChangeState(new CCanBlowAway);
		m_b = true;
	}
		

	// �X�e�[�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);
}

//===========================================================
// �`�揈��
//===========================================================
void CMapObject_Can::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// �X�e�[�g�̕ύX����
//===========================================================
void CMapObject_Can::ChangeState(CCanState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//===========================================================
// ��������
//===========================================================
CMapObject_Can* CMapObject_Can::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_Can* pCan = new CMapObject_Can(aModelFliename, nPriority);

	if (pCan != nullptr)
	{
		pCan->Init();
	}

	return pCan;
}

//=======================================================================
// �X�e�[�g
//=======================================================================
//===========================================================
// ������ԏ��
//===========================================================
CCanBlowAway::CCanBlowAway()
{
	
}

//===========================================================
// ������ԏ�Ԃ̍X�V����
//===========================================================
void CCanBlowAway::Update(CMapObject_Can* pCan)
{
	D3DXVECTOR3 Playerrot = CPlayer::GetInstance()->GetRotition();
	D3DXVECTOR3 Playermove = CPlayer::GetInstance()->GetMove();
	
	CObjectX::INFO *pInfo = pCan->GetInfo();

	pInfo->rot.x = D3DX_PI * 0.5f;
	pInfo->rot.y = Playerrot.y;
	pInfo->rot.z = D3DX_PI * 0.5f;

	pInfo->move.x += (Playermove.x * 1.2f);
	pInfo->move.y += 1.0f;
	pInfo->move.z += (Playermove.z * 1.2f);

	if (pInfo->pos.y - pInfo->vtxMax.x > 0.0f)
	{
		pInfo->move.y -= 1.0f;
		pCan->ChangeState(new CCanBound);
	}

	if (pInfo->pos.y < 0.0f)
	{
		pInfo->pos.y = 0.0f;
		pInfo->move.y = 0.0f;
		pCan->ChangeState(new CCanBound);
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
CCanBound::CCanBound()
{
	m_fTime = 1.0f;
}

//===========================================================
// �o�E���h��Ԃ̍X�V����
//===========================================================
void CCanBound::Update(CMapObject_Can* pCan)
{
	CObjectX::INFO* pInfo = pCan->GetInfo();

	//if (pInfo->pos.y > 0.0f)
	//{
	//	pInfo->pos.y -= 1.0f;
	//}

	//if (pInfo->pos.y < 0.0f)
	//{
	//	pInfo->pos.y = 0.0f;
	//}

	pInfo->pos += pInfo->move;

	// �ړ��ʂ��X�V(����������)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;

	pInfo->pos.y = easing::EaseInBounce(m_fTime);

	if (m_fTime > 0.0f)
		m_fTime -= 0.1f;
}

//===========================================================
// ������ꂽ��Ԃ̏���
//===========================================================
CCanThrow::CCanThrow()
{

}

//===========================================================
// ������ꂽ��Ԃ̍X�V����
//===========================================================
void CCanThrow::Update(CMapObject_Can* pCan)
{
	CObjectX::INFO* pInfo = pCan->GetInfo();

	pInfo->move.x -= sinf(pInfo->rot.y);
	pInfo->move.y -= 1.0f;
	pInfo->move.z -= cosf(pInfo->rot.y);

	pInfo->pos += pInfo->move;

	if (pInfo->pos.y <= 0.0f)
		pCan->ChangeState(new CCanBound);

	// �ړ��ʂ��X�V(����������)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}