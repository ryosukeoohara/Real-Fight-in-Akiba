//===========================================================
//
// �G�̏��� [enemy.cpp]
// Author : �匴�@�叫
//
//===========================================================
#include "enemy.h"
#include "enemy_weak_far.h"
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
#include "bullet.h"
#include "manager.h"
#include "MyEffekseer.h"

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const int DAMAGE = 20;             // �_���[�W���
	const int ATTACKAGAINCOUNT = 120;   // �čU���ł���܂ł̎���
	const float SPEED = 2.0f;          // ���鑬��
	const float ATTACKLENGHT = 200.0f;  // �U���\�͈�
	const float SEARCHRANGE = 400.0f;  // �T���͈�
	const int RECOVER_DAMAGE_TIME = 15;  // �_���[�W��Ԃł��鎞��
	const char* TEXT_NAME = "data\\TEXT\\enemyfar.txt";  // �e�L�X�g�t�@�C���̖��O
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CEnemyWeakFar::CEnemyWeakFar()
{
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;


	m_Mobility = Immobile;
	m_nDamageCounter = 0;
	m_pLife3D = nullptr;
	m_bDamage = false;
	m_bStagger = false;
	ChangeState(new CEnemyWeakFarStateMoveWait);
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CEnemyWeakFar::CEnemyWeakFar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority) : CEnemy(pos, rot, nlife, nPriority)
{
	// �l���N���A
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;
	m_Mobility = Immobile;
	m_nDamageCounter = 0;
	m_pLife3D = nullptr;
	m_bDamage = false;
	m_bStagger = false;
	ChangeState(new CEnemyWeakFarStateMoveWait);
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CEnemyWeakFar::~CEnemyWeakFar()
{

}

//===========================================================
// ��������
//===========================================================
CEnemyWeakFar* CEnemyWeakFar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
{
	CEnemyWeakFar* pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CEnemyWeakFar(pos, rot, nlife, nPriority);

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
HRESULT CEnemyWeakFar::Init(void)
{
	CEnemy::Init();
	SetType(WEAK);

	// �G�̏��擾
	CEnemy::INFO* Info = GetInfo();

	ReadText(TEXT_NAME);

	if (m_pLife3D == nullptr)
	{
		m_pLife3D = CGage3D::Create(D3DXVECTOR3(Info->pos.x, Info->pos.y, Info->pos.z), 5.0f, (float)((Info->nLife * 0.01f) * 20), CGage3D::TYPE_LIFE);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		m_pLife3D->SetPos(D3DXVECTOR3(mtx->_41, mtx->_42, mtx->_43));
		m_pLife3D->SetUpHeight(80.0f);
		m_pLife3D->GetBill()->SetEdgeCenter((float)((Info->nLife * 0.01f) * 20), 5.0f);
	}

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CEnemyWeakFar::Uninit(void)
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
void CEnemyWeakFar::Update(void)
{
	// �G�̏��擾
	CEnemy::INFO* Info = GetInfo();

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

	

	if (Info->nLife <= 0 && Info->state != STATE_DEATH)
	{
		Info->state = CEnemy::STATE_DEATH;
		pMotion->Set(CEnemy::MOTION_DEATH);
		ChangeState(new CEnemyWeakFarStateDeath);
	}

	

	CManager::GetInstance()->GetDebugProc()->Print("�G�̈ʒu�F[%f, %f, %f]\n", Info->pos.x, Info->pos.y, Info->pos.z);
}

//===========================================================
// �`�揈��
//===========================================================
void CEnemyWeakFar::Draw(void)
{
	CEnemy::Draw();
}

//===========================================================
// �X�e�C�g�̐؂�ւ�
//===========================================================
void CEnemyWeakFar::ChangeState(CEnemyWeakFarState* pState)
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
void CEnemyWeakFar::DashEffect(void)
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
void CEnemyWeakFar::Damage(void)
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
		GetMotion()->Set(MOTION_DAMAGE);
		m_bHeatDamage = true;
	}

	Info->nLife -= CPlayer::GetInstance()->GetMotion()->GetAttackDamage();
	
	CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_HIT, ::Effekseer::Vector3D(Info->pos.x, Info->pos.y + 50.0f, Info->pos.z));

	if (Info->nLife > 0 && CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_NONE)
	{// �̗͂��T�O�ȏ�̂Ƃ�

		// �_���[�W��Ԃɐ؂�ւ���
		ChangeState(new CEnemyWeakFarStateDamage);
	}
	if (Info->nLife > 0 && (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH))
	{// �̗͂��T�O�ȏ�̂Ƃ�

		// �_���[�W��Ԃɐ؂�ւ���
		ChangeState(new CEnemyWeakFarStateHeavyDamage);
	}
	else if (Info->nLife <= 0)
	{// �̗͂��O�ȉ��̂Ƃ�

		// ���S��Ԃɐ؂�ւ���
		ChangeState(new CEnemyWeakFarStateDeath);

		m_bDeath = true;
	}
}

