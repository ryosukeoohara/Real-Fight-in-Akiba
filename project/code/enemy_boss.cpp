//===========================================================
//
// �{�X�̏��� [enemy_boss.cpp]
// Author : �匴�@�叫
//
//===========================================================
#include "enemy.h"
#include "enemy_boss.h"
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
#include "fade.h"
#include "utility.h"
#include "gage.h"
#include "camera.h"
#include "particle.h"
#include "item.h"
#include "animation.h"
#include "InputKeyBoard.h"
#include "fade.h"
#include "MyEffekseer.h"

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const int DAMAGECOUNT = 20;         // �_���[�W���
	const float ATTACKLENGTH = 120.0f;  // �U���\�ȋ���
	const float SPEED = 2.0f;           // ���鑬��
	const float ATTACKLENGHT = 100.0f;   // �U���\�͈�
	const int STAGGER_TIME = 600;       // ���߂���Ԃł��鎞��
	const int RECOVER_DAMAGE_TIME = 15;  // �_���[�W��Ԃł��鎞��
	const int NUM_DAMAGE_TIME = 60;      // �_���[�W��Ԃ��������v����

	const D3DXVECTOR3 CAMERA_ROT[CPlayer::HEAT_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 2.35f, D3DX_PI * -0.38f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.15f, D3DX_PI * -0.38f),

	};  // �q�[�g�A�N�V�������̃J�����ʒu

	const char *ENEMY_TEXT = "data\\TEXT\\motion_boss.txt";
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CEnemyBoss::CEnemyBoss()
{
	m_Chase = CHASE_ON;
	m_pLife2D = nullptr;
	m_nBiriBiriCount = 0;
	m_nReceivedAttack = 0;
	m_nIdleTime = 0;
	m_bDamage = false;
	m_bHeatDamage = false;
	m_bAttack = false;
	m_IsAttackReady = false;
	m_bDeath = false;
	m_bStagger = false;
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CEnemyBoss::CEnemyBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority) : CEnemy(pos, rot, nlife, nPriority)
{
	// �l���N���A
	SetPosition(pos);
	SetRotition(rot);
	SetLife(nlife);
	SetState(CEnemy::STATE_NONE);
	m_nAtcCounter = 0;
	m_Chase = CHASE_ON;
	m_pLife2D = nullptr;
	m_nBiriBiriCount = 0;
	m_nReceivedAttack = 0;
	m_nIdleTime = 0;
	m_bDamage = false;
	m_bHeatDamage = false;
	m_bAttack = false;
	m_IsAttackReady = false;
	m_bDeath = false;
	m_bStagger = false;
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CEnemyBoss::~CEnemyBoss()
{

}

//===========================================================
// ��������
//===========================================================
CEnemyBoss * CEnemyBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
{
	CEnemyBoss *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CEnemyBoss(pos, rot, nlife);

		pEnemy->Init();
	}

	return pEnemy;
}

//===========================================================
// ����������
//===========================================================
HRESULT CEnemyBoss::Init(void)
{
	CEnemy::Init();
	SetType(BOSS);
	ReadText(ENEMY_TEXT);

	CMotion* pMotion = GetMotion();

	if (pMotion != nullptr)
	{
		//pMotion->Set(MOTION_ONSTEGE);
		ChangeState(new CEnemyBossAppear);
	}

	// �G�̏��擾
	CEnemy::INFO* Info = GetInfo();

	if (m_pLife2D == nullptr)
	{
		m_pLife2D = CGage2D::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.25f) + 4.0f, 650.0f, 0.0f), 40.0f, (float)Info->nLife * 2.0f, CGage2D::TYPE_LIFE);
		m_pLife2D->GetObj2D()->SetEdgeCenterTex((float)(Info->nLife * 0.1f));
	}

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CEnemyBoss::Uninit(void)
{
	CEnemy::Uninit();
	CObject::Release();

	if (m_pLife2D != nullptr)
	{
		m_pLife2D->Uninit();
		m_pLife2D = nullptr;
	}
}

