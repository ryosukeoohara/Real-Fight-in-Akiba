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

// 無名名前空間を定義
namespace
{
	const int MICROWAVE = 3600;  // ヒートアクション:電子レンジが再度利用可能になるまでの時間
	const int MICRO = 60;


	const D3DXVECTOR3 CAMERA_ROT[CPlayer::HEAT_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 2.35f, D3DX_PI * -0.38f),
		D3DXVECTOR3(0.0f, D3DX_PI, D3DX_PI * -0.38f),

	};  // ヒートアクション時のカメラ位置

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
}

//===========================================================
// 初期化処理
//===========================================================
void CHeatAction::Init(void)
{
	m_bInMicroWave = false;
}

//===========================================================
// 終了処理
//===========================================================
void CHeatAction::Uninit(void)
{

}

//===========================================================
// 更新処理
//===========================================================
void CHeatAction::Updata(void)
{
	switch (m_pPlayer->GetHeatAct())
	{
	case CPlayer::HEAT_NONE:  // なんもない
		break;
	case CPlayer::HEAT_SMASH: // 自転車たたきつけ

		BikeCrash(m_pPlayer, m_pEnemy);
		break;
	case CPlayer::HEAT_FIRE:  // 電子レンジ

		MicroWave(m_pPlayer, m_pEnemy, nullptr);
		break;
	case CPlayer::HEAT_MAX:
		break;
	default:
		break;
	}
}

//===========================================================
// 自転車を使ったヒートアクションをする処理
//===========================================================
void CHeatAction::BikeCrash(CPlayer* pPlayer, CEnemy* pEnemy)
{
	if (pEnemy != nullptr)
	{
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

			if (CGame::GetCollision()->ItemEnemy(pPlayer->GetGrapItem(), pEnemy, 50.0f, 50.0f, 100.0f) == true)
			{// 範囲内

				if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TUTORIAL)
				{
					// 持っていたアイテムを消す
					if (CTutorial::GetItemManager())
						CTutorial::GetItemManager()->Release(pPlayer->GetGrapItem()->GetID());

					// ターゲット以外の敵の描画を再開
					if (CTutorial::GetEnemyManager())
						CTutorial::GetEnemyManager()->SetTrue(pEnemy->GetIdxID());
				}

				if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
				{
					// 持っていたアイテムを消す
					if (CGame::GetItemManager())
						CGame::GetItemManager()->Release(pPlayer->GetGrapItem()->GetID());

					// ターゲット以外の敵の描画を再開
					if (CGame::GetEnemyManager() )
						CGame::GetEnemyManager()->SetTrue(pEnemy->GetIdxID());
				}
				

				if (pEnemy->GetMotion())
					pEnemy->GetMotion()->Set(CEnemy::TYPE_HEATACTDAMEGE);
				
				// 持っていたアイテムを消す
				pPlayer->SetGrapItem(nullptr);

				// 敵の位置取得
				D3DXVECTOR3 Enempos = pEnemy->GetPosition();

				pPlayer->SetbHeatActFlag(false);

				// エフェクトの再生
				CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::m_apEfkName[CMyEffekseer::TYPE_IMPACT], ::Effekseer::Vector3D(Enempos.x, Enempos.y, Enempos.z),
					::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(25.0f, 25.0f, 25.0f));
			}
		}
	}
}