//===========================================================
// �߂܂ꂽ���̏���
//===========================================================
void CEnemyWeakFar::Grabbed(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer->GetbGrap() == true)
		ChangeState(new CEnemyWeakFarStateGrabbed);

	else if (pPlayer->GetbGrap() == false)
	{
		ChangeState(new CEnemyWeakFarStateMoveWait);
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
void CEnemyWeakFar::Denial(void)
{
	// �X�e�[�g�̐؂�ւ�
	ChangeState(new CEnemyWeakFarStateDenial);
}

//======================================================================
// �X�e�C�g
//======================================================================
//===========================================================
// �s���҂���Ԃ̏���
//===========================================================
CEnemyWeakFarStateMoveWait::CEnemyWeakFarStateMoveWait()
{

}

//===========================================================
// �s���҂���Ԃ̍X�V����
//===========================================================
void CEnemyWeakFarStateMoveWait::Update(CEnemyWeakFar* pEnemyWeak)
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
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	if (/*m_Chase == CHASE_ON && */CGame::GetCollision()->Circle(Info->pos, pPlayer->GetPosition(), SEARCHRANGE, pPlayer->GetRadius()) == true)
	{//�~�̒��Ƀv���C���[��������

		Info->state = pEnemyWeak->STATE_DASH;
		pMotion->Set(pEnemyWeak->MOTION_DASH);
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateMove);
	}
	else
	{// �v���C���[���~�̒��ɂ��Ȃ�

		Info->move.x = 0.0f;
		Info->move.z = 0.0f;

		//Info->pos.y = -80.0f;

		// �ҋ@��Ԃɂ���
		if (pMotion->GetType() != pEnemyWeak->MOTION_NEUTRAL)
			pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);

	}
}

//=================================================================
// �U���҂���Ԃ̏���
//=================================================================
//===========================================================
// �U���҂���Ԃ̃R���X�g���N�^
//===========================================================
CEnemyWeakFarStateAttackWait::CEnemyWeakFarStateAttackWait()
{

}

//===========================================================
// �U���҂���Ԃ̍X�V����
//===========================================================
void CEnemyWeakFarStateAttackWait::Update(CEnemyWeakFar* pEnemyWeak)
{
	// ���[�V�������擾
	CMotion* pMotion = pEnemyWeak->GetMotion();

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	if (pMotion->GetType() != pEnemyWeak->MOTION_BATTLE_NEUTRAL)
		pMotion->Set(pEnemyWeak->MOTION_BATTLE_NEUTRAL);

	m_nAtcCounter++;

	if (m_nAtcCounter >= ATTACKAGAINCOUNT)
	{
		m_nAtcCounter = 0;

		// �U����Ԃɂ���
		pMotion->Set(pEnemyWeak->MOTION_ATTACK);
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateAttack);
	}
}

//===========================================================
// �U����Ԃ̏���
//===========================================================
CEnemyWeakFarStateAttack::CEnemyWeakFarStateAttack()
{

}

