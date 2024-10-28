//===========================================================
//
// �G�̏��� [enemy.cpp]
// Author : �匴�@�叫
//
//===========================================================
#include "enemy.h"
#include "tutorial_enemy.h"
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
CTutorial_Enemy::CTutorial_Enemy()
{
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;


	m_Mobility = Immobile;
	m_nDamageCounter = 0;
	m_pLife3D = nullptr;
	m_bDamage = false;
	m_bStagger = false;
	ChangeState(new CTutorialEnemyNeutral);
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CTutorial_Enemy::CTutorial_Enemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority) : CEnemy(pos, rot, nlife, nPriority)
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
	ChangeState(new CTutorialEnemyNeutral);
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CTutorial_Enemy::~CTutorial_Enemy()
{

}

//===========================================================
// ��������
//===========================================================
CTutorial_Enemy* CTutorial_Enemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
{
	CTutorial_Enemy* pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CTutorial_Enemy(pos, rot, nlife, nPriority);

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
HRESULT CTutorial_Enemy::Init(void)
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
		m_pLife3D->SetPos(GetInfo()->pos);
		m_pLife3D->SetMatrix(mtx);
		m_pLife3D->SetUpHeight(90.0f);
		m_pLife3D->GetBill()->SetEdgeCenter((float)((Info->nLife * 0.01f) * 20), 5.0f);
	}

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CTutorial_Enemy::Uninit(void)
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
void CTutorial_Enemy::Update(void)
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

	// �X�e�C�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (Info->state == STATE_DEATH)
		return;

	if (m_pLife3D != nullptr)
	{
		m_pLife3D->GetBill()->SetEdgeCenter((float)((Info->nLife * 0.01f) * 20), 5.0f);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		m_pLife3D->SetPos(D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43));
	}

	CManager::GetInstance()->GetDebugProc()->Print("�G�̈ʒu�F[%f, %f, %f]\n", Info->pos.x, Info->pos.y, Info->pos.z);
}

//===========================================================
// �`�揈��
//===========================================================
void CTutorial_Enemy::Draw(void)
{
	CEnemy::Draw();
}

//===========================================================
// �X�e�C�g�̐؂�ւ�
//===========================================================
void CTutorial_Enemy::ChangeState(CTutorialEnemyState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//===========================================================
// �_���[�W����
//===========================================================
void CTutorial_Enemy::Damage(void)
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
		ChangeState(new CTutorialEnemyStateDamage);
	}
	if (Info->nLife > 0 && (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH))
	{// �̗͂��T�O�ȏ�̂Ƃ�

		// �_���[�W��Ԃɐ؂�ւ���
		ChangeState(new CTutorialEnemyStateHeavyDamage);
	}
	else if (Info->nLife <= 0)
	{// �̗͂��O�ȉ��̂Ƃ�

		// ���S��Ԃɐ؂�ւ���
		ChangeState(new CTutorialEnemyStateDeath);

		m_bDeath = true;
	}
}

//===========================================================
// �߂܂ꂽ�Ƃ��̏���
//===========================================================
void CTutorial_Enemy::Grabbed(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer->GetbGrap() == true)
		ChangeState(new CTutorialEnemyStateGrabbed);

	else if (pPlayer->GetbGrap() == false)
	{
		ChangeState(new CTutorialEnemyNeutral);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		GetInfo()->pos = D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43);

		// �G�̏��擾
		CEnemy::INFO* Info = GetInfo();
		D3DXVECTOR3 ForwardVector = utility::CalculateDirection(Info->pos, pPlayer->GetPosition());
		float angle = atan2f(ForwardVector.x, ForwardVector.z);
		angle -= D3DX_PI;
		angle = utility::CorrectAngle(angle);
		Info->rot.y = angle;

		m_bDamage = false;
	}
}

//===========================================================
// �q�[�g�A�N�V�����F���]�Ԏ󂯑҂�
//===========================================================
void CTutorial_Enemy::Denial(void)
{
	// �X�e�[�g�̐؂�ւ�
	ChangeState(new CTutorialEnemyStateDenial);
}

//======================================================================
// �X�e�C�g
//======================================================================
//===========================================================
// �_���[�W��Ԃ̏���
//===========================================================
CTutorialEnemyStateDamage::CTutorialEnemyStateDamage()
{
	m_nRecoverDamageTime = DAMAGECOUNT;
}

void CTutorialEnemyStateDamage::Update(CTutorial_Enemy* pEnemyWeak)
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
		pEnemyWeak->ChangeState(new CTutorialEnemyNeutral);
		pEnemyWeak->SetbDamage();
	}
}

//===========================================================
// �����U�����󂯂���Ԃ̏���
//===========================================================
CTutorialEnemyStateHeavyDamage::CTutorialEnemyStateHeavyDamage()
{

}

//===========================================================
// �����U�����󂯂���Ԃ̍X�V����
//===========================================================
void CTutorialEnemyStateHeavyDamage::Update(CTutorial_Enemy* pEnemyWeak)
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
		pEnemyWeak->ChangeState(new CTutorialEnemyStateGetUp);

		// �q�[�g�A�N�V��������������t���O��܂�
		pEnemyWeak->RestHeatDamageFrag();
	}
}

//===========================================================
// ���S��Ԃ̏���
//===========================================================
CTutorialEnemyStateDeath::CTutorialEnemyStateDeath()
{
}

void CTutorialEnemyStateDeath::Update(CTutorial_Enemy* pEnemyWeak)
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

CTutorialEnemyStateGetUp::CTutorialEnemyStateGetUp()
{
}

void CTutorialEnemyStateGetUp::Update(CTutorial_Enemy* pEnemyWeak)
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
		pEnemyWeak->ChangeState(new CTutorialEnemyNeutral);
		pEnemyWeak->SetbDamage();
	}
}

CTutorialEnemyStateGrabbed::CTutorialEnemyStateGrabbed()
{
}

void CTutorialEnemyStateGrabbed::Update(CTutorial_Enemy* pEnemyWeak)
{
}

CTutorialEnemyNeutral::CTutorialEnemyNeutral()
{
}

void CTutorialEnemyNeutral::Update(CTutorial_Enemy* pEnemy)
{
	CMotion* pMotion = pEnemy->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V�������N���オ��ȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemy->MOTION_NEUTRAL)
		pMotion->Set(pEnemy->MOTION_NEUTRAL);

}

//================================================================
// ���ۏ�Ԃ̏���
//================================================================
//===========================================================
// ���ۏ�Ԃ̃R���X�g���N�^
//===========================================================
CTutorialEnemyStateDenial::CTutorialEnemyStateDenial()
{

}

//===========================================================
// ���ۏ�Ԃ̍X�V����
//===========================================================
void CTutorialEnemyStateDenial::Update(CTutorial_Enemy* pEnemy)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemy->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V���������ۈȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemy->MOTION_DENIAL)
		pMotion->Set(pEnemy->MOTION_DENIAL);

}