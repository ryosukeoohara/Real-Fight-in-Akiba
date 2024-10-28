//===========================================================
//
// �G�̏��� [enemy.cpp]
// Author : �匴�@�叫
//
//===========================================================
#include "enemy.h"
#include "enemy_weak.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "motion.h"
#include "sound.h"
#include "player.h"
#include "game.h"
#include "debugproc.h"
#include "score.h"
#include "player.h"
#include "character.h"
#include "collision.h"
#include "enemymanager.h"
#include "utility.h"
#include "gage.h"
#include "camera.h"
#include "particle.h"
#include "item.h"
#include "animation.h"
#include "MyEffekseer.h"

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const int DAMAGECOUNT = 20;             // �_���[�W���
	const int ATTACKAGAINCOUNT = 60;   // �čU���ł���܂ł̎���
	const float SPEED = 2.0f;          // ���鑬��
	const float ATTACKLENGHT = 50.0f;  // �U���\�͈�
	const float SEARCHRANGE = 400.0f;  // �T���͈�
	const int RECOVER_DAMAGE_TIME = 15;  // �_���[�W��Ԃł��鎞��
	const char* TEXT_NAME = "data\\TEXT\\motion_enemy.txt";  // �e�L�X�g�t�@�C���̖��O
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CEnemyWeak::CEnemyWeak()
{
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;

	
	m_Mobility = Immobile;
	m_nDamageCounter = 0;
	/*m_pCurrent = nullptr;
	m_pNext = nullptr;
	m_pLife2D = nullptr;*/
	m_pLife3D = nullptr;
	//m_bDeath = false;
	m_bDamage = false;
	m_bStagger = false;
	ChangeState(new CEnemyWeakStateMoveWait);
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CEnemyWeak::CEnemyWeak(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority) : CEnemy(pos, rot, nlife, nPriority)
{
	// �l���N���A
	//SetState(CEnemy::STATE_NONE);
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;
	m_Mobility = Immobile;
	m_nDamageCounter = 0;
	m_pLife3D = nullptr;
	m_bDamage = false;
	m_bStagger = false;
	ChangeState(new CEnemyWeakStateMoveWait);
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CEnemyWeak::~CEnemyWeak()
{

}

//===========================================================
// ��������
//===========================================================
CEnemyWeak * CEnemyWeak::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
{
	CEnemyWeak *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CEnemyWeak(pos, rot, nlife, nPriority);

		pEnemy->Init();

		// �G�̏��擾
		CEnemy::INFO* Info = pEnemy->GetInfo();

		Info->pos = pos;
		Info->rot = rot;
		Info->nLife = nlife;
	}

	return pEnemy;
}

//===========================================================
// ����������
//===========================================================
HRESULT CEnemyWeak::Init(void)
{
	CEnemy::Init();
	SetType(WEAK);

	// �G�̏��擾
	CEnemy::INFO *Info = GetInfo();

	ReadText(TEXT_NAME);

	if (m_pLife3D == nullptr)
	{
		m_pLife3D = CGage3D::Create(D3DXVECTOR3(Info->pos.x, Info->pos.y, Info->pos.z), 5.0f, (float)((Info->nLife * 0.01f) * 20), CGage3D::TYPE_LIFE);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		m_pLife3D->SetPos(D3DXVECTOR3(mtx->_41, mtx->_42, mtx->_43));
		m_pLife3D->SetUpHeight(90.0f);
		m_pLife3D->GetBill()->SetEdgeCenter((float)((Info->nLife * 0.01f) * 20), 5.0f);
	}
	
	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CEnemyWeak::Uninit(void)
{
	CEnemy::Uninit();

	if (m_pLife3D != nullptr)
	{
		m_pLife3D->Uninit();
		m_pLife3D = nullptr;
	}

	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	CObject::Release();
}

//===========================================================
// �X�V����
//===========================================================
void CEnemyWeak::Update(void)
{
	// �G�̏��擾
	CEnemy::INFO *Info = GetInfo();

	if (Info == nullptr)
		return;

	// ���[�V�������擾
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
		return;

	CEnemy::Update();

	if (m_pLife3D != nullptr)
	{
		m_pLife3D->GetBill()->SetEdgeCenter((float)((Info->nLife * 0.01f) * 20), 5.0f);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		m_pLife3D->SetPos(D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43));
	}

	if (GetMobility() == Immobile)
		return;

	// �X�e�C�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (Info->state == STATE_DEATH)
		return;

	CManager::GetInstance()->GetDebugProc()->Print("�G�̈ʒu�F[%f, %f, %f]\n", Info->pos.x, Info->pos.y, Info->pos.z);
}

//===========================================================
// �`�揈��
//===========================================================
void CEnemyWeak::Draw(void)
{
	CEnemy::Draw();
}

//===========================================================
// �X�e�C�g�̐؂�ւ�
//===========================================================
void CEnemyWeak::ChangeState(CEnemyWeakState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//================================================================
// �����Ă���Ƃ��̃G�t�F�N�g
//================================================================
void CEnemyWeak::DashEffect(void)
{
	D3DMATRIX* mtx = {};

	int nNow = GetMotion()->GetKeyFrame();

	if (nNow == 1)
	{
		mtx = GetCharcter()[12]->GetMtxWorld();

		CParticle::Create(D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43), CParticle::TYPE_GROUND);
	}

	if (nNow % 20 == 0)
	{
		mtx = GetCharcter()[15]->GetMtxWorld();

		CParticle::Create(D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43), CParticle::TYPE_GROUND);
	}
}