//===========================================================
// �U����Ԃ̍X�V����
//===========================================================
void CEnemyWeakFarStateAttack::Update(CEnemyWeakFar* pEnemyWeak)
{
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	if (pMotion->GetNowFrame() == 30)
	{
		CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_ATTACK, ::Effekseer::Vector3D(Info->pos.x, Info->pos.y + 50.0f, Info->pos.z), ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(25.0f, 25.0f, 25.0f));
	}

	if (!m_bAttack && pMotion->GetNowFrame() == 70)
	{
		D3DXMATRIX* mtx = pEnemyWeak->GetCharcter()[5]->GetMtxWorld();

		CBullet::Create(D3DXVECTOR3(mtx->_41, mtx->_42, mtx->_43), Info->rot);

		m_bAttack = true;
	}
	
	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		// �v���C���[�Ƃ̋���
		float fLenght = utility::Distance(Info->pos, PlayerPos);

		Info->state = pEnemyWeak->STATE_NEUTRAL;
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		m_bAttack = false;

		// ��Ԃ̐؂�ւ�
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateMoveWait);
	}
}

//===========================================================
// �ړ���Ԃ̏���
//===========================================================
CEnemyWeakFarStateMove::CEnemyWeakFarStateMove()
{
}

void CEnemyWeakFarStateMove::Update(CEnemyWeakFar* pEnemyWeak)
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

		// ��Ԃ̐؂�ւ�
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateAttackWait);
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
			pEnemyWeak->ChangeState(new CEnemyWeakFarStateAttackWait);
		}
		
		pEnemy = pEnemyNext;
	}
}

//===========================================================
// �_���[�W��Ԃ̏���
//===========================================================
CEnemyWeakFarStateDamage::CEnemyWeakFarStateDamage()
{
	m_nRecoverDamageTime = RECOVER_DAMAGE_TIME;
}

void CEnemyWeakFarStateDamage::Update(CEnemyWeakFar* pEnemyWeak)
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

	if(m_nRecoverDamageTime <= 0)
	   pEnemyWeak->SetbDamage();

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateMoveWait);
		pEnemyWeak->SetbDamage();
	}
}

//================================================================
// ���ۏ�Ԃ̏���
//================================================================
//===========================================================
// ���ۏ�Ԃ̃R���X�g���N�^
//===========================================================
CEnemyWeakFarStateDenial::CEnemyWeakFarStateDenial()
{

}

//===========================================================
// ���ۏ�Ԃ̍X�V����
//===========================================================
void CEnemyWeakFarStateDenial::Update(CEnemyWeakFar* pEnemyWeak)
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
CEnemyWeakFarStateHeavyDamage::CEnemyWeakFarStateHeavyDamage()
{

}

//===========================================================
// �����U�����󂯂���Ԃ̍X�V����
//===========================================================
void CEnemyWeakFarStateHeavyDamage::Update(CEnemyWeakFar* pEnemyWeak)
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
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateGetUp);

		// �q�[�g�A�N�V��������������t���O��܂�
		pEnemyWeak->RestHeatDamageFrag();
	}
}

//===========================================================
// ���S��Ԃ̏���
//===========================================================
CEnemyWeakFarStateDeath::CEnemyWeakFarStateDeath()
{
}

void CEnemyWeakFarStateDeath::Update(CEnemyWeakFar* pEnemyWeak)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V���������S�ȊO�̂Ƃ�
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

CEnemyWeakFarStateGetUp::CEnemyWeakFarStateGetUp()
{
}

void CEnemyWeakFarStateGetUp::Update(CEnemyWeakFar* pEnemyWeak)
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
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateMoveWait);
		pEnemyWeak->SetbDamage();
	}
}

CEnemyWeakFarStateGrabbed::CEnemyWeakFarStateGrabbed()
{
}

void CEnemyWeakFarStateGrabbed::Update(CEnemyWeakFar* pEnemyWeak)
{
}