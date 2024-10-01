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
#include "MyEffekseer.h"

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const int DAMEGECOUNT = 10;         // �_���[�W���
	const float ATTACKLENGTH = 120.0f;  // �U���\�ȋ���
	const float SPEED = 2.0f;           // ���鑬��
	const float ATTACKLENGHT = 50.0f;   // �U���\�͈�

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
	m_nAttackType = -1;
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CEnemyBoss::CEnemyBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife)
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
	m_nAttackType = -1;
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
CEnemyBoss * CEnemyBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife)
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

void CEnemyBoss::RecoverFromDamage(void)
{
	//// �G�̏��擾
	//CEnemy::INFO* Info = GetInfo();

	//if (Info == nullptr)
	//	return;

	//CMotion* pMotion = GetMotion();

	//if (pMotion == nullptr)
	//	return;

	//CPlayer* pPlayer = CPlayer::GetInstance();

	//if (pPlayer == nullptr)
	//	return;

	//// ���[�V�������I�����Ă�����
	//if (pMotion->IsFinish())
	//{
	//	Info->state = pEnemyWeak->STATE_NEUTRAL;
	//	pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
	//	pEnemyWeak->ChangeState(new CEnemyWeakStateMoveWait);
	//}

	//// ���[�V�������I�����Ă�����
	//if (pMotion->IsFinish())
	//{
	//	Info->state = CEnemy::STATE_NEUTRAL;
	//	pMotion->Set(MOTION_NEUTRAL);
	//	ChangeState(new CEnemyBossStateMove);
	//}
}

//===========================================================
// �_���[�W����
//===========================================================
void CEnemyBoss::Damege(void)
{
	ChangeState(new CEnemyBossStateDamege);
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
	
}

//===========================================================
// ����������
//===========================================================
HRESULT CEnemyBoss::Init(void)
{
	CEnemy::Init();
	SetType(BOSS);
	ReadText(ENEMY_TEXT);

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

	if (Info != nullptr)
		return;

	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
		return;

	// �X�e�C�g�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (m_pLife2D != nullptr)
		m_pLife2D->GetObj2D()->SetEdgeCenterTex((float)((Info->nLife * 0.1f) * 20.0f));

	if (Info->nLife <= 0 && Info->state != STATE_DEATH)
	{
		Info->state = CEnemy::STATE_DEATH;
		pMotion->Set(CEnemy::MOTION_DEATH);
		ChangeState(new CEnemyBossStateDeath);
	}
}

//===========================================================
// �`�揈��
//===========================================================
void CEnemyBoss::Draw(void)
{
	CEnemy::Draw();
}

//======================================================================
// �X�e�C�g
//======================================================================
CEnemyBossState::CEnemyBossState()
{
}

//===========================================================
// �ҋ@�҂���Ԃ̏���
//===========================================================
CEnemyBossStateMoveWait::CEnemyBossStateMoveWait()
{

}

void CEnemyBossStateMoveWait::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����


}


//===========================================================
// �ړ���Ԃ̏���
//===========================================================
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

		Info->state = CEnemy::STATE_NEUTRAL;
		pMotion->Set(CEnemy::MOTION_NEUTRAL);
		pEnemyBoss->ChangeState(new CEnemyBossStateAttackWait);
	}
}


//===========================================================
// �U����Ԃ̏���
//===========================================================
CEnemyBossStateAttack::CEnemyBossStateAttack()
{

}

void CEnemyBossStateAttack::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����


}


//===========================================================
// �U���҂���Ԃ̏���
//===========================================================
CEnemyBossStateAttackWait::CEnemyBossStateAttackWait()
{

}

void CEnemyBossStateAttackWait::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����


}





//===========================================================
// �_���[�W��Ԃ̏���
//===========================================================
CEnemyBossStateDamege::CEnemyBossStateDamege()
{
}

void CEnemyBossStateDamege::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����

	// �G�̏��擾
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		Info->state = CEnemy::STATE_NEUTRAL;
		pMotion->Set(pEnemyBoss->MOTION_NEUTRAL);
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
	}

	if (Info->state != pEnemyBoss->STATE_DAMEGE)
	{
		Info->state = CEnemy::STATE_DAMEGE;
		pMotion->Set(pEnemyBoss->MOTION_DAMEGE);
		Info->nLife -= pPlayer->GetMotion()->GetAttackDamege();
	}
}


//===========================================================
// ���S��Ԃ̏���
//===========================================================
CEnemyBossStateDeath::CEnemyBossStateDeath()
{

}