//===========================================================
// �X�V����
//===========================================================
void CEnemyBoss::Update(void)
{
	CEnemy::Update();

	// �G�̏��擾
	CEnemy::INFO* Info = GetInfo();

	if (Info == nullptr)
		return;

	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
		return;

	// �X�e�C�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (m_pLife2D != nullptr)
		m_pLife2D->GetObj2D()->SetEdgeCenterTex((float)((Info->nLife * 0.1f) * 20.0f));

	// �_���[�W��Ԃł͂Ȃ��Ƃ������𔲂���
	if (!m_bDamage || m_bHeatDamage || m_bStagger || Info->nLife < 0)
		return;

	m_nIdleTime++;

	if (m_nIdleTime >= NUM_DAMAGE_TIME)
	{
		m_nIdleTime = 0;

		// �v���C���[�̏��擾
		CPlayer* pPlayer = CPlayer::GetInstance();

		if (pPlayer == nullptr)
			return;

		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		// �v���C���[�Ƃ̋���
		float fLenght = utility::Distance(Info->pos, PlayerPos);

		if (fLenght <= ATTACKLENGHT)
		{// �U���\�͈͓��ɓ�����

			m_IsAttackReady = true;
			m_bDamage = false;

			ChangeState(new CEnemyBossStateAttackWait);
		}
	}
}

//===========================================================
// �`�揈��
//===========================================================
void CEnemyBoss::Draw(void)
{
	CEnemy::Draw();
}

//===========================================================
// �X�e�C�g�̐؂�ւ�
//===========================================================
void CEnemyBoss::ChangeState(CEnemyBossState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//===========================================================
// �߂܂ꂽ���̏���
//===========================================================
void CEnemyBoss::Grabbed(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer->GetbGrap() == true)
	{
		ChangeState(new CEnemyBossStateGrabbed);
		m_bAttack = false;
	}
	else if (pPlayer->GetbGrap() == false)
	{
		ChangeState(new CEnemyBossStateMove);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		GetInfo()->pos = D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43);

		// �G�̏��擾
		CEnemy::INFO* Info = GetInfo();
		D3DXVECTOR3 ForwardVector = utility::CalculateDirection(Info->pos, pPlayer->GetPosition());
		float angle = atan2f(ForwardVector.x, ForwardVector.z);
		angle -= D3DX_PI;
		angle = utility::CorrectAngle(angle);
		Info->rot.y = angle;
		m_bAttack = false;
		m_bDamage = false;
	}
		
}

//===========================================================
// �q�[�g�A�N�V�����F���]�Ԏ󂯑҂�
//===========================================================
void CEnemyBoss::Denial(void)
{
	// �X�e�[�g�̐؂�ւ�
	ChangeState(new CEnemyBossStateDenial);

	m_bAttack = false;
}

//===========================================================
// �_���[�W����
//===========================================================
void CEnemyBoss::Damage(void)
{
	// ���łɃ_���[�W���
	if (m_bDamage || m_bAttack || m_IsAttackReady || m_bDeath)
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
	else if (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH)
	{
		GetMotion()->Set(MOTION_FALLDOWN);
		m_bHeatDamage = true;
	}
	else if (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE)
	{
		GetMotion()->Set(MOTION_HEATACTFAINTING); 
		m_bHeatDamage = true;
	}
	
	Info->nLife -= CPlayer::GetInstance()->GetMotion()->GetAttackDamage();
	m_bStagger = false;
	CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_HIT, ::Effekseer::Vector3D(Info->pos.x, Info->pos.y + 50.0f, Info->pos.z));

	if (Info->nLife >= 30 && CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_NONE)
	{// �̗͂��T�O�ȏ�̂Ƃ�

		// �_���[�W��Ԃɐ؂�ւ���
		ChangeState(new CEnemyBossStateDamage);
	}
	else if (Info->nLife >= 30 && (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH))
	{// �̗͂��T�O�ȏ�̂Ƃ�

		// �_���[�W��Ԃɐ؂�ւ���
		ChangeState(new CEnemyBossStateHeavyDamage);
	}
	else if (Info->nLife < 30 && Info->nLife > 0)
	{// �̗͂��T�O��������0��荂���Ƃ�
		
		// ���߂���Ԃɐ؂�ւ���
		ChangeState(new CEnemyBossStateKnockDown);
		m_bStagger = true;
	}
	else if (Info->nLife <= 0 && CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE)
	{// �̗͂��O�ȉ��̂Ƃ�

		// ���S��Ԃɐ؂�ւ���
		CGame::GetInstance()->SetbFinish(true);
		CCamera::GetInstance()->ChangeState(new FinalBlowCamera);
		ChangeState(new CEnemyBossStateDeathByMicrowave);
		m_bDeath = true;
	}
	else if (Info->nLife <= 0)
	{
		CGame::GetInstance()->SetbFinish(true);
		CCamera::GetInstance()->ChangeState(new FinalBlowCamera);
		ChangeState(new CEnemyBossStateDeath);

		m_bDeath = true;
	}
}