//===========================================================
// 電子レンジを使ったヒートアクションをする処理
//===========================================================
void CHeatAction::MicroWave(CPlayer* pPlayer, CEnemy* pEnemy, CItem* pItem)
{
	if (m_bInMicroWave == false && pPlayer->GetMotion()->IsFinish() == true)
	{
		// プレイヤーの向き取得
		D3DXVECTOR3 PlayerRot = pPlayer->GetRotition();

		// 電子レンジの方向に向かせる
		PlayerRot.y += utility::MoveToPosition(pPlayer->GetPosition(), pEnemy->GetPosition(), pPlayer->GetRotition().y);
		PlayerRot.y = utility::CorrectAngle(PlayerRot.y);
		
		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TUTORIAL)
		{
			pPlayer->SetPosition(D3DXVECTOR3(-160.0f, 0.0f, 210.0f));
		}

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{
			pPlayer->SetPosition(D3DXVECTOR3(-720.0f, 0.0f, 580.0f));
		}

		// プレイヤーとの関係を切る
		{
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
			pEnemy->SetPosition(D3DXVECTOR3(0.0f, -70.0f, -30.0f));
			pEnemy->SetRotition(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

			// 状態とモーションを設定
			if (pEnemy->GetState() != CEnemy::STATE_HEATACTELECTROWAIT)
			{
				pEnemy->SetState(CEnemy::STATE_HEATACTELECTROWAIT);

				// 敵のモーションが使用されていなかった場合処理を抜ける
				if (pEnemy->GetMotion() == nullptr)
					return;

				// ヒートアクション:電子レンジに頭を入れられて待っている状態
				pEnemy->GetMotion()->Set(CEnemy::TYPE_HEATACTELECTROWAIT);
				CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_GRASS);

				// ガラス片のパーティクルを生成
				CParticle::Create(pPlayer->GetItem()->GetPosition(), CParticle::TYPE_GLASS);

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
		if (m_nHeatActTime > 60)
		{
			if (pEnemy->GetState() != CEnemy::STATE_HEATACTELECTRO)
			{
				pEnemy->SetState(CEnemy::STATE_HEATACTELECTRO);
				pEnemy->GetMotion()->Set(CEnemy::TYPE_HEATACTELECTRO);
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

	if (m_nHeatActTime > 120 && pEnemy->GetState() == CEnemy::STATE_HEATACTELECTRO)
	{
		if (pEnemy->GetState() != CEnemy::STATE_HEATACTFAINTING)
		{
			pEnemy->SetState(CEnemy::STATE_HEATACTFAINTING);
			pEnemy->GetMotion()->Set(CEnemy::TYPE_HEATACTFAINTING);
			pEnemy->Damege(100, 0.0f, CPlayer::TYPE_HEATACTMICROWAVE);
			CManager::GetInstance()->GetCamera()->SetMode(CCamera::MODE_RETURN);
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

//===========================================================
// 行うアクションを設定
//===========================================================
void CHeatAction::SetAction(CPlayer::HEAT heatact, CPlayer* pPlayer, CEnemy* pEnemy)
{
	m_pPlayer = pPlayer;
	m_pEnemy = pEnemy;

	pPlayer->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	pEnemy->SetChase(CEnemy::CHASE_OFF);
	//pEnemy->SetDraw()
	pEnemy->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ヒートアクションのカメラモードにする
	CManager::GetInstance()->GetCamera()->SetMode(CCamera::MODE_HEAT);
	
	CManager::GetInstance()->GetCamera()->SetDistnce(CAMERA_DISTNCE[heatact]);

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TUTORIAL)
	{
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(TUTORIALCAMERAROT[heatact].x, TUTORIALCAMERAROT[heatact].y, TUTORIALCAMERAROT[heatact].z));
	}

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(CAMERA_ROT[heatact].x, CAMERA_ROT[heatact].y, CAMERA_ROT[heatact].z));
	}

	switch (heatact)
	{
	case CPlayer::HEAT_NONE:  // なんもない
		break;
	case CPlayer::HEAT_SMASH: // 自転車たたきつけ

		if(pPlayer->GetMotion())
		pPlayer->GetMotion()->Set(CPlayer::TYPE_THROW);

		//BikeCrash(pPlayer, pEnemy);
		break;
	case CPlayer::HEAT_FIRE:  // 電子レンジ

		//MicroWave(pPlayer, pEnemy, nullptr);
		break;
	case CPlayer::HEAT_MAX:
		break;
	default:
		break;
	}
}