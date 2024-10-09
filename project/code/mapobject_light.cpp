//===========================================================
//
// �}�b�v�ɔz�u��Ă���O������[mapobject_light.cpp]
// Author �匴�叫
//
//===========================================================
#include "mapobject_light.h"
#include "mapobject_brokenlight.h"
#include "player.h"
#include "collision.h"
#include "particle.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CMapObject_Light* CMapObject_Light::m_pTop = nullptr;      // �擪�̃|�C���^
CMapObject_Light* CMapObject_Light::m_pCur = nullptr;      // �Ō���̃|�C���^

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
CMapObject_Light::CMapObject_Light()
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
CMapObject_Light::CMapObject_Light(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
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
CMapObject_Light::~CMapObject_Light()
{

}

//===========================================================
// ����������
//===========================================================
HRESULT CMapObject_Light::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject_Light::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// �X�V����
//===========================================================
void CMapObject_Light::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �X�e�[�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	// �v���C���[���U�������Ă��āA���̍U�����������Ă�����
	if (pPlayer->GetState() == CPlayer::STATE_ATTACK && pPlayer->IsHitCollision() && !m_bLightFall)
	{
		// �U�����������Ă�����
		if (CCollision::GetInstance()->HitOBJ(pPlayer, this, 30.0f, 30.0f))
		{
			// �X�e�[�g�̕ύX�F����ꂽ���
			ChangeState(new CLightBeaten);
		}
	}

	CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f);
}

//===========================================================
// �`�揈��
//===========================================================
void CMapObject_Light::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// ��������
//===========================================================
CMapObject_Light* CMapObject_Light::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_Light* pTrashBox = new CMapObject_Light(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// �X�e�[�g�̕ύX����
//===========================================================
void CMapObject_Light::ChangeState(CLightState* pState)
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
// ����ꂽ��Ԃ̏���
//===========================================================
CLightBeaten::CLightBeaten()
{
	m_nShakeTimeCounter = 0;
	m_nFallDownCount = 0;
	m_fShakeAngle = 0.0f;
}

//===========================================================
// ����ꂽ��Ԃ̍X�V����
//===========================================================
void CLightBeaten::Update(CMapObject_Light* pLighth)
{
	CObjectX::INFO* pInfo = pLighth->GetInfo();

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
		int nNum = pLighth->GetFallDownCount();
		nNum++;
		pLighth->SetFallDown(nNum);

		// �X�e�[�g�̕ύX�F�j���[�g����
		if (nNum >= FALL_DOWN)
		{
			pLighth->ChangeState(new CLightFall);

			pLighth->SetLightFall(true);
		}
		else
		{
			pLighth->ChangeState(new CLightNeutral);
		}
		
	}

	pInfo->pos += pInfo->move;

	// �ړ��ʂ��X�V(����������)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}

//===========================================================
// ������Ԃ̏���
//===========================================================
CLightFall::CLightFall()
{
	
}

//===========================================================
// ������Ԃ̍X�V����
//===========================================================
void CLightFall::Update(CMapObject_Light* pLighth)
{
	CObjectX::INFO* pInfo = pLighth->GetInfo();

	//if (m_pLightR == nullptr)
	//{
	//	m_pLightR = CObjectX::Create("data\\MODEL\\map\\light_rubble_L.x");
	//	m_pLightR->SetPosition(D3DXVECTOR3(pInfo->pos.x, 400.0f, pInfo->pos.z + 50.0f));
	//	m_pLightR->SetGravity(5.0f);
	//}

	//if (m_pLightL == nullptr)
	//{
	//	m_pLightL = CObjectX::Create("data\\MODEL\\map\\light_rubble_R.x");
	//	m_pLightL->SetPosition(D3DXVECTOR3(pInfo->pos.x, 400.0f, pInfo->pos.z - 50.0f));
	//	m_pLightL->SetGravity(5.0f);
	//}

	//if (m_pLightR != nullptr)
	//{
	//	CObjectX::INFO* pInfo = m_pLightR->GetInfo();
	//	
	//	if (pInfo->pos.y > 0.0f)
	//	{
	//		pInfo->pos.y -= 9.0f;
	//	}
	//	else
	//	{
	//		// �K���X�Ђ̃p�[�e�B�N���𐶐�
	//		CParticle::Create(pInfo->pos, CParticle::TYPE_GLASS);
	//	}
	//		
	//	pInfo->pos += pInfo->move;

	//	// �ړ��ʂ��X�V(����������)
	//	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	//	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	//	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
	//}

	//if (m_pLightL != nullptr)
	//{
	//	CObjectX::INFO* pInfo = m_pLightL->GetInfo();

	//	if (pInfo->pos.y > 0.0f)
	//	{
	//		pInfo->pos.y -= 9.0f;
	//	}
	//	else
	//	{
	//		// �K���X�Ђ̃p�[�e�B�N���𐶐�
	//		CParticle::Create(pInfo->pos, CParticle::TYPE_GLASS);

	//		pLighth->ChangeState(new CLightNeutral);
	//	}

	//	pInfo->pos += pInfo->move;

	//	// �ړ��ʂ��X�V(����������)
	//	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	//	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	//	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
	//}
	CMapObject_BrokenLight *pLightL = CMapObject_BrokenLight::Create("data\\MODEL\\map\\light_rubble_L.x");
	pLightL->SetPosition(D3DXVECTOR3(pInfo->pos.x + 50.0f, 400.0f, pInfo->pos.z));
	pLightL->SetRotition(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pLightL->SetIdxModel(5);
	pLightL->SetVtx();

	CMapObject_BrokenLight* pLightR = CMapObject_BrokenLight::Create("data\\MODEL\\map\\light_rubble_R.x");
	pLightR->SetPosition(D3DXVECTOR3(pInfo->pos.x - 50.0f, 400.0f, pInfo->pos.z));
	pLightR->SetRotition(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pLightL->SetIdxModel(5);
	pLightR->SetVtx();

	pInfo->vtxMax.x -= 50.0f;
	pInfo->vtxMini.x += 10.0f;
	
	pLighth->ChangeState(new CLightNeutral);
}

CLightNeutral::CLightNeutral()
{

}

void CLightNeutral::Update(CMapObject_Light* pLighth)
{

}