//===========================================================
// �_���[�W����
//===========================================================
void CEnemyWeak::Damage(void)
{
	// ���łɃ_���[�W���
	if (m_bDamage || m_bHeatDamage)
		return;

	// �G�̏��擾
	CEnemy::INFO* Info = GetInfo();

	if (Info == nullptr)
		return;

	if (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_NONE)
	{
		GetMotion()->Set(MOTION_DAMAGE);
		m_bDamage = true;
	}
	else if (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH)
	{
		m_bHeatDamage = true;
	}

	Info->nLife -= CPlayer::GetInstance()->GetMotion()->GetAttackDamage();
	
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_HIT, false, D3DXVECTOR3(Info->pos.x, Info->pos.y + 50.0f, Info->pos.z));

	if (Info->nLife > 0 && CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_NONE)
	{// �̗͂��T�O�ȏ�̂Ƃ�

		// �_���[�W��Ԃɐ؂�ւ���
		ChangeState(new CEnemyWeakStateDamage);
	}
	if (Info->nLife > 0 && (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH))
 	{// �̗͂��T�O�ȏ�̂Ƃ�

		// �_���[�W��Ԃɐ؂�ւ���
		ChangeState(new CEnemyWeakStateHeavyDamage);
	}
	else if (Info->nLife <= 0)
	{// �̗͂��O�ȉ��̂Ƃ�

		// ���S��Ԃɐ؂�ւ���
		ChangeState(new CEnemyWeakStateDeath);

		m_bDeath = true;
	}
}

//===========================================================
// �߂܂ꂽ�Ƃ��̏���
//===========================================================
void CEnemyWeak::Grabbed(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	// ���S�t���O�������Ă���Ƃ�
	if (m_bDeath)
		return;

	if (pPlayer->GetbGrap() == true)
		ChangeState(new CEnemyWeakStateGrabbed);

	else if (pPlayer->GetbGrap() == false)
	{
		ChangeState(new CEnemyWeakStateMoveWait);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		GetInfo()->pos = D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43);
		m_bDamage = false;

		// �G�̏��擾
		CEnemy::INFO* Info = GetInfo();
		D3DXVECTOR3 ForwardVector = utility::CalculateDirection(Info->pos, pPlayer->GetPosition());
		float angle = atan2f(ForwardVector.x, ForwardVector.z);
		angle -= D3DX_PI;
		angle = utility::CorrectAngle(angle);
		Info->rot.y = angle;
	}
		
}

//===========================================================
// �q�[�g�A�N�V�����F���]�Ԏ󂯑҂�
//===========================================================
void CEnemyWeak::Denial(void)
{
	// �X�e�[�g�̐؂�ւ�
	ChangeState(new CEnemyWeakStateDenial);
}

//======================================================================
// �X�e�C�g
//======================================================================
//===========================================================
// �s���҂���Ԃ̏���
//===========================================================
CEnemyWeakStateMoveWait::CEnemyWeakStateMoveWait()
{

}

