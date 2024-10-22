//===========================================================
//
// ����Ō��Ă�l[audience.cpp]
// Author �匴�叫
//
//===========================================================
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "audience.h"
#include "objectX.h"
#include "debugproc.h"
#include "object.h"
#include "object3D.h"
#include "collision.h"
#include "player.h"
#include "mapobject_can.h"
#include "utility.h"

//================================================================
// �ÓI�����o�ϐ�
//================================================================
CAudience* CAudience::m_pTop = nullptr;
CAudience* CAudience::m_pCur = nullptr;

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const int JUMP_WAIT_TIME = 60;  // �ĂуW�����v�ł���悤�ɂȂ�܂ł̎���
	const int THROW_WAIT_TIME = 60;  // �Ăъʂ𓊂�����悤�ɂȂ�܂ł̎���
	const float JUMP = 8.0f;      // �W�����v�̍���
	const float GRAVITY = -1.0f;  // �d��
}

//================================================================
// �R���X�g���N�^
//================================================================
CAudience::CAudience()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nJumpWaitTime = 0;
	m_nThrowWaitTime = 0;
	m_bJump = false;
	m_bThrow = false;

	if (m_pTop != nullptr)
	{// �擪�����݂��Ă���ꍇ

		m_pCur->m_pNext = this;
		m_pPrev = m_pCur;
		m_pCur = this;
	}
	else
	{// ���݂��Ȃ��ꍇ

		m_pTop = this;
		m_pCur = this;
	}
}

//================================================================
// �R���X�g���N�^(�I�[�o�[���[�h)
//================================================================
CAudience::CAudience(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filename) : CObjectX(filename)
{
	SetPosition(pos);
	SetRotition(rot);

	m_pos = pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nJumpWaitTime = 0;
	m_bJump = false;

	if (m_pTop != nullptr)
	{// �擪�����݂��Ă���ꍇ

		m_pCur->m_pNext = this;
		m_pPrev = m_pCur;
		m_pCur = this;
	}
	else
	{// ���݂��Ȃ��ꍇ

		m_pTop = this;
		m_pCur = this;
	}
}

//================================================================
// �f�X�g���N�^
//================================================================
CAudience::~CAudience()
{

}

//================================================================
// ��������
//================================================================
CAudience *CAudience::Create(void)
{
	//�I�u�W�F�N�g2D�̃|�C���^
	CAudience *pAudience = nullptr;

	if (pAudience == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pAudience = new CAudience();

		//����������
		pAudience->Init();
	}
	
	return pAudience;
}

//================================================================
// ��������
//================================================================
CAudience* CAudience::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filename)
{
	//�I�u�W�F�N�g2D�̃|�C���^
	CAudience* pAudience = nullptr;

	if (pAudience == nullptr)
	{
		// ����
		pAudience = new CAudience(pos, rot, filename);
		pAudience->m_pos = pos;

		// ����������
		pAudience->Init();
	}
	
	return pAudience;
}

//================================================================
// ����������
//================================================================
HRESULT CAudience::Init(void)
{
	CObjectX::Init();

	// �ۉe�̐���
	CObject3D* pShadow = CObject3D::Create();

	if (pShadow != nullptr)
	{
		pShadow->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg"));
		pShadow->SetSize(20.0f, 20.0f);
		pShadow->SetTexPosition(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
		pShadow->SetPosition(D3DXVECTOR3(m_pos.x, 1.0f, m_pos.z));
		pShadow->SetbSubBlend(true);
		pShadow->SetDraw(true);
	}
	
	return S_OK;
}

//================================================================
// �I������
//================================================================
void CAudience::Uninit(void)
{
	CAudience* pObject = m_pTop;

	while (pObject != nullptr)
	{
		CAudience* pObjectNext = pObject->m_pNext;

		CObjectX::Uninit();
		pObject = nullptr;

		pObject = pObjectNext;
	}
}

//================================================================
// �X�V����
//================================================================
void CAudience::Update(void)
{
	CObjectX::Update();

	// �d�͂�^����
	m_move.y += GRAVITY;

	if (m_bJump == false)
	{// �W�����v���Ă��Ȃ��Ƃ�

		int i = rand() % 3;
		
		if (i == 1)
		{
			float fjump = (rand() % 31 - 10) * 0.1f;
			// �W�����v�����čĂуW�����v�ł���܂ł̎��Ԃ�ݒ�
			m_bJump = true;
			m_move.y = JUMP + fjump;
			m_nJumpWaitTime = JUMP_WAIT_TIME;
		}
	}

	m_pos += m_move;
	
	// �n�ʂɂ߂荞�܂Ȃ��悤��
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
	}

	// �ĂуW�����v�ł���悤�ɂȂ�܂ŃJ�E���g
	if (m_bJump == true)
	{
		m_nJumpWaitTime--;
	}

	// 0�ɂȂ�����ĂуW�����v�ł���悤�ɂ���
	if (m_nJumpWaitTime <= 0)
	{
		m_bJump = false;
	}

	CCollision *pColl = CCollision::GetInstance();

	// �ʒu�ݒ�
	SetPosition(m_pos);

	if (pColl == nullptr)
		return;

	float fDistance = utility::Distance(m_pos, CPlayer::GetInstance()->GetPosition());

	// �v���C���[���͈͓��ɓ��������A�ʂ𓊂��Ă��Ȃ�
	if (fDistance <= 300.0f && !m_bThrow)
	{
		int i = rand() % 7;

		if (i == 1)
		{
			CMapObject_Can* pCan = CMapObject_Can::Create("data\\MODEL\\map\\00_can.x");
			pCan->SetPosition(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z));
			D3DXVECTOR3 rot = GetRotition();
			pCan->SetRotition(rot);
			pCan->ChangeState(new CCanThrow);
		}

		m_bThrow = true;
		m_nThrowWaitTime = THROW_WAIT_TIME;
	}

	// �ʂ𓊂���
	if (m_bThrow)
		m_nThrowWaitTime--;

	// �Ăѓ�������悤�ɂ���
	if (m_nThrowWaitTime <= 0)
		m_bThrow = false;
		
}

//================================================================
// �`�揈��
//================================================================
void CAudience::Draw(void)
{
	CObjectX::Draw();
}