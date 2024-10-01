//===========================================================
//
// ボスの処理 [enemy_boss.cpp]
// Author : 大原　怜将
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
// 定数定義
//===========================================================
namespace
{
	const int DAMEGECOUNT = 10;         // ダメージ状態
	const float ATTACKLENGTH = 120.0f;  // 攻撃可能な距離
	const float SPEED = 2.0f;           // 走る速さ
	const float ATTACKLENGHT = 50.0f;   // 攻撃可能範囲

	const D3DXVECTOR3 CAMERA_ROT[CPlayer::HEAT_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 2.35f, D3DX_PI * -0.38f),
		D3DXVECTOR3(0.0f, D3DX_PI * 0.15f, D3DX_PI * -0.38f),

	};  // ヒートアクション時のカメラ位置

	const char *ENEMY_TEXT = "data\\TEXT\\motion_boss.txt";
}

//===========================================================
// コンストラクタ
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
// コンストラクタ
//===========================================================
CEnemyBoss::CEnemyBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife)
{
	// 値をクリア
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
// デストラクタ
//===========================================================
CEnemyBoss::~CEnemyBoss()
{

}

//===========================================================
// 生成処理
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
// ステイトの切り替え
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
	//// 敵の情報取得
	//CEnemy::INFO* Info = GetInfo();

	//if (Info == nullptr)
	//	return;

	//CMotion* pMotion = GetMotion();

	//if (pMotion == nullptr)
	//	return;

	//CPlayer* pPlayer = CPlayer::GetInstance();

	//if (pPlayer == nullptr)
	//	return;

	//// モーションが終了していたら
	//if (pMotion->IsFinish())
	//{
	//	Info->state = pEnemyWeak->STATE_NEUTRAL;
	//	pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
	//	pEnemyWeak->ChangeState(new CEnemyWeakStateMoveWait);
	//}

	//// モーションが終了していたら
	//if (pMotion->IsFinish())
	//{
	//	Info->state = CEnemy::STATE_NEUTRAL;
	//	pMotion->Set(MOTION_NEUTRAL);
	//	ChangeState(new CEnemyBossStateMove);
	//}
}

//===========================================================
// ダメージ処理
//===========================================================
void CEnemyBoss::Damege(void)
{
	ChangeState(new CEnemyBossStateDamege);
}

//===========================================================
// ぐるぐるパンチ攻撃
//===========================================================
void CEnemyBoss::RollingPunch(void)
{
	// キャラのパーツ取得
	CCharacter **ppParts = nullptr;
	ppParts = GetCharcter();

	D3DXVECTOR3 PartsPosR = {};
	D3DXMATRIX *PartsMtxR = {};
	D3DXVECTOR3 PartsPosL = {};
	D3DXMATRIX *PartsMtxL = {};

	// 左手
	if (ppParts[8] != nullptr)
	{
		// マトリックスの取得
		PartsMtxR = ppParts[8]->GetMtxWorld();

		PartsPosR.x = PartsMtxR->_41;
		PartsPosR.y = PartsMtxR->_42;
		PartsPosR.z = PartsMtxR->_43;
	}

	// 右手
	if (ppParts[11] != nullptr)
	{
		// マトリックスの取得
		PartsMtxL = ppParts[11]->GetMtxWorld();

		PartsPosL.x = PartsMtxL->_41;
		PartsPosL.y = PartsMtxL->_42;
		PartsPosL.z = PartsMtxL->_43;
	}

	// 攻撃判定
	HitDetection(PartsPosR, 20.0f, 50.0f);

	// 攻撃判定
	HitDetection(PartsPosL, 20.0f, 50.0f);
}

//===========================================================
// 普通のパンチ
//===========================================================
void CEnemyBoss::NormalPunch(void)
{
	// キャラのパーツ取得
	CCharacter **ppParts = nullptr;
	ppParts = GetCharcter();

	D3DXVECTOR3 PartsPos = {};
	D3DXMATRIX *PartsMtx = {};

	// 右手
	if (ppParts[11] != nullptr)
	{
		// マトリックスの取得
		PartsMtx = ppParts[11]->GetMtxWorld();

		PartsPos.x = PartsMtx->_41;
		PartsPos.y = PartsMtx->_42;
		PartsPos.z = PartsMtx->_43;
	}

	// 攻撃判定
	HitDetection(PartsPos, 50.0f, 50.0f);
}

