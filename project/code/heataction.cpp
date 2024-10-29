//===========================================================
//
// �q�[�g�A�N�V����[heataction.cpp]
// Author �匴�叫
//
//===========================================================
#include "heataction.h"
#include "manager.h"
#include "player.h"
#include "enemy.h"
#include "utility.h"
#include "game.h"
#include "itemmanager.h"
#include "collision.h"
#include "motion.h"
#include "MyEffekseer.h"
#include "enemymanager.h"
#include "camera.h"
#include "sound.h"
#include "particle.h"
#include "item.h"
#include "tutorial.h"

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const int MICROWAVE = 3600;        // �q�[�g�A�N�V����:�d�q�����W���ēx���p�\�ɂȂ�܂ł̎���
	const int SHOCK_COUNT_TIMER = 60;  // �q�[�g�A�N�V����:�d�q�����W�œG���т�т肵�����܂ł̃J�E���g
	const int STUN_COUNT_TIMER = 120;  // �q�[�g�A�N�V����:�d�q�����W�œG���C�₷��܂ł̃J�E���g
	const int MICROWAVE_DAMAGE = 100;  // �q�[�g�A�N�V����:�d�q�����W�œG���󂯂�_���[�W
	const int MICROWAVE_SMOOK_RATE = 20;  // �d�q�����W���牌�̃G�t�F�N�g���o��Ԋu

	const D3DXVECTOR3 GAME_CAMERA_ROT[CPlayer::HEAT_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 2.35f, D3DX_PI * -0.38f),
		D3DXVECTOR3(0.0f, D3DX_PI, D3DX_PI * -0.38f),

	};  // �q�[�g�A�N�V�������̃J�����ʒu

	const D3DXVECTOR3 TUTORIAL_CAMERA_ROT = D3DXVECTOR3(0.0f, -1.3f, 0.0f);

	const D3DXVECTOR3 TUTORIALCAMERAROT[CPlayer::HEAT_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 2.35f, D3DX_PI * -0.38f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),

	};  // �q�[�g�A�N�V�������̃J�����ʒu

	const float CAMERA_DISTNCE[CPlayer::HEAT_MAX] =
	{
		300.0f,
		300.0f,
		200.0f,

	};  // �q�[�g�A�N�V�������̃J�����̋���

	const D3DXVECTOR3 ENEMY_GRAP_POS[CEnemy::MAX] =
	{
		D3DXVECTOR3(-10.0f, -10.0f, 60.0f),
		D3DXVECTOR3(-15.0f, -10.0f, 80.0f),
	};

	const D3DXVECTOR3 TUTORIAL_PLAYER_POSITION = { -160.0f, 0.0f, 210.0f };  // �q�[�g�A�N�V����:�`���[�g���A���œd�q�����W���g�p�����ۂ̈ʒu
	const D3DXVECTOR3 GAME_PLAYER_POSITION = { -720.0f, 0.0f, 580.0f };      // �q�[�g�A�N�V����:�Q�[���œd�q�����W���g�p�����ۂ̈ʒu
	const D3DXVECTOR3 ENEMY_POSITION = { 0.0f, -70.0f, -30.0f };             // �q�[�g�A�N�V����:�d�q�����W���g�p�����ۂ̓G�̈ʒu
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CHeatAction::CHeatAction()
{
	m_pPlayer = nullptr;
	m_pEnemy = nullptr;
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CHeatAction::~CHeatAction()
{
}

//===========================================================
// ����������
//===========================================================
void CHeatAction::Init(void)
{
	
}

//===========================================================
// �I������
//===========================================================
void CHeatAction::Uninit(void)
{
	if (m_pPlayer != nullptr)
		m_pPlayer = nullptr;

	if (m_pEnemy != nullptr)
		m_pEnemy = nullptr;
}

//===========================================================
// �X�V����
//===========================================================
void CHeatAction::Updata(void)
{
	// �r�w�C�r�A�̍X�V
	if (m_pState != nullptr)
		m_pState->Update(this);
}

//===========================================================
// �s���A�N�V������ݒ�
//===========================================================
void CHeatAction::SetAction(CHeatActionState* Behaviour, CPlayer* pPlayer, CEnemy* pEnemy)
{
	m_pPlayer = pPlayer;
	m_pEnemy = pEnemy;

	pPlayer->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	pEnemy->SetChase(CEnemy::CHASE_OFF);
	pEnemy->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �q�[�g�A�N�V�����̃J�������[�h�ɂ���
	CManager::GetInstance()->GetCamera()->ChangeState(new HeatActionCamera);
	
	ChangeState(Behaviour);
}

//===========================================================
// �r�w�C�r�A�̐؂�ւ�
//===========================================================
void CHeatAction::ChangeState(CHeatActionState* Behaviour)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = Behaviour;
}

//===========================================================
// ���]�Ԃ��g�����q�[�g�A�N�V���������鏈��
//===========================================================
BikeCrash::BikeCrash()
{
	CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(GAME_CAMERA_ROT[1].x, GAME_CAMERA_ROT[1].y, GAME_CAMERA_ROT[1].z));

	CEnemyManager* pEnemyManager = CEnemyManager::GetInstance();
}

//===========================================================
// ���]�Ԃ��g�����q�[�g�A�N�V���������鏈��
//===========================================================
void BikeCrash::Update(CHeatAction* pHeatAct)
{
	// �v���C���[�擾
	CPlayer* pPlayer = pHeatAct->GetPlayer();

	// �G�擾
	CEnemy* pEnemy = pHeatAct->GetEnemy();

	// �ǂ��炩���g�p����Ă��Ȃ��ꍇ
	if (pEnemy == nullptr || pPlayer == nullptr)
		return;

	// �G���v���C���[�̕����Ɍ�������
	CEnemy::INFO* Info = pEnemy->GetInfo();
	D3DXVECTOR3 ForwardVector = utility::CalculateDirection(Info->pos, pPlayer->GetPosition());
	float angle = atan2f(ForwardVector.x, ForwardVector.z);
	angle -= D3DX_PI;
	angle = utility::CorrectAngle(angle);
	Info->rot.y = angle;
	
	// �v���C���[�̌����擾
	D3DXVECTOR3 PlayerRot = pPlayer->GetRotition();

	// �G�̕����Ɍ�������
	PlayerRot.y += utility::MoveToPosition(pPlayer->GetPosition(), pEnemy->GetPosition(), pPlayer->GetRotition().y);
	PlayerRot.y = utility::CorrectAngle(PlayerRot.y);

	// �v���C���[�̌����ݒ�
	pPlayer->SetRotition(PlayerRot);

	// �����蔻��
	if (pPlayer->GetMotion()->GetAttackOccurs() <= pPlayer->GetMotion()->GetNowFrame()
		&& pPlayer->GetMotion()->GetAttackEnd() >= pPlayer->GetMotion()->GetNowFrame())
	{// ���݂̃t���[�����U�����蔭���t���[���ȏォ�U������I���t���[���ȉ�

		// �G�̔��a
		float EnemyRadius = pEnemy->GetRadius();

		// �G�̍���
		float EnemyHeight = pEnemy->GetHeight();

		CItem* pItem = pPlayer->GetGrapItem();

		if (pItem == nullptr)
			return;

		if (CGame::GetCollision()->ItemEnemy(pItem, pEnemy, EnemyRadius, pItem->GetRadius(), EnemyHeight) == true)
		{
			pEnemy->Damage();

			// �G�̈ʒu�擾
			D3DXVECTOR3 Enempos = pEnemy->GetPosition();

			pPlayer->SetbHeatActFlag(false);

			pPlayer->SetHeatActType(pPlayer->HEAT_NONE);

			CSound* pSound = CManager::GetInstance()->GetSound();

			if (pSound != nullptr)
				pSound->Play(CSound::SOUND_LABEL_SE_ULT_HIT);

			// �G�t�F�N�g�̍Đ�
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_IMPACT, false, D3DXVECTOR3(Enempos.x, Enempos.y, Enempos.z));
		}
	}
	else if (pPlayer->GetMotion()->GetAttackEnd() <= pPlayer->GetMotion()->GetNowFrame())
	{
		// �����Ă����A�C�e��������
		if (CItemManager::GetInstance() != nullptr)
			CItemManager::GetInstance()->Release(pPlayer->GetGrapItem()->GetID());

		// �����Ă����A�C�e��������
		pPlayer->SetGrapItem(nullptr);
	}
}