void CEnemyWeakStateMoveWait::Update(CEnemyWeak* pEnemyWeak)
{
	// �����蔻��̏��擾
	CCollision* pCollision = CCollision::GetInstance();

	if (pCollision == nullptr)
		return;

	// �v���C���[�̏��擾
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// ���[�V�������擾
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// �G�̏��擾
	CEnemy::INFO *Info = pEnemyWeak->GetInfo();

	if (/*m_Chase == CHASE_ON && */CGame::GetCollision()->Circle(Info->pos, pPlayer->GetPosition(), SEARCHRANGE, pPlayer->GetRadius()) == true)
	{//�~�̒��Ƀv���C���[��������

		Info->state = pEnemyWeak->STATE_DASH;
		pMotion->Set(pEnemyWeak->MOTION_DASH);
		pEnemyWeak->ChangeState(new CEnemyWeakStateMove);
	}
	else
	{// �v���C���[���~�̒��ɂ��Ȃ�

		Info->move.x = 0.0f;
		Info->move.z = 0.0f;

		// �ҋ@��Ԃɂ���
		if (Info->state != pEnemyWeak->STATE_NEUTRAL)
		{
			Info->state = pEnemyWeak->STATE_NEUTRAL;
			pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		}
	}
}

//===========================================================
// �U���҂���Ԃ̏���
//===========================================================
CEnemyWeakStateAttackWait::CEnemyWeakStateAttackWait()
{

}

void CEnemyWeakStateAttackWait::Update(CEnemyWeak* pEnemyWeak)
{
	// ���[�V�������擾
	CMotion* pMotion = pEnemyWeak->GetMotion();

	// �G�̏��擾
	CEnemy::INFO *Info = pEnemyWeak->GetInfo();

	m_nAtcCounter++;

	if (m_nAtcCounter >= ATTACKAGAINCOUNT)
	{
		m_nAtcCounter = 0;

		// �U����Ԃɂ���
		Info->state = pEnemyWeak->STATE_ATTACK;
		pMotion->Set(pEnemyWeak->MOTION_ATTACK);
		pEnemyWeak->ChangeState(new CEnemyWeakStateAttack);
	}
}

//===========================================================
// �U����Ԃ̏���
//===========================================================
CEnemyWeakStateAttack::CEnemyWeakStateAttack()
{

}

void CEnemyWeakStateAttack::Update(CEnemyWeak* pEnemyWeak)
{
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �G�̏��擾
	CEnemy::INFO *Info = pEnemyWeak->GetInfo();

	pEnemyWeak->HitDetection(Info->pos, ATTACKLENGHT, pPlayer->GetRadius());

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		// �v���C���[�Ƃ̋���
		float fLenght = utility::Distance(Info->pos, PlayerPos);

		Info->state = pEnemyWeak->STATE_NEUTRAL;
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		
		pEnemyWeak->ChangeState(new CEnemyWeakStateMoveWait);
	}
}

//===========================================================
// �ړ���Ԃ̏���
//===========================================================
CEnemyWeakStateMove::CEnemyWeakStateMove()
{
}

void CEnemyWeakStateMove::Update(CEnemyWeak* pEnemyWeak)
{
	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	// ���[�V�������擾
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// �v���C���[�̏��擾
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �ʒu�ۑ�
	Info->posOld = Info->pos;

	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	float fDiffmove = 0.0f;

	D3DXVECTOR3 ForwardVector = utility::CalculateDirection(Info->pos, PlayerPos);

	float angle = atan2f(ForwardVector.x, ForwardVector.z);

	angle -= D3DX_PI;

	angle = utility::CorrectAngle(angle);

	Info->rot.y = angle;

	Info->move = ForwardVector * SPEED;

	Info->pos += Info->move;

	// �����Ă���Ƃ��̃G�t�F�N�g
	pEnemyWeak->DashEffect();

	// �v���C���[�Ƃ̋���
	float fLenght = utility::Distance(Info->pos, PlayerPos);

	if (fLenght <= ATTACKLENGHT)
	{// �U���\�͈͓��ɓ�����

		Info->state = pEnemyWeak->STATE_NEUTRAL;
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		pEnemyWeak->ChangeState(new CEnemyWeakStateAttackWait);
	}

	// �G�Ƃ̓����蔻��
	CEnemy* pEnemy = CEnemy::GetTop();

	// �����蔻��擾
	CCollision* pCollision = CGame::GetCollision();

	while (pEnemy != nullptr)
	{
		CEnemy* pEnemyNext = pEnemy->GetNext();

		if (Info->nIdxID != pEnemy->GetIdxID() && utility::CheckCirclePushOut(&Info->pos, &pEnemy->GetPosition(), 20.0f, ForwardVector))
		{
			pEnemyWeak->ChangeState(new CEnemyWeakStateAttackWait);
		}
			
		pEnemy = pEnemyNext;
	}
}