//===========================================================
// ���邮��p���`�U��
//===========================================================
void CEnemyBoss::RollingPunch(void)
{
	// �L�����̃p�[�c�擾
	CCharacter **ppParts = nullptr;
	ppParts = GetCharcter();

	D3DXVECTOR3 PartsPosR = {};
	D3DXMATRIX *PartsMtxR = {};
	D3DXVECTOR3 PartsPosL = {};
	D3DXMATRIX *PartsMtxL = {};

	// ����
	if (ppParts[8] != nullptr)
	{
		// �}�g���b�N�X�̎擾
		PartsMtxR = ppParts[8]->GetMtxWorld();

		PartsPosR.x = PartsMtxR->_41;
		PartsPosR.y = PartsMtxR->_42;
		PartsPosR.z = PartsMtxR->_43;
	}

	// �E��
	if (ppParts[11] != nullptr)
	{
		// �}�g���b�N�X�̎擾
		PartsMtxL = ppParts[11]->GetMtxWorld();

		PartsPosL.x = PartsMtxL->_41;
		PartsPosL.y = PartsMtxL->_42;
		PartsPosL.z = PartsMtxL->_43;
	}

	// �U������
	HitDetection(PartsPosR, 20.0f, 50.0f);

	// �U������
	HitDetection(PartsPosL, 20.0f, 50.0f);
}

//===========================================================
// ���ʂ̃p���`
//===========================================================
void CEnemyBoss::NormalPunch(void)
{
	// �L�����̃p�[�c�擾
	CCharacter **ppParts = nullptr;
	ppParts = GetCharcter();

	D3DXVECTOR3 PartsPos = {};
	D3DXMATRIX *PartsMtx = {};

	// �E��
	if (ppParts[11] != nullptr)
	{
		// �}�g���b�N�X�̎擾
		PartsMtx = ppParts[11]->GetMtxWorld();

		PartsPos.x = PartsMtx->_41;
		PartsPos.y = PartsMtx->_42;
		PartsPos.z = PartsMtx->_43;
	}

	// �U������
	HitDetection(PartsPos, 50.0f, 50.0f);
}

//===========================================================
// ��э��ݍU��
//===========================================================
void CEnemyBoss::Fly(void)
{
	// �L�����̃p�[�c�擾
	CCharacter **ppParts = nullptr;
	ppParts = GetCharcter();

	D3DXVECTOR3 PartsPos = {};
	D3DXMATRIX *PartsMtx = {};

	// ��
	if (ppParts[5] != nullptr)
	{
		// �}�g���b�N�X�̎擾
		PartsMtx = ppParts[5]->GetMtxWorld();

		PartsPos.x = PartsMtx->_41;
		PartsPos.y = PartsMtx->_42;
		PartsPos.z = PartsMtx->_43;
	}

	// �U������
	HitDetection(PartsPos, 50.0f, 50.0f);
}

