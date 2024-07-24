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
#include <assert.h>

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const int DAMEGECOUNT = 10;  // �_���[�W���
	const float ATTACKLENGTH = 120.0f;  // �U���\�ȋ���

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
// ���䏈��
//===========================================================
void CEnemyBoss::Attack(void)
{
	if (m_Info.state != STATE_ATTACK)
	{
		m_nAtcCounter++;

		if (m_nAtcCounter >= 60)
		{
			m_nAtcCounter = 0;

			m_nAttackType = rand() % ATTACKTYPE_MAX;  //�U���̎�ޒ��I

			switch (m_nAttackType)
			{
			case ATTACKTYPE_GURUGURU:  // ���邮��p���`

				m_Info.state = STATE_ATTACK;
				GetMotion()->Set(TYPE_GURUGURU);
				RollingPunch();

				break;

			case ATTACKTYPE_PUNCH:  // ���ʂ̃p���`

				m_Info.state = STATE_ATTACK;
				GetMotion()->Set(TYPE_PUNCH);
				NormalPunch();

				break;

			case ATTACKTYPE_FLY:  // ���ł��Ȃ����痎����

				m_Info.state = STATE_ATTACK;
				GetMotion()->Set(TYPE_ATTACK);
				Fly();

				break;

			default:
				break;
			}
		}
	}
}

//===========================================================
// ���䏈��
//===========================================================
void CEnemyBoss::Move(void)
{
	//�v���C���[�̏��擾
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	float fDiffmove = 0.0f;

	m_Info.posOld = m_Info.pos;

	if (m_Info.state != STATE_PAINFULDAMAGE && m_Chase == CHASE_ON)
	{
		if (m_Info.state != STATE_ATTACK)
		{
			fDiffmove = utility::MoveToPosition(m_Info.pos, PlayerPos, m_Info.rot.y);

			fDiffmove = utility::CorrectAngle(fDiffmove);

			m_Info.rot.y += fDiffmove * 0.05f;

			m_Info.rot.y = utility::CorrectAngle(m_Info.rot.y);
		}

		//�ړ��ʂ��X�V(����������)
		m_Info.move.x = sinf(m_Info.rot.y + D3DX_PI) * 2.0f;
		m_Info.move.z = cosf(m_Info.rot.y + D3DX_PI) * 2.0f;

		float fLength = utility::Distance(m_Info.pos, PlayerPos);

		if (fLength <= ATTACKLENGTH)
		{
			if (m_Info.state != STATE_NEUTRAL && m_Info.state != STATE_ATTACK && m_Info.state != STATE_DAMEGE && m_Info.state != STATE_PAINFULDAMAGE)
			{
				m_Info.state = STATE_NEUTRAL;
				GetMotion()->Set(TYPE_NEUTRAL);
			}

			Attack();

			m_Info.move.x = 0.0f;
			m_Info.move.z = 0.0f;
		}
		else
		{
			if (m_Info.state != STATE_DASH && m_Info.state != STATE_ATTACK && m_Info.state != STATE_DAMEGE && m_Info.state != STATE_PAINFULDAMAGE)
			{
				m_Info.state = STATE_DASH;
				GetMotion()->Set(TYPE_DASH);
			}
		}
	}
	
	if (GetMotion()->IsFinish() == true && (m_Info.state == STATE_HEATDAMEGE || m_Info.state == STATE_PAINFULDAMAGE) && m_Info.state != STATE_GETUP)
	{
		m_Info.state = STATE_GETUP;
		GetMotion()->Set(TYPE_GETUP);
	}
	else if (GetMotion()->IsFinish() == true)
	{
		m_Info.state = STATE_NEUTRAL;
		GetMotion()->Set(TYPE_NEUTRAL);

		if (m_Chase != CHASE_ON)
		{
			m_Chase = CHASE_ON;

			m_nAtcCounter = 0;
		}
	}
}

//===========================================================
// �_���[�W����
//===========================================================
void CEnemyBoss::Damege(int damege, float blowaway, CPlayer::ATTACKTYPE act)
{
	if (m_Info.state != STATE_DAMEGE && m_Info.state != STATE_HEATDAMEGE && m_Info.state != STATE_PAINFULDAMAGE 
	 && m_Info.state != STATE_DEATH && (m_Info.state != STATE_ATTACK || CGame::GetPlayer()->GetState() == CPlayer::STATE_HEAT))
	{
		m_Info.nLife -= damege;

		m_Info.move = D3DXVECTOR3(sinf(CGame::GetPlayer()->GetRotition().y) * -blowaway, blowaway, cosf(CGame::GetPlayer()->GetRotition().y) * -blowaway);

		if (act == CPlayer::ATTACKTYPE::TYPE_HEATACTBIKE || act == CPlayer::ATTACKTYPE::TYPE_HEATACTREF || act == CPlayer::ATTACKTYPE::TYPE_HEATACTMICROWAVE)
		{
			if (m_Info.state != STATE_HEATDAMEGE)
			{
				m_Info.state = STATE_HEATDAMEGE;
				GetMotion()->Set(TYPE_HEATACTDAMEGE);
			}
		}
		else
		{
			if (m_Info.state != STATE_DAMEGE)
			{
				int a = rand() % 50;
				int n = m_Info.nLife / 3;
				if (a >= n && CGame::GetPlayer()->GetActType() == CPlayer::TYPE_ATTACK3)
				{
					m_Info.state = STATE_PAINFULDAMAGE;
					GetMotion()->Set(TYPE_HEATACTDAMEGE);
					m_Chase = CHASE_OFF;
					m_Info.move.x = 0.0f;
					m_Info.move.z = 0.0f;
				}
				else
				{
					m_Info.state = STATE_DAMEGE;
					GetMotion()->Set(TYPE_DAMEGE);
				}
			}
		}

		CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_PUNCH);
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
	if (GetMotion()->GetAttackOccurs() <= GetMotion()->GetNowFrame() && GetMotion()->GetAttackEnd() >= GetMotion()->GetNowFrame())
	{// ���݂̃t���[�����U�����蔭���t���[���ȏォ�U������I���t���[���Ȃ�

		if (CGame::GetCollision()->Circle(MyPos, CGame::GetPlayer()->GetPosition(), attackrange, targetradius) == true)
			CGame::GetPlayer()->Damage(GetMotion()->GetAttackDamege(), GetMotion()->GetKnockBack());
	}
}

//===========================================================
// ����������
//===========================================================
HRESULT CEnemyBoss::Init(void)
{
	CEnemy::Init();
	SetType(BOSS);
	ReadText(ENEMY_TEXT);

	if (m_pLife2D == nullptr)
	{
		m_pLife2D = CGage2D::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.25f) + 4.0f, 650.0f, 0.0f), 40.0f, (float)m_Info.nLife * 2.0f, CGage2D::TYPE_LIFE);
		m_pLife2D->GetObj2D()->SetEdgeCenterTex((float)(m_Info.nLife * 0.1f));
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

	if (m_pLife2D != nullptr)
		m_pLife2D->GetObj2D()->SetEdgeCenterTex((float)((m_Info.nLife * 0.1f) * 20.0f));
}

//===========================================================
// �`�揈��
//===========================================================
void CEnemyBoss::Draw(void)
{
	CEnemy::Draw();
}