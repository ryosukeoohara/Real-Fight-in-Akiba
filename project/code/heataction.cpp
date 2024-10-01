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

	const D3DXVECTOR3 CAMERA_ROT[CPlayer::HEAT_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 2.35f, D3DX_PI * -0.38f),
		D3DXVECTOR3(0.0f, D3DX_PI, D3DX_PI * -0.38f),

	};  // �q�[�g�A�N�V�������̃J�����ʒu

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

CHeatAction::CHeatAction()
{
}

CHeatAction::~CHeatAction()
{
}

//===========================================================
// ����������
//===========================================================
void CHeatAction::Init(void)
{
	m_bInMicroWave = false;
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
	CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(CAMERA_ROT[1].x, CAMERA_ROT[1].y, CAMERA_ROT[1].z));

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

		if (CGame::GetCollision()->ItemEnemy(pPlayer->GetGrapItem(), pEnemy, 50.0f, 50.0f, 100.0f) == true)
		{
			// �����Ă����A�C�e��������
			if (CItemManager::GetInstance() != nullptr)
				CItemManager::GetInstance()->Release(pPlayer->GetGrapItem()->GetID());

			pEnemy->Damege();

			// �����Ă����A�C�e��������
			pPlayer->SetGrapItem(nullptr);

			// �G�̈ʒu�擾
			D3DXVECTOR3 Enempos = pEnemy->GetPosition();

			pPlayer->SetbHeatActFlag(false);

			// �G�t�F�N�g�̍Đ�
			CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_IMPACT, ::Effekseer::Vector3D(Enempos.x, Enempos.y, Enempos.z));
		}

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
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(CAMERA_ROT[2].x, CAMERA_ROT[2].y, CAMERA_ROT[2].z));
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

				// �K���X�Ђ̃p�[�e�B�N���𐶐�
				CParticle::Create(pPlayer->GetItem()->GetPosition(), CParticle::TYPE_GLASS);

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
					CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(0.0f, -1.3f, 0.0f));
				}

				if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
				{
					CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(CAMERA_ROT[0].x, CAMERA_ROT[0].y, CAMERA_ROT[0].z));
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
			pEnemy->Damege();
			CManager::GetInstance()->GetCamera()->ChangeState(new ReturnPlayerBehindCamera);
			pPlayer->SetState(CPlayer::STATE_NEUTRAL);
			pPlayer->SetUseMicroCount(3600);
			CGame::GetEnemyManager()->SetTrue(CPlayer::GetInstance()->GetGrapEnemy()->GetIdxID());
			CPlayer::GetInstance()->SetGrapEnemy(nullptr);
			pEnemy->SetPosition(D3DXVECTOR3(CPlayer::GetInstance()->GetItem()->GetPosition().x, 0.0f, CPlayer::GetInstance()->GetItem()->GetPosition().z));
			pEnemy->SetCurrent(nullptr);
			pPlayer->SetbHeatActFlag(false);
		}

		m_nHeatActTime = 0;
	}

	if (pEnemy->GetState() == CEnemy::STATE_HEATACTELECTRO)
	{
		if (m_nHeatActTime % 20 == 0)
		{
			CParticle::Create(CPlayer::GetInstance()->GetItem()->GetPosition(), CParticle::TYPE_SMOOK);
		}
	}
}