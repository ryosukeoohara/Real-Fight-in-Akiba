//===========================================================
//
// ヒートアクション[heataction.cpp]
// Author 大原怜将
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
// 定数定義
//===========================================================
namespace
{
	const int MICROWAVE = 3600;        // ヒートアクション:電子レンジが再度利用可能になるまでの時間
	const int SHOCK_COUNT_TIMER = 60;  // ヒートアクション:電子レンジで敵がびりびりしだすまでのカウント
	const int STUN_COUNT_TIMER = 120;  // ヒートアクション:電子レンジで敵が気絶するまでのカウント
	const int MICROWAVE_DAMAGE = 100;  // ヒートアクション:電子レンジで敵が受けるダメージ
	const int MICROWAVE_SMOOK_RATE = 20;  // 電子レンジから煙のエフェクトが出る間隔

	const D3DXVECTOR3 GAME_CAMERA_ROT[CPlayer::HEAT_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 2.35f, D3DX_PI * -0.38f),
		D3DXVECTOR3(0.0f, D3DX_PI, D3DX_PI * -0.38f),

	};  // ヒートアクション時のカメラ位置

	const D3DXVECTOR3 TUTORIAL_CAMERA_ROT = D3DXVECTOR3(0.0f, -1.3f, 0.0f);

	const D3DXVECTOR3 TUTORIALCAMERAROT[CPlayer::HEAT_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 2.35f, D3DX_PI * -0.38f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),

	};  // ヒートアクション時のカメラ位置

	const float CAMERA_DISTNCE[CPlayer::HEAT_MAX] =
	{
		300.0f,
		300.0f,
		200.0f,

	};  // ヒートアクション時のカメラの距離

	const D3DXVECTOR3 ENEMY_GRAP_POS[CEnemy::MAX] =
	{
		D3DXVECTOR3(-10.0f, -10.0f, 60.0f),
		D3DXVECTOR3(-15.0f, -10.0f, 80.0f),
	};

	const D3DXVECTOR3 TUTORIAL_PLAYER_POSITION = { -160.0f, 0.0f, 210.0f };  // ヒートアクション:チュートリアルで電子レンジを使用した際の位置
	const D3DXVECTOR3 GAME_PLAYER_POSITION = { -720.0f, 0.0f, 580.0f };      // ヒートアクション:ゲームで電子レンジを使用した際の位置
	const D3DXVECTOR3 ENEMY_POSITION = { 0.0f, -70.0f, -30.0f };             // ヒートアクション:電子レンジを使用した際の敵の位置
}

//===========================================================
// コンストラクタ
//===========================================================
CHeatAction::CHeatAction()
{
	m_pPlayer = nullptr;
	m_pEnemy = nullptr;
}

//===========================================================
// デストラクタ
//===========================================================
CHeatAction::~CHeatAction()
{
}

//===========================================================
// 初期化処理
//===========================================================
void CHeatAction::Init(void)
{
	
}

//===========================================================
// 終了処理
//===========================================================
void CHeatAction::Uninit(void)
{
	if (m_pPlayer != nullptr)
		m_pPlayer = nullptr;

	if (m_pEnemy != nullptr)
		m_pEnemy = nullptr;
}

//===========================================================
// 更新処理
//===========================================================
void CHeatAction::Updata(void)
{
	// ビヘイビアの更新
	if (m_pState != nullptr)
		m_pState->Update(this);
}

//===========================================================
// 行うアクションを設定
//===========================================================
void CHeatAction::SetAction(CHeatActionState* Behaviour, CPlayer* pPlayer, CEnemy* pEnemy)
{
	m_pPlayer = pPlayer;
	m_pEnemy = pEnemy;

	pPlayer->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	pEnemy->SetChase(CEnemy::CHASE_OFF);
	pEnemy->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ヒートアクションのカメラモードにする
	CManager::GetInstance()->GetCamera()->ChangeState(new HeatActionCamera);
	
	ChangeState(Behaviour);
}

//===========================================================
// ビヘイビアの切り替え
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
// 自転車を使ったヒートアクションをする処理
//===========================================================
BikeCrash::BikeCrash()
{
	CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(GAME_CAMERA_ROT[1].x, GAME_CAMERA_ROT[1].y, GAME_CAMERA_ROT[1].z));

	CEnemyManager* pEnemyManager = CEnemyManager::GetInstance();
}