//===========================================================
// �U�����������Ă��邩�ǂ�������
//===========================================================
void CEnemyBoss::HitDetection(D3DXVECTOR3 MyPos, float attackrange, float targetradius)
{
	int Now = GetMotion()->GetNowFrame();
	int Occurs = GetMotion()->GetAttackOccurs();

	if (Now == Occurs)
	{
		D3DXMATRIX mtx = *GetCharcter()[0]->GetMtxWorld();
		CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_ATTACK, ::Effekseer::Vector3D(mtx._41, mtx._42, mtx._43), ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(25.0f, 25.0f, 25.0f));
	}

	if (GetMotion()->GetAttackOccurs() <= GetMotion()->GetNowFrame() && GetMotion()->GetAttackEnd() >= GetMotion()->GetNowFrame())
	{// ���݂̃t���[�����U�����蔭���t���[���ȏォ�U������I���t���[���Ȃ�

		if (CGame::GetCollision()->Circle(MyPos, CGame::GetPlayer()->GetPosition(), attackrange, targetradius) == true)
			CGame::GetPlayer()->Damage(GetMotion()->GetAttackDamage(), D3DXVECTOR3(sinf(GetRotition().y) * GetMotion()->GetKnockBack(), 0.0f, cosf(GetRotition().y) * GetMotion()->GetKnockBack()));
	}
}

//================================================================
// �����Ă���Ƃ��̃G�t�F�N�g
//================================================================
void CEnemyBoss::DashEffect(void)
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

//======================================================================
// �X�e�[�g
//======================================================================
CEnemyBossState::CEnemyBossState()
{
}

//================================================================
// �ҋ@�҂���Ԃ̏���
//================================================================
CEnemyBossStateMoveWait::CEnemyBossStateMoveWait()
{

}

void CEnemyBossStateMoveWait::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����


}


//================================================================
// �ړ���Ԃ̏���
//================================================================
CEnemyBossStateMove::CEnemyBossStateMove()
{

}

void CEnemyBossStateMove::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	// ���[�V�������擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	if (pMotion->GetType() != pEnemyBoss->MOTION_DASH)
		pMotion->Set(pEnemyBoss->MOTION_DASH);

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

	// �����Ă���Ƃ��̃G�t�F�N�g
	pEnemyBoss->DashEffect();

	// �v���C���[�Ƃ̋���
	float fLenght = utility::Distance(Info->pos, PlayerPos);

	if (fLenght <= ATTACKLENGHT)
	{// �U���\�͈͓��ɓ�����

		pMotion->Set(CEnemy::MOTION_NEUTRAL);
		pEnemyBoss->ChangeState(new CEnemyBossStateAttackWait);
	}
}

//================================================================
// �U����Ԃ̏���
//================================================================
CEnemyBossStateAttack::CEnemyBossStateAttack()
{

}

void CEnemyBossStateAttack::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����


}


//================================================================
// �U���҂���Ԃ̏���
//================================================================
CEnemyBossStateAttackWait::CEnemyBossStateAttackWait()
{

}

void CEnemyBossStateAttackWait::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����

	// ���[�V�������擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// �U�����Ă��Ȃ��Ƃ�
	if(!pEnemyBoss->GetbAttack())
	   m_nAtcCounter++;
	
	if (m_nAtcCounter >= 60 || pEnemyBoss->GetbAttackReady())
	{
		m_nAtcCounter = 0;
		pEnemyBoss->SetbAttack(true);
		pEnemyBoss->SetbAttackReady(false);

		m_nAttackType = rand() % ATTACKTYPE_MAX;  //�U���̎�ޒ��I

		switch (m_nAttackType)
		{
		case ATTACKTYPE_GURUGURU:  // ���邮��p���`

			pMotion->Set(pEnemyBoss->MOTION_GURUGURU);
			pEnemyBoss->RollingPunch();

			break;

		case ATTACKTYPE_PUNCH:  // ���ʂ̃p���`

			pMotion->Set(pEnemyBoss->MOTION_PUNCH);
			pEnemyBoss->NormalPunch();

			break;

		case ATTACKTYPE_FLY:  // ���ł��Ȃ����痎����

			pMotion->Set(pEnemyBoss->MOTION_ATTACK);
			pEnemyBoss->Fly();

			break;

		default:
			break;
		}
	}
	else if(m_nAtcCounter == 0 && pEnemyBoss->GetbAttack())
	{
	switch (m_nAttackType)
	{
	case ATTACKTYPE_GURUGURU:  // ���邮��p���`

 		pEnemyBoss->RollingPunch();

		break;

	case ATTACKTYPE_PUNCH:  // ���ʂ̃p���`

		pEnemyBoss->NormalPunch();

		break;

	case ATTACKTYPE_FLY:  // ���ł��Ȃ����痎����

		pEnemyBoss->Fly();

		break;

	default:
		break;
	}
	}

	// ���[�V�������I������
	if (pMotion->IsFinish())
	{
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
		pEnemyBoss->SetbAttack(false);
	}
		
	// �U�����Ă���Ƃ��͏����𔲂���
	if (pEnemyBoss->GetbAttack())
		return;

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	// �v���C���[�̏��擾
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	// �v���C���[�Ƃ̋���
	float fLenght = utility::Distance(Info->pos, PlayerPos);

	if (fLenght > ATTACKLENGHT)
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);

}