//===========================================================
// �d�q�����W���g�����q�[�g�A�N�V���������鏈��
//===========================================================
MicroWave::MicroWave()
{
	CManager::GetInstance()->GetCamera()->SetDistnce(CAMERA_DISTNCE[2]);

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TUTORIAL)
	{
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(TUTORIALCAMERAROT[2].x, TUTORIALCAMERAROT[2].y, TUTORIALCAMERAROT[2].z));
	}

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(GAME_CAMERA_ROT[2].x, GAME_CAMERA_ROT[2].y, GAME_CAMERA_ROT[2].z));
	}
}

//===========================================================
// �d�q�����W���g�����q�[�g�A�N�V���������鏈��
//===========================================================
void MicroWave::Update(CHeatAction* pHeatAct)
{
	CPlayer* pPlayer = pHeatAct->GetPlayer();

	CEnemy* pEnemy = pHeatAct->GetEnemy();

	if (m_bInMicroWave == false && pPlayer->GetMotion()->IsFinish() == true)
	{
		// �v���C���[�̌����擾
		D3DXVECTOR3 PlayerRot = pPlayer->GetRotition();

		// �d�q�����W�̕����Ɍ�������
		PlayerRot.y += utility::MoveToPosition(pPlayer->GetPosition(), pEnemy->GetPosition(), pPlayer->GetRotition().y);
		PlayerRot.y = utility::CorrectAngle(PlayerRot.y);

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TUTORIAL)
		{
			pPlayer->SetPosition(TUTORIAL_PLAYER_POSITION);
		}

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{
			pPlayer->SetPosition(GAME_PLAYER_POSITION);
		}

		// �v���C���[�Ƃ̊֌W��؂�
		{
			pPlayer->SetImmobile();

			pEnemy->SetCurrent(nullptr);
			pEnemy->SetRotition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			// �v���C���[�̃��[�V�������g�p����Ă��Ȃ������ꍇ�����𔲂���
			if (pPlayer->GetMotion() == nullptr)
				return;

			pPlayer->GetMotion()->Set(CPlayer::TYPE_NEUTRAL);
			pPlayer->SetState(CPlayer::STATE_NEUTRAL);
		}

		// �d�q�����W�Ƃ̊֌W�����
		{
			// �A�C�e���F�d�q�����W���Ȃ������ꍇ����������
			if (pPlayer->GetItem() == nullptr)
				return;

			pEnemy->SetCurrent(pPlayer->GetItem()->GetMtxWorld());
			pEnemy->SetPosition(ENEMY_POSITION);
			pEnemy->SetRotition(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

			// ��Ԃƃ��[�V������ݒ�
			if (pEnemy->GetState() != CEnemy::STATE_HEATACTELECTROWAIT)
			{
				pEnemy->SetState(CEnemy::STATE_HEATACTELECTROWAIT);

				// �G�̃��[�V�������g�p����Ă��Ȃ������ꍇ�����𔲂���
				if (pEnemy->GetMotion() == nullptr)
					return;

				// �q�[�g�A�N�V����:�d�q�����W�ɓ��������đ҂��Ă�����
				pEnemy->GetMotion()->Set(CEnemy::MOTION_HEATACTELECTROWAIT);
				CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_GRASS);

				D3DXVECTOR3 pos = pPlayer->GetItem()->GetPosition();
				// �K���X�Ђ̃p�[�e�B�N���𐶐�
				CParticle::Create(pos, CParticle::TYPE_GLASS);

				// �d�q�����W�̒��ɓ�����
				m_bInMicroWave = true;
			}
		}
	}

	if (m_bInMicroWave == true)
	{
		m_nHeatActTime++;
	}

	if (pEnemy->GetState() == CEnemy::STATE_HEATACTELECTROWAIT)
	{
		if (m_nHeatActTime > SHOCK_COUNT_TIMER)
		{
			if (pEnemy->GetState() != CEnemy::STATE_HEATACTELECTRO)
			{
				pEnemy->SetState(CEnemy::STATE_HEATACTELECTRO);
				pEnemy->GetMotion()->Set(CEnemy::MOTION_HEATACTELECTRO);
				CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_FIRE);

				if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TUTORIAL)
				{
					CManager::GetInstance()->GetCamera()->SetRotation(TUTORIAL_CAMERA_ROT);
				}

				if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
				{
					CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(GAME_CAMERA_ROT[0].x, GAME_CAMERA_ROT[0].y, GAME_CAMERA_ROT[0].z));
				}

			}

			m_nHeatActTime = 0;
		}
	}

	if (m_nHeatActTime > STUN_COUNT_TIMER && pEnemy->GetState() == CEnemy::STATE_HEATACTELECTRO)
	{
		if (pEnemy->GetState() != CEnemy::STATE_HEATACTFAINTING)
		{
			pEnemy->SetState(CEnemy::STATE_HEATACTFAINTING);
			pEnemy->GetMotion()->Set(CEnemy::MOTION_HEATACTFAINTING);
			pEnemy->GetInfo()->nLife -= MICROWAVE_DAMAGE;
			CManager::GetInstance()->GetCamera()->ChangeState(new ReturnPlayerBehindCamera);

			if (pEnemy->GetInfo()->nLife > 0)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				pPlayer->SetUseMicroCount(3600);
				CGame::GetEnemyManager()->SetTrue(CPlayer::GetInstance()->GetGrapEnemy()->GetIdxID());
				pPlayer->SetGrapEnemy(nullptr);
				pEnemy->SetPosition(D3DXVECTOR3(CPlayer::GetInstance()->GetItem()->GetPosition().x, 0.0f, CPlayer::GetInstance()->GetItem()->GetPosition().z));
				pEnemy->SetCurrent(nullptr);
				pPlayer->SetbHeatActFlag(false);
				pEnemy->Damage();

				pPlayer->SetHeatActType(pPlayer->HEAT_NONE);
			}
			else if (pEnemy->GetInfo()->nLife <= 0)
			{
				pPlayer->SetState(CPlayer::STATE_NEUTRAL);
				pPlayer->SetUseMicroCount(3600);
				CGame::GetEnemyManager()->SetTrue(CPlayer::GetInstance()->GetGrapEnemy()->GetIdxID());
				pPlayer->SetGrapEnemy(nullptr);
				pPlayer->SetbHeatActFlag(false);
				pEnemy->Damage();

				pPlayer->SetHeatActType(pPlayer->HEAT_NONE);
			}
		}

		m_nHeatActTime = 0;
	}

	if (pEnemy->GetState() == CEnemy::STATE_HEATACTELECTRO)
	{
		if (m_nHeatActTime % MICROWAVE_SMOOK_RATE == 0)
		{
			CParticle::Create(CPlayer::GetInstance()->GetItem()->GetPosition(), CParticle::TYPE_BREAKDOWN);
		}
	}
}