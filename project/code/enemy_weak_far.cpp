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
#include "MyEffekseer.h"

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const int DAMEGE = 10;             // �_���[�W���
	const int ATTACKAGAINCOUNT = 60;   // �čU���ł���܂ł̎���
	const float SPEED = 2.0f;          // ���鑬��
	const float ATTACKLENGHT = 200.0f;  // �U���\�͈�
	const float SEARCHRANGE = 400.0f;  // �T���͈�
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
	m_nDamegeCounter = 0;
	/*m_pCurrent = nullptr;
	m_pNext = nullptr;
	m_pLife2D = nullptr;*/
	m_pLife3D = nullptr;
	//m_bDeath = false;
	m_bDamage = false;
	ChangeState(new CEnemyWeakFarStateMoveWait);
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CEnemyWeakFar::CEnemyWeakFar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority) : CEnemy(pos, rot, nlife, nPriority)
{
	// �l���N���A
	//SetState(CEnemy::STATE_NONE);
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;
	m_Mobility = Immobile;
	m_nDamegeCounter = 0;
	m_pLife3D = nullptr;
	m_bDamage = false;
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
		m_pLife3D->SetPos(&Info->pos);
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

	if (GetMobility() == Immobile)
		return;

	// �X�e�C�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (Info->state == STATE_DEATH)
		return;

	if (m_pLife3D != nullptr)
	{
		m_pLife3D->GetBill()->SetEdgeCenter((float)((Info->nLife * 0.01f) * 20), 5.0f);
	}

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

//===========================================================
// �_���[�W��Ԃ���̕��A
//===========================================================
void CEnemyWeakFar::RecoverFromDamage(void)
{
}

//===========================================================
// �_���[�W����
//===========================================================
void CEnemyWeakFar::Damege(void)
{
	// ���łɃ_���[�W���
	if (m_bDamage)
		return;

	// �G�̏��擾
	CEnemy::INFO* Info = GetInfo();

	if (Info == nullptr)
		return;

	if (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_NONE)
	{
		GetMotion()->Set(MOTION_DAMEGE);
	}
	else if (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH)
	{
		GetMotion()->Set(MOTION_DAMEGE);
	}

	Info->nLife -= CPlayer::GetInstance()->GetMotion()->GetAttackDamege();
	m_bDamage = true;

	if (Info->nLife > 0 && CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_NONE)
	{// �̗͂��T�O�ȏ�̂Ƃ�

		// �_���[�W��Ԃɐ؂�ւ���
		ChangeState(new CEnemyWeakFarStateDamege);
	}
	if (Info->nLife > 0 && (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH))
	{// �̗͂��T�O�ȏ�̂Ƃ�

		// �_���[�W��Ԃɐ؂�ւ���
		ChangeState(new CEnemyWeakFarStateHeavyDamege);
	}
	else if (Info->nLife <= 0)
	{// �̗͂��O�ȉ��̂Ƃ�

		// ���S��Ԃɐ؂�ւ���
		CGame::GetInstance()->SetbFinish(true);
		CCamera::GetInstance()->ChangeState(new FinalBlowCamera);
		ChangeState(new CEnemyWeakFarStateDeath);
	}
}

void CEnemyWeakFar::HardDamege(void)
{
}

void CEnemyWeakFar::Grabbed(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer->GetbGrap() == true)
		ChangeState(new CEnemyWeakFarStateGrabbed);

	else if (pPlayer->GetbGrap() == false)
		ChangeState(new CEnemyWeakFarStateMoveWait);
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

	//pEnemyWeak->HitDetection(Info->pos, ATTACKLENGHT, pPlayer->GetRadius());

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		// �v���C���[�Ƃ̋���
		float fLenght = utility::Distance(Info->pos, PlayerPos);

		Info->state = pEnemyWeak->STATE_NEUTRAL;
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);

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

	// �ǔ�
	fDiffmove = utility::MoveToPosition(Info->pos, PlayerPos, Info->rot.y);

	// �p�x�␳
	fDiffmove = utility::CorrectAngle(fDiffmove);

	Info->rot.y += fDiffmove * 0.05f;

	// �p�x�␳
	Info->rot.y = utility::CorrectAngle(Info->rot.y);

	//�ړ��ʂ��X�V(����������)
	Info->move.x = sinf(Info->rot.y + D3DX_PI) * SPEED;
	Info->move.z = cosf(Info->rot.y + D3DX_PI) * SPEED;

	Info->pos += Info->move;

	// �v���C���[�Ƃ̋���
	float fLenght = utility::Distance(Info->pos, PlayerPos);

	if (fLenght <= ATTACKLENGHT)
	{// �U���\�͈͓��ɓ�����

		// ��Ԃ̐؂�ւ�
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateAttackWait);
	}
}

//===========================================================
// �_���[�W��Ԃ̏���
//===========================================================
CEnemyWeakFarStateDamege::CEnemyWeakFarStateDamege()
{

}

void CEnemyWeakFarStateDamege::Update(CEnemyWeakFar* pEnemyWeak)
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

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateMoveWait);
		pEnemyWeak->SetbDamage();
	}
}

//===========================================================
// �����U�����󂯂���Ԃ̏���
//===========================================================
CEnemyWeakFarStateHeavyDamege::CEnemyWeakFarStateHeavyDamege()
{

}

//===========================================================
// �����U�����󂯂���Ԃ̍X�V����
//===========================================================
void CEnemyWeakFarStateHeavyDamege::Update(CEnemyWeakFar* pEnemyWeak)
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