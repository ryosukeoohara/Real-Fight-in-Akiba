//===========================================================
//
// �}�b�v�ɔz�u��Ă���j�󂳂ꂽ�O������[mapobject_brokenlight.cpp]
// Author �匴�叫
//
//===========================================================
#include "mapobject_brokenlight.h"
#include "player.h"
#include "collision.h"
#include "particle.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CMapObject_BrokenLight* CMapObject_BrokenLight::m_pTop = nullptr;      // �擪�̃|�C���^
CMapObject_BrokenLight* CMapObject_BrokenLight::m_pCur = nullptr;      // �Ō���̃|�C���^

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
CMapObject_BrokenLight::CMapObject_BrokenLight()
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
CMapObject_BrokenLight::CMapObject_BrokenLight(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
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
CMapObject_BrokenLight::~CMapObject_BrokenLight()
{

}

//===========================================================
// ����������
//===========================================================
HRESULT CMapObject_BrokenLight::Init(void)
{
	CMapObject::Init();

	ChangeState(new CBrokenLightFall);

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject_BrokenLight::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// �X�V����
//===========================================================
void CMapObject_BrokenLight::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �X�e�[�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f);
}

//===========================================================
// �`�揈��
//===========================================================
void CMapObject_BrokenLight::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// ��������
//===========================================================
CMapObject_BrokenLight* CMapObject_BrokenLight::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_BrokenLight* pTrashBox = new CMapObject_BrokenLight(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// �X�e�[�g�̕ύX����
//===========================================================
void CMapObject_BrokenLight::ChangeState(CBrokenLightState* pState)
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
// �j���[�g������Ԃ̏���
//===========================================================
CBrokenLightNeutral::CBrokenLightNeutral()
{

}

//===========================================================
// �j���[�g������Ԃ̍X�V����
//===========================================================
void CBrokenLightNeutral::Update(CMapObject_BrokenLight* pLighth)
{

}

//===========================================================
// ������Ԃ̏���
//===========================================================
CBrokenLightFall::CBrokenLightFall()
{

}

//===========================================================
// ������Ԃ̍X�V����
//===========================================================
void CBrokenLightFall::Update(CMapObject_BrokenLight* pLighth)
{
	CObjectX::INFO* pInfo = pLighth->GetInfo();

	if (pInfo->pos.y > 0.0f)
	{
		pInfo->move.y -= 1.5f;
	}
	else
	{
		// �K���X�Ђ̃p�[�e�B�N���𐶐�
		CParticle::Create(pInfo->pos, CParticle::TYPE_GLASS);

		pLighth->ChangeState(new CBrokenLightNeutral);
	}

	pInfo->pos += pInfo->move;

	// �ړ��ʂ��X�V(����������)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}