void CEnemyBossStateDeath::Update(CEnemyBoss* pEnemyBoss)
{// �X�V����

	// ���[�V�����̏��擾
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// ���[�V�������I�����Ă�����
	if (pMotion->IsFinish())
	{
		// �G�̑��������炷
		int nNum = CEnemyManager::GetNum() - 1;
		CEnemyManager::SetNum(nNum);
		pEnemyBoss->Uninit();

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{// �Q�[���̎�

			// �G��|�������𑝂₷
			int nDefeat = CPlayer::GetInstance()->GetDefeat() + 1;
			CPlayer::GetInstance()->SetDefeat(nDefeat);
		}
	}
}


////===========================================================
//// �U���̎��
////===========================================================
//CAttackType::CAttackType()
//{
//}
//
//CAttackTypeRollingPunch::CAttackTypeRollingPunch()
//{
//}
//
////===========================================================
//// �U���̎�ށF���[�����O�p���`
////===========================================================
//void CAttackTypeRollingPunch::Update(CEnemy* pEnemy)
//{
//	// �L�����̃p�[�c�擾
//	CCharacter** ppParts = nullptr;
//	ppParts = pEnemy->GetCharcter();
//
//	if (ppParts == nullptr)
//		return;
//
//	D3DXVECTOR3 PartsPosR = {};
//	D3DXMATRIX* PartsMtxR = {};
//	D3DXVECTOR3 PartsPosL = {};
//	D3DXMATRIX* PartsMtxL = {};
//
//	// ����
//	if (ppParts[8] != nullptr)
//	{
//		// �}�g���b�N�X�̎擾
//		PartsMtxR = ppParts[8]->GetMtxWorld();
//
//		PartsPosR.x = PartsMtxR->_41;
//		PartsPosR.y = PartsMtxR->_42;
//		PartsPosR.z = PartsMtxR->_43;
//	}
//
//	// �E��
//	if (ppParts[11] != nullptr)
//	{
//		// �}�g���b�N�X�̎擾
//		PartsMtxL = ppParts[11]->GetMtxWorld();
//
//		PartsPosL.x = PartsMtxL->_41;
//		PartsPosL.y = PartsMtxL->_42;
//		PartsPosL.z = PartsMtxL->_43;
//	}
//
//	// �U������
//	pEnemy->HitDetection(PartsPosR, 20.0f, 50.0f);
//
//	// �U������
//	pEnemy->HitDetection(PartsPosL, 20.0f, 50.0f);
//}
//
//CAttackTypeNormalPunch::CAttackTypeNormalPunch()
//{
//}
//
////===========================================================
//// �U���̎�ށF�m�[�}���p���`
////===========================================================
//void CAttackTypeNormalPunch::Update(CEnemy* pEnemy)
//{
//	// �L�����̃p�[�c�擾
//	CCharacter** ppParts = nullptr;
//	ppParts = pEnemy->GetCharcter();
//
//	if (ppParts == nullptr)
//		return;
//
//	D3DXVECTOR3 PartsPos = {};
//	D3DXMATRIX* PartsMtx = {};
//
//	// �E��
//	if (ppParts[11] != nullptr)
//	{
//		// �}�g���b�N�X�̎擾
//		PartsMtx = ppParts[11]->GetMtxWorld();
//
//		PartsPos.x = PartsMtx->_41;
//		PartsPos.y = PartsMtx->_42;
//		PartsPos.z = PartsMtx->_43;
//	}
//
//	// �U������
//	pEnemy->HitDetection(PartsPos, 50.0f, 50.0f);
//}
//
//CAttackTypeBodyPress::CAttackTypeBodyPress()
//{
//}
//
////===========================================================
//// �U���̎�ށF�t���C���O�v���X
////===========================================================
//void CAttackTypeBodyPress::Update(CEnemy* pEnemy)
//{
//	// �L�����̃p�[�c�擾
//	CCharacter** ppParts = nullptr;
//	ppParts = pEnemy->GetCharcter();
//
//	if (ppParts == nullptr)
//		return;
//
//	D3DXVECTOR3 PartsPos = {};
//	D3DXMATRIX* PartsMtx = {};
//
//	// ��
//	if (ppParts[5] != nullptr)
//	{
//		// �}�g���b�N�X�̎擾
//		PartsMtx = ppParts[5]->GetMtxWorld();
//
//		PartsPos.x = PartsMtx->_41;
//		PartsPos.y = PartsMtx->_42;
//		PartsPos.z = PartsMtx->_43;
//	}
//
//	// �U������
//	pEnemy->HitDetection(PartsPos, 50.0f, 50.0f);
//}
//
////===========================================================
//// �X�e�C�g�̕ύX
////===========================================================
//void CEnemyStateAttack::ChangeState(CAttackType* pAttackType)
//{
//	if (m_pAttackType != nullptr)
//	{
//		delete m_pAttackType;
//		m_pAttackType = nullptr;
//	}
//
//	m_pAttackType = pAttackType;
//}