//================================================================
// �_���[�W��Ԃ̏���
//================================================================
//===========================================================
// �_���[�W��Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossStateDamage::CEnemyBossStateDamage()
{
	m_nRecoverDamageTime = DAMAGECOUNT;
}

//===========================================================
// �_���[�W��Ԃ̍X�V����
//===========================================================
void CEnemyBossStateDamage::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	m_nRecoverDamageTime--;

	if (m_nRecoverDamageTime <= 0)
		pEnemyBoss->SetbDamage();

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		pMotion->Set(pEnemyBoss->MOTION_NEUTRAL);
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
		
	}
}

//================================================================
// �����U�����󂯂���Ԃ̏���
//================================================================
//===========================================================
// �����U�����󂯂���Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossStateHeavyDamage::CEnemyBossStateHeavyDamage()
{
}

//===========================================================
// �����U�����󂯂���Ԃ̍X�V����
//===========================================================
void CEnemyBossStateHeavyDamage::Update(CEnemyBoss* pEnemyBoss)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V�������m�b�N�_�E���ȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemyBoss->MOTION_HEATDAMAGE)
		pMotion->Set(pEnemyBoss->MOTION_HEATDAMAGE);

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		pEnemyBoss->ChangeState(new CEnemyBossStateGetUp);

		// �q�[�g�A�N�V��������������t���O��܂�
		pEnemyBoss->RestHeatDamageFrag();
	}
}

//================================================================
// ���߂���Ԃ̏���
//================================================================
//===========================================================
// ���߂���Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossStateStagger::CEnemyBossStateStagger()
{
	m_nStaggerTime = STAGGER_TIME;  // ���߂���Ԃł��鎞��
}

//===========================================================
// ���߂���Ԃ̍X�V����
//===========================================================
void CEnemyBossStateStagger::Update(CEnemyBoss* pEnemyBoss)
{
	pEnemyBoss->SetbDamage();

	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	// ���[�V���������߂��ȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemyBoss->MOTION_SUTAN)
		pMotion->Set(pEnemyBoss->MOTION_SUTAN);

	m_nStaggerTime--;

	if (m_nStaggerTime <= 0)
	{
		pEnemyBoss->ChangeState(new CEnemyBossStateStaggerReCover);
	}
}

//================================================================
// �]�ԏ�Ԃ̏���
//================================================================
//===========================================================
// �]�ԏ�Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossStateKnockDown::CEnemyBossStateKnockDown()
{

}

//===========================================================
// �]�ԏ�Ԃ̍X�V����
//===========================================================
void CEnemyBossStateKnockDown::Update(CEnemyBoss* pEnemyBoss)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V�������m�b�N�_�E���ȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemyBoss->MOTION_HEATDAMAGE)
		pMotion->Set(pEnemyBoss->MOTION_HEATDAMAGE);

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		pEnemyBoss->SetbHeatDamage(false);
		pEnemyBoss->ChangeState(new CEnemyBossStateGetUp);
	}
}