//===========================================================
// 飛び込み攻撃
//===========================================================
void CEnemyBoss::Fly(void)
{
	// キャラのパーツ取得
	CCharacter **ppParts = nullptr;
	ppParts = GetCharcter();

	D3DXVECTOR3 PartsPos = {};
	D3DXMATRIX *PartsMtx = {};

	// 腹
	if (ppParts[5] != nullptr)
	{
		// マトリックスの取得
		PartsMtx = ppParts[5]->GetMtxWorld();

		PartsPos.x = PartsMtx->_41;
		PartsPos.y = PartsMtx->_42;
		PartsPos.z = PartsMtx->_43;
	}

	// 攻撃判定
	HitDetection(PartsPos, 50.0f, 50.0f);
}

//===========================================================
// 攻撃が発生しているかどうか判定
//===========================================================
void CEnemyBoss::HitDetection(D3DXVECTOR3 MyPos, float attackrange, float targetradius)
{
	
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CEnemyBoss::Init(void)
{
	CEnemy::Init();
	SetType(BOSS);
	ReadText(ENEMY_TEXT);

	// 敵の情報取得
	CEnemy::INFO* Info = GetInfo();

	if (m_pLife2D == nullptr)
	{
		m_pLife2D = CGage2D::Create(D3DXVECTOR3((SCREEN_WIDTH * 0.25f) + 4.0f, 650.0f, 0.0f), 40.0f, (float)Info->nLife * 2.0f, CGage2D::TYPE_LIFE);
		m_pLife2D->GetObj2D()->SetEdgeCenterTex((float)(Info->nLife * 0.1f));
	}
	
	return S_OK;
}

//===========================================================
// 終了処理
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
// 更新処理
//===========================================================
void CEnemyBoss::Update(void)
{
	CEnemy::Update();

	// 敵の情報取得
	CEnemy::INFO* Info = GetInfo();

	if (Info != nullptr)
		return;

	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
		return;

	// ステイトの更新
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
// 描画処理
//===========================================================
void CEnemyBoss::Draw(void)
{
	CEnemy::Draw();
}

//======================================================================
// ステイト
//======================================================================
CEnemyBossState::CEnemyBossState()
{
}

//===========================================================
// 待機待ち状態の処理
//===========================================================
CEnemyBossStateMoveWait::CEnemyBossStateMoveWait()
{

}

void CEnemyBossStateMoveWait::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理


}


//===========================================================
// 移動状態の処理
//===========================================================
CEnemyBossStateMove::CEnemyBossStateMove()
{

}

void CEnemyBossStateMove::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	// モーション情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// プレイヤーの情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// 位置保存
	Info->posOld = Info->pos;

	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	float fDiffmove = 0.0f;

	// 追尾
	fDiffmove = utility::MoveToPosition(Info->pos, PlayerPos, Info->rot.y);

	// 角度補正
	fDiffmove = utility::CorrectAngle(fDiffmove);

	Info->rot.y += fDiffmove * 0.05f;

	// 角度補正
	Info->rot.y = utility::CorrectAngle(Info->rot.y);

	//移動量を更新(減衰させる)
	Info->move.x = sinf(Info->rot.y + D3DX_PI) * SPEED;
	Info->move.z = cosf(Info->rot.y + D3DX_PI) * SPEED;

	Info->pos += Info->move;

	// プレイヤーとの距離
	float fLenght = utility::Distance(Info->pos, PlayerPos);

	if (fLenght <= ATTACKLENGHT)
	{// 攻撃可能範囲内に入った

		Info->state = CEnemy::STATE_NEUTRAL;
		pMotion->Set(CEnemy::MOTION_NEUTRAL);
		pEnemyBoss->ChangeState(new CEnemyBossStateAttackWait);
	}
}


//===========================================================
// 攻撃状態の処理
//===========================================================
CEnemyBossStateAttack::CEnemyBossStateAttack()
{

}

void CEnemyBossStateAttack::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理


}


//===========================================================
// 攻撃待ち状態の処理
//===========================================================
CEnemyBossStateAttackWait::CEnemyBossStateAttackWait()
{

}