//===========================================================
// 自転車を使ったヒートアクションをする処理
//===========================================================
void BikeCrash::Update(CHeatAction* pHeatAct)
{
	// プレイヤー取得
	CPlayer* pPlayer = pHeatAct->GetPlayer();

	// 敵取得
	CEnemy* pEnemy = pHeatAct->GetEnemy();

	// どちらかが使用されていない場合
	if (pEnemy == nullptr || pPlayer == nullptr)
		return;

	// 敵をプレイヤーの方向に向かせる
	CEnemy::INFO* Info = pEnemy->GetInfo();
	D3DXVECTOR3 ForwardVector = utility::CalculateDirection(Info->pos, pPlayer->GetPosition());
	float angle = atan2f(ForwardVector.x, ForwardVector.z);
	angle -= D3DX_PI;
	angle = utility::CorrectAngle(angle);
	Info->rot.y = angle;
	
	// プレイヤーの向き取得
	D3DXVECTOR3 PlayerRot = pPlayer->GetRotition();

	// 敵の方向に向かせる
	PlayerRot.y += utility::MoveToPosition(pPlayer->GetPosition(), pEnemy->GetPosition(), pPlayer->GetRotition().y);
	PlayerRot.y = utility::CorrectAngle(PlayerRot.y);

	// プレイヤーの向き設定
	pPlayer->SetRotition(PlayerRot);

	// 当たり判定
	if (pPlayer->GetMotion()->GetAttackOccurs() <= pPlayer->GetMotion()->GetNowFrame()
		&& pPlayer->GetMotion()->GetAttackEnd() >= pPlayer->GetMotion()->GetNowFrame())
	{// 現在のフレームが攻撃判定発生フレーム以上かつ攻撃判定終了フレーム以下

		// 敵の半径
		float EnemyRadius = pEnemy->GetRadius();

		// 敵の高さ
		float EnemyHeight = pEnemy->GetHeight();

		CItem* pItem = pPlayer->GetGrapItem();

		if (pItem == nullptr)
			return;

		if (CGame::GetCollision()->ItemEnemy(pItem, pEnemy, EnemyRadius, pItem->GetRadius(), EnemyHeight) == true)
		{
			pEnemy->Damage();

			// 敵の位置取得
			D3DXVECTOR3 Enempos = pEnemy->GetPosition();

			pPlayer->SetbHeatActFlag(false);

			pPlayer->SetHeatActType(pPlayer->HEAT_NONE);

			CSound* pSound = CManager::GetInstance()->GetSound();

			if (pSound != nullptr)
				pSound->Play(CSound::SOUND_LABEL_SE_ULT_HIT);

			// エフェクトの再生
			MyEffekseer::EffectCreate(CMyEffekseer::TYPE_IMPACT, false, D3DXVECTOR3(Enempos.x, Enempos.y, Enempos.z));
		}
	}
	else if (pPlayer->GetMotion()->GetAttackEnd() <= pPlayer->GetMotion()->GetNowFrame())
	{
		// 持っていたアイテムを消す
		if (CItemManager::GetInstance() != nullptr)
			CItemManager::GetInstance()->Release(pPlayer->GetGrapItem()->GetID());

		// 持っていたアイテムを消す
		pPlayer->SetGrapItem(nullptr);
	}
}

//===========================================================
// 電子レンジを使ったヒートアクションをする処理
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
// 電子レンジを使ったヒートアクションをする処理
//===========================================================
void MicroWave::Update(CHeatAction* pHeatAct)
{
	CPlayer* pPlayer = pHeatAct->GetPlayer();

	CEnemy* pEnemy = pHeatAct->GetEnemy();

	if (m_bInMicroWave == false && pPlayer->GetMotion()->IsFinish() == true)
	{
		// プレイヤーの向き取得
		D3DXVECTOR3 PlayerRot = pPlayer->GetRotition();

		// 電子レンジの方向に向かせる
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

		// プレイヤーとの関係を切る
		{
			pPlayer->SetImmobile();

			pEnemy->SetCurrent(nullptr);
			pEnemy->SetRotition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			// プレイヤーのモーションが使用されていなかった場合処理を抜ける
			if (pPlayer->GetMotion() == nullptr)
				return;

			pPlayer->GetMotion()->Set(CPlayer::TYPE_NEUTRAL);
			pPlayer->SetState(CPlayer::STATE_NEUTRAL);
		}

		// 電子レンジとの関係を作る
		{
			// アイテム：電子レンジがなかった場合処理抜ける
			if (pPlayer->GetItem() == nullptr)
				return;

			pEnemy->SetCurrent(pPlayer->GetItem()->GetMtxWorld());
			pEnemy->SetPosition(ENEMY_POSITION);
			pEnemy->SetRotition(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

			// 状態とモーションを設定
			if (pEnemy->GetState() != CEnemy::STATE_HEATACTELECTROWAIT)
			{
				pEnemy->SetState(CEnemy::STATE_HEATACTELECTROWAIT);

				// 敵のモーションが使用されていなかった場合処理を抜ける
				if (pEnemy->GetMotion() == nullptr)
					return;

				// ヒートアクション:電子レンジに頭を入れられて待っている状態
				pEnemy->GetMotion()->Set(CEnemy::MOTION_HEATACTELECTROWAIT);
				CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_GRASS);

				D3DXVECTOR3 pos = pPlayer->GetItem()->GetPosition();
				// ガラス片のパーティクルを生成
				CParticle::Create(pos, CParticle::TYPE_GLASS);

				// 電子レンジの中に入った
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