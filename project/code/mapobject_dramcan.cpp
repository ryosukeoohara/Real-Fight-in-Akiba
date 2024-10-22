//===========================================================
//
// �}�b�v�ɔz�u��Ă���h�����ʏ���[mapobject_dramcan.cpp]
// Author �匴�叫
//
//===========================================================
#include "mapobject_dramcan.h"
#include "mapobject.h"
#include "object3D.h"
#include "collision.h"
#include "player.h"
#include "manager.h"
#include "texture.h"
#include "ripples.h"
#include "manager.h"
#include "sound.h"
#include "utility.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
//CMapObject_Dramcan* CMapObject_Dramcan::m_pDramcan = nullptr;  // �������g
CMapObject_Dramcan* CMapObject_Dramcan::m_pTop = nullptr;      // �擪�̃|�C���^
CMapObject_Dramcan* CMapObject_Dramcan::m_pCur = nullptr;      // �Ō���̃|�C���^

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const char* TEXTURE_PATH = "data\\TEXTURE\\water.png";  // �����܂�̃e�N�X�`��
	const D3DXVECTOR2 TARGET_POS = { 100.0f, 100.0f };        // �ڕW�̒l
	const float LERP_SPEED = 0.0015f;                        // �ڕW�̒l�܂ł̕␳�W��
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CMapObject_Dramcan::CMapObject_Dramcan()
{
	//m_pDramcan = this;
	m_pState = nullptr;

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
CMapObject_Dramcan::CMapObject_Dramcan(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
{
	//m_pDramcan = this;
	m_pState = nullptr;

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
CMapObject_Dramcan::~CMapObject_Dramcan()
{

}

//===========================================================
// ����������
//===========================================================
HRESULT CMapObject_Dramcan::Init(void)
{
	CMapObject::Init();

	m_pState = new CDramcanNeutral;

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject_Dramcan::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// �X�V����
//===========================================================
void CMapObject_Dramcan::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �v���C���[���U�������Ă��āA���̍U�����������Ă�����
	if (pPlayer->GetState() == CPlayer::STATE_ATTACK && pPlayer->IsHitCollision())
	{
		// �U�����������Ă�����
		if (CCollision::GetInstance()->AttackHitCheck(pPlayer, this, 30.0f, 30.0f))
		{
			// �X�e�[�g�̕ύX�F����ꂽ���
			ChangeState(new CDramcanBlowAway);

			CSound* pSound = CManager::GetInstance()->GetSound();

			if (pSound != nullptr)
				pSound->Play(CSound::SOUND_LABEL_SE_IRON_ROT_HIT);
		}
	}

	//CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f);

	// �X�e�[�g�̕ύX
	if (m_pState != nullptr)
		m_pState->Update(this);
}

//===========================================================
// �`�揈��
//===========================================================
void CMapObject_Dramcan::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// ��������
//===========================================================
CMapObject_Dramcan* CMapObject_Dramcan::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_Dramcan* pTrashBox = new CMapObject_Dramcan(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// �X�e�C�g�̕ύX����
//===========================================================
void CMapObject_Dramcan::ChangeState(CDramcanState* pState)
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
CDramcanNeutral::CDramcanNeutral()
{

}

//===========================================================
// �j���[�g������Ԃ̍X�V����
//===========================================================
void CDramcanNeutral::Update(CMapObject_Dramcan* pDramcan)
{

}

//===========================================================
// ������ԏ�Ԃ̏���
//===========================================================
CDramcanBlowAway::CDramcanBlowAway()
{

}

//===========================================================
// ������ԏ�Ԃ̍X�V����
//===========================================================
void CDramcanBlowAway::Update(CMapObject_Dramcan* pDramcan)
{
	D3DXVECTOR3 Playerrot = CPlayer::GetInstance()->GetRotition();
	D3DXVECTOR3 Playermove = CPlayer::GetInstance()->GetMove();

	CObjectX::INFO* pInfo = pDramcan->GetInfo();

	pInfo->rot.x = 1.57f;
	pInfo->rot.y = Playerrot.y;
	pInfo->rot.z = 1.57f;

	pInfo->move.x += (Playermove.x * 1.2f);
	pInfo->move.y += 1.0f;
	pInfo->move.z += (Playermove.z * 1.2f);

	if (pInfo->pos.y - pInfo->vtxMax.x >= 0.0f)
	{
		pInfo->pos.y -= 1.0f;
		pDramcan->ChangeState(new CDramcanWaterLeak);
	}
		
	if (pInfo->pos.y <= 0.0f)
	{
		pInfo->pos.y = 1.0f;
		pDramcan->ChangeState(new CDramcanWaterLeak);
	}

	pInfo->pos += pInfo->move;

	// �ړ��ʂ��X�V(����������)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}

//===========================================================
// �����R����Ԃ̏���
//===========================================================
CDramcanWaterLeak::CDramcanWaterLeak()
{

}

//===========================================================
// �����R����Ԃ̍X�V����
//===========================================================
void CDramcanWaterLeak::Update(CMapObject_Dramcan* pDramcan)
{
	CObjectX::INFO* pInfo = pDramcan->GetInfo();

	// �����܂�𐶐�
	if (m_pPuddle == nullptr)
	{
		m_pPuddle = CObject3D::Create();
		m_pPuddle->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(TEXTURE_PATH));
		m_pPuddle->SetSize(1.0f, 1.0f);
		m_pPuddle->SetColor({ 1.0f, 1.0f, 1.0f, 0.8f });
		m_pPuddle->SetDraw(true);
		m_pPuddle->SetPosition(D3DXVECTOR3(pInfo->pos.x, 0.5f, pInfo->pos.z));
	}

	// �ڕW�̒l�܂ŕ␳
	if (m_pPuddle != nullptr)
		m_pPuddle->EaseToTarget(TARGET_POS, LERP_SPEED);
		
	// �v���C���[�̏��擾
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �v���C���[�̈ʒu�擾
	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	// �v���C���[���͈͓��ɓ������Ƃ��A�g��G�t�F�N�g�𐶐�����
	if (CCollision::GetInstance()->Circle(m_pPuddle->GetPosition(), PlayerPos, 75.0f, 50.0f))
		pPlayer->CreateRippleshEffect();

}