void CEnemyBossStateAttackWait::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理


}





//===========================================================
// ダメージ状態の処理
//===========================================================
CEnemyBossStateDamege::CEnemyBossStateDamege()
{
}

void CEnemyBossStateDamege::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// モーションが終了していたら
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
// 死亡状態の処理
//===========================================================
CEnemyBossStateDeath::CEnemyBossStateDeath()
{

}

void CEnemyBossStateDeath::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理

	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		// 敵の総数を減らす
		int nNum = CEnemyManager::GetNum() - 1;
		CEnemyManager::SetNum(nNum);
		pEnemyBoss->Uninit();

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{// ゲームの時

			// 敵を倒した数を増やす
			int nDefeat = CPlayer::GetInstance()->GetDefeat() + 1;
			CPlayer::GetInstance()->SetDefeat(nDefeat);
		}
	}
}


////===========================================================
//// 攻撃の種類
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
//// 攻撃の種類：ローリングパンチ
////===========================================================
//void CAttackTypeRollingPunch::Update(CEnemy* pEnemy)
//{
//	// キャラのパーツ取得
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
//	// 左手
//	if (ppParts[8] != nullptr)
//	{
//		// マトリックスの取得
//		PartsMtxR = ppParts[8]->GetMtxWorld();
//
//		PartsPosR.x = PartsMtxR->_41;
//		PartsPosR.y = PartsMtxR->_42;
//		PartsPosR.z = PartsMtxR->_43;
//	}
//
//	// 右手
//	if (ppParts[11] != nullptr)
//	{
//		// マトリックスの取得
//		PartsMtxL = ppParts[11]->GetMtxWorld();
//
//		PartsPosL.x = PartsMtxL->_41;
//		PartsPosL.y = PartsMtxL->_42;
//		PartsPosL.z = PartsMtxL->_43;
//	}
//
//	// 攻撃判定
//	pEnemy->HitDetection(PartsPosR, 20.0f, 50.0f);
//
//	// 攻撃判定
//	pEnemy->HitDetection(PartsPosL, 20.0f, 50.0f);
//}
//
//CAttackTypeNormalPunch::CAttackTypeNormalPunch()
//{
//}
//
////===========================================================
//// 攻撃の種類：ノーマルパンチ
////===========================================================
//void CAttackTypeNormalPunch::Update(CEnemy* pEnemy)
//{
//	// キャラのパーツ取得
//	CCharacter** ppParts = nullptr;
//	ppParts = pEnemy->GetCharcter();
//
//	if (ppParts == nullptr)
//		return;
//
//	D3DXVECTOR3 PartsPos = {};
//	D3DXMATRIX* PartsMtx = {};
//
//	// 右手
//	if (ppParts[11] != nullptr)
//	{
//		// マトリックスの取得
//		PartsMtx = ppParts[11]->GetMtxWorld();
//
//		PartsPos.x = PartsMtx->_41;
//		PartsPos.y = PartsMtx->_42;
//		PartsPos.z = PartsMtx->_43;
//	}
//
//	// 攻撃判定
//	pEnemy->HitDetection(PartsPos, 50.0f, 50.0f);
//}
//
//CAttackTypeBodyPress::CAttackTypeBodyPress()
//{
//}
//
////===========================================================
//// 攻撃の種類：フライングプレス
////===========================================================
//void CAttackTypeBodyPress::Update(CEnemy* pEnemy)
//{
//	// キャラのパーツ取得
//	CCharacter** ppParts = nullptr;
//	ppParts = pEnemy->GetCharcter();
//
//	if (ppParts == nullptr)
//		return;
//
//	D3DXVECTOR3 PartsPos = {};
//	D3DXMATRIX* PartsMtx = {};
//
//	// 腹
//	if (ppParts[5] != nullptr)
//	{
//		// マトリックスの取得
//		PartsMtx = ppParts[5]->GetMtxWorld();
//
//		PartsPos.x = PartsMtx->_41;
//		PartsPos.y = PartsMtx->_42;
//		PartsPos.z = PartsMtx->_43;
//	}
//
//	// 攻撃判定
//	pEnemy->HitDetection(PartsPos, 50.0f, 50.0f);
//}
//
////===========================================================
//// ステイトの変更
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