//================================================================
// �N���オ���Ԃ̏���
//================================================================
//===========================================================
// �N���オ���Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossStateGetUp::CEnemyBossStateGetUp()
{

}

//===========================================================
// �N���オ���Ԃ̍X�V����
//===========================================================
void CEnemyBossStateGetUp::Update(CEnemyBoss* pEnemyBoss)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	// ���[�V�������N���オ��ȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemyBoss->MOTION_GETUP)
		pMotion->Set(pEnemyBoss->MOTION_GETUP);


	if (pMotion->IsFinish() && Info->nLife < 50)
	{// ���[�V�������I�����Ă��Ă��A�̗͂��T�O������������

		// ���߂���Ԃɐ؂�ւ���
		pEnemyBoss->ChangeState(new CEnemyBossStateStagger);
		pEnemyBoss->SetbDamage();
	}
	else if (pMotion->IsFinish())
	{// ���[�V�������I�����Ă���

		// �ړ���Ԃɐ؂�ւ���
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
		pEnemyBoss->SetbDamage();
	}
}

//================================================================
// ���߂���Ԃ���̕��A������
//================================================================
//===========================================================
// ���߂���Ԃ���̕��A�����Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossStateStaggerReCover::CEnemyBossStateStaggerReCover()
{

}

//===========================================================
// ���߂���Ԃ���̕��A�����Ԃ̍X�V����
//===========================================================
void CEnemyBossStateStaggerReCover::Update(CEnemyBoss* pEnemyBoss)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	// ���[�V�������N���オ��ȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemyBoss->MOTION_SUTANRECOVER)
		pMotion->Set(pEnemyBoss->MOTION_SUTANRECOVER);

	// ���[�V�������I�����Ă���
	if (pMotion->IsFinish())
	{
		Info->nLife += 100;
		pEnemyBoss->SetbDamage();

		// �ړ���Ԃɐ؂�ւ���
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
	}
}

//================================================================
// ���S��Ԃ̏���
//================================================================
//===========================================================
// ���S��Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossStateDeath::CEnemyBossStateDeath()
{

}

//===========================================================
// ���S��Ԃ̍X�V����
//===========================================================
void CEnemyBossStateDeath::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����

	//�t�F�[�h�̏����擾
	CFade* pFade = CManager::GetInstance()->GetFade();

	if (m_bDeath)
	{
		if (pFade->Get() != pFade->FADE_OUT)
			pFade->Set(CScene::MODE_RESULT);

		return;
	}

	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V���������S�ȊO��������
	if(pMotion->GetType() != CEnemyBoss::MOTION_DEATH)
		pMotion->Set(CEnemyBoss::MOTION_DEATH);

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		// �G�̑��������炷
		int nNum = CEnemyManager::GetNum() - 1;
		CEnemyManager::SetNum(nNum);

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{// �Q�[���̎�

			// �G��|�������𑝂₷
			int nDefeat = CManager::GetInstance()->GetDefeat();
			nDefeat++;
			CManager::GetInstance()->SetDefeat(nDefeat);
			m_bDeath = true;
			
		}
	}
}

//================================================================
// �d�q�����W�Ŏ��S������Ԃ̏���
//================================================================
//===========================================================
// �d�q�����W�Ŏ��S������Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossStateDeathByMicrowave::CEnemyBossStateDeathByMicrowave()
{
}

//===========================================================
// �d�q�����W�Ŏ��S������Ԃ̍X�V����
//===========================================================
void CEnemyBossStateDeathByMicrowave::Update(CEnemyBoss* pEnemyBoss)
{
	////�t�F�[�h�̏����擾
	//CFade* pFade = CManager::GetInstance()->GetFade();

	//if (pFade->Get() != pFade->FADE_OUT)
	//	pFade->Set(CScene::MODE_RESULT);

	// ���S������Ԃ̂Ƃ������𔲂���
	if (m_bDeath)
		return;

	// �G�̑��������炷
	int nNum = CEnemyManager::GetNum() - 1;
	CEnemyManager::SetNum(nNum);

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{// �Q�[���̎�

		// �G��|�������𑝂₷
		int nDefeat = CManager::GetInstance()->GetDefeat();
		nDefeat++;
		CManager::GetInstance()->SetDefeat(nDefeat);
		m_bDeath = true;
	}
}