//===========================================================
// �_���[�W��Ԃ̏���
//===========================================================
CEnemyWeakStateDamage::CEnemyWeakStateDamage()
{
	m_nRecoverDamageTime = DAMAGECOUNT;
}

void CEnemyWeakStateDamage::Update(CEnemyWeak* pEnemyWeak)
{
	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	if (Info == nullptr)
		return;

	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	m_nRecoverDamageTime--;

	if (m_nRecoverDamageTime <= 0)
		pEnemyWeak->SetbDamage();

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		pEnemyWeak->ChangeState(new CEnemyWeakStateMoveWait);
		pEnemyWeak->SetbDamage();
	}
}

//================================================================
// ���ۏ�Ԃ̏���
//================================================================
//===========================================================
// ���ۏ�Ԃ̃R���X�g���N�^
//===========================================================
CEnemyWeakStateDenial::CEnemyWeakStateDenial()
{

}

//===========================================================
// ���ۏ�Ԃ̍X�V����
//===========================================================
void CEnemyWeakStateDenial::Update(CEnemyWeak* pEnemyWeak)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V���������ۈȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemyWeak->MOTION_DENIAL)
		pMotion->Set(pEnemyWeak->MOTION_DENIAL);

}

//===========================================================
// �����U�����󂯂���Ԃ̏���
//===========================================================
CEnemyWeakStateHeavyDamage::CEnemyWeakStateHeavyDamage()
{

}

//===========================================================
// �����U�����󂯂���Ԃ̍X�V����
//===========================================================
void CEnemyWeakStateHeavyDamage::Update(CEnemyWeak* pEnemyWeak)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V�������m�b�N�_�E���ȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemyWeak->MOTION_FALLDOWN)
		pMotion->Set(pEnemyWeak->MOTION_FALLDOWN);

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		pEnemyWeak->ChangeState(new CEnemyWeakStateGetUp);

		// �q�[�g�A�N�V��������������t���O��܂�
		pEnemyWeak->RestHeatDamageFrag();
	}
}

//===========================================================
// ���S��Ԃ̏���
//===========================================================
CEnemyWeakStateDeath::CEnemyWeakStateDeath()
{
}

void CEnemyWeakStateDeath::Update(CEnemyWeak* pEnemyWeak)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V�������m�b�N�_�E���ȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemyWeak->MOTION_DEATH)
		pMotion->Set(pEnemyWeak->MOTION_DEATH);

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		// �G�̑��������炷
		int nNum = CEnemyManager::GetNum() - 1;
		CEnemyManager::SetNum(nNum);
		pEnemyWeak->Uninit();

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{// �Q�[���̎�

			// �G��|�������𑝂₷
			int nDefeat = CManager::GetInstance()->GetDefeat();
			nDefeat++;
			CManager::GetInstance()->SetDefeat(nDefeat);
		}
	}
}

CEnemyWeakStateGetUp::CEnemyWeakStateGetUp()
{
}

void CEnemyWeakStateGetUp::Update(CEnemyWeak* pEnemyWeak)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	if (Info == nullptr)
		return;

	// ���[�V�������N���オ��ȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemyWeak->MOTION_GETUP)
		pMotion->Set(pEnemyWeak->MOTION_GETUP);

	else if (pMotion->IsFinish())
	{// ���[�V�������I�����Ă���

		// �ړ���Ԃɐ؂�ւ���
		pEnemyWeak->ChangeState(new CEnemyWeakStateMoveWait);
		pEnemyWeak->SetbDamage();
	}
}

CEnemyWeakStateGrabbed::CEnemyWeakStateGrabbed()
{

}

void CEnemyWeakStateGrabbed::Update(CEnemyWeak* pEnemyWeak)
{

}