//================================================================
// �߂܂�Ă����Ԃ̏���
//================================================================
//===========================================================
// �߂܂�Ă����Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossStateGrabbed::CEnemyBossStateGrabbed()
{

}

//===========================================================
// �߂܂�Ă����Ԃ̍X�V����
//===========================================================
void CEnemyBossStateGrabbed::Update(CEnemyBoss* pEnemyBoss)
{

}

//================================================================
// �o�ꂵ�Ă���ړ��J�n����܂ł̏�Ԃ̏���
//================================================================
//===========================================================
// �ړ��J�n�҂���Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossAppear::CEnemyBossAppear()
{

}

//===========================================================
// �ړ��J�n�҂���Ԃ̍X�V����
//===========================================================
void CEnemyBossAppear::Update(CEnemyBoss* pEnemyBoss)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V���������т���ȊO�̂Ƃ�
	if(pMotion->GetType() != pEnemyBoss->MOTION_KUGIKIRI)
	   pMotion->Set(pEnemyBoss->MOTION_KUGIKIRI);

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		pMotion->Set(pEnemyBoss->MOTION_ONSTEGE);
		pEnemyBoss->ChangeState(new CEnemyBossAppearMove);
	}
}

//================================================================
// �o���Ԃ̏���
//================================================================
//===========================================================
// �o���Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossAppearMove::CEnemyBossAppearMove()
{

}

void CEnemyBossAppearMove::Update(CEnemyBoss* pEnemyBoss)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	CPlayer* pPlayer = CPlayer::GetInstance();
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (pPlayer != nullptr)
		posPlayer = pPlayer->GetPosition();

	float fDiffmove = 0.0f;

	// �ǔ�
	fDiffmove = utility::MoveToPosition(Info->pos, posPlayer, Info->rot.y);

	// �p�x�␳
	fDiffmove = utility::CorrectAngle(fDiffmove);

	Info->rot.y += fDiffmove * 0.05f;

	// �p�x�␳
	Info->rot.y = utility::CorrectAngle(Info->rot.y);

	//�ړ��ʂ��X�V(����������)
	Info->move.x = sinf(Info->rot.y + D3DX_PI) * 2.0f;
	Info->move.z = cosf(Info->rot.y + D3DX_PI) * 2.0f;

	Info->pos += Info->move;

	float Distance = utility::Distance(Info->pos, posPlayer);

	if (Distance <= 400.0f)
	{
		pMotion->Set(CEnemyBoss::MOTION_KUGIKIRI);
		pEnemyBoss->ChangeState(new CEnemyBossOnStege);
		CCamera::GetInstance()->ChangeState(new CutSceneCamera);
	}
}

//================================================================
// �o���Ԃ̏���
//================================================================
//===========================================================
// �o���Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossOnStege::CEnemyBossOnStege()
{
}

//===========================================================
// �o���Ԃ̍X�V����
//===========================================================
void CEnemyBossOnStege::Update(CEnemyBoss* pEnemyBoss)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	if (pMotion->IsFinish())
	{
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
		pMotion->Set(pEnemyBoss->MOTION_NEUTRAL);
	}
}

//================================================================
// ���ۏ�Ԃ̏���
//================================================================
//===========================================================
// ���ۏ�Ԃ̃R���X�g���N�^
//===========================================================
CEnemyBossStateDenial::CEnemyBossStateDenial()
{

}

//===========================================================
// ���ۏ�Ԃ̍X�V����
//===========================================================
void CEnemyBossStateDenial::Update(CEnemyBoss* pEnemyBoss)
{
	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V���������ۈȊO�̂Ƃ�
	if (pMotion->GetType() != pEnemyBoss->MOTION_DENIAL)
		pMotion->Set(pEnemyBoss->MOTION_DENIAL);

}