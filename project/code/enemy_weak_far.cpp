//===========================================================
//
// 敵の処理 [enemy.cpp]
// Author : 大原　怜将
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
#include "bullet.h"
#include "MyEffekseer.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const int DAMEGE = 10;             // ダメージ状態
	const int ATTACKAGAINCOUNT = 60;   // 再攻撃できるまでの時間
	const float SPEED = 2.0f;          // 走る速さ
	const float ATTACKLENGHT = 200.0f;  // 攻撃可能範囲
	const float SEARCHRANGE = 400.0f;  // 探索範囲
	const char* TEXT_NAME = "data\\TEXT\\enemyfar.txt";  // テキストファイルの名前
}

//===========================================================
// コンストラクタ
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
// コンストラクタ
//===========================================================
CEnemyWeakFar::CEnemyWeakFar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority) : CEnemy(pos, rot, nlife, nPriority)
{
	// 値をクリア
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
// デストラクタ
//===========================================================
CEnemyWeakFar::~CEnemyWeakFar()
{

}

//===========================================================
// 生成処理
//===========================================================
CEnemyWeakFar* CEnemyWeakFar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
{
	CEnemyWeakFar* pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CEnemyWeakFar(pos, rot, nlife, nPriority);

		pEnemy->Init();

		// 敵の情報取得
		CEnemy::INFO* Info = pEnemy->GetInfo();

		Info->pos = pos;
		Info->rot = rot;
		Info->nLife = nlife;
	}

	return pEnemy;
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CEnemyWeakFar::Init(void)
{
	CEnemy::Init();
	SetType(WEAK);

	// 敵の情報取得
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
// 終了処理
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
// 更新処理
//===========================================================
void CEnemyWeakFar::Update(void)
{
	// 敵の情報取得
	CEnemy::INFO* Info = GetInfo();

	if (Info == nullptr)
		return;

	// モーション情報取得
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
		return;

	CEnemy::Update();

	if (GetMobility() == Immobile)
		return;

	// ステイトの更新
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

	CManager::GetInstance()->GetDebugProc()->Print("敵の位置：[%f, %f, %f]\n", Info->pos.x, Info->pos.y, Info->pos.z);
}

//===========================================================
// 描画処理
//===========================================================
void CEnemyWeakFar::Draw(void)
{
	CEnemy::Draw();
}

//===========================================================
// ステイトの切り替え
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
// ダメージ処理
//===========================================================
void CEnemyWeakFar::Damege(void)
{
	// すでにダメージ状態
	if (m_bDamage)
		return;

	// 敵の情報取得
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
	{// 体力が５０以上のとき

		// ダメージ状態に切り替える
		ChangeState(new CEnemyWeakFarStateDamege);
	}
	if (Info->nLife > 0 && (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH))
	{// 体力が５０以上のとき

		// ダメージ状態に切り替える
		ChangeState(new CEnemyWeakFarStateHeavyDamege);
	}
	else if (Info->nLife <= 0)
	{// 体力が０以下のとき

		// 死亡状態に切り替える
		CGame::GetInstance()->SetbFinish(true);
		CCamera::GetInstance()->ChangeState(new FinalBlowCamera);
		ChangeState(new CEnemyWeakFarStateDeath);
	}
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
// ステイト
//======================================================================
//===========================================================
// 行動待ち状態の処理
//===========================================================
CEnemyWeakFarStateMoveWait::CEnemyWeakFarStateMoveWait()
{

}

//===========================================================
// 行動待ち状態の更新処理
//===========================================================
void CEnemyWeakFarStateMoveWait::Update(CEnemyWeakFar* pEnemyWeak)
{
	// 当たり判定の情報取得
	CCollision* pCollision = CCollision::GetInstance();

	if (pCollision == nullptr)
		return;

	// プレイヤーの情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// モーション情報取得
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	if (/*m_Chase == CHASE_ON && */CGame::GetCollision()->Circle(Info->pos, pPlayer->GetPosition(), SEARCHRANGE, pPlayer->GetRadius()) == true)
	{//円の中にプレイヤーが入った

		Info->state = pEnemyWeak->STATE_DASH;
		pMotion->Set(pEnemyWeak->MOTION_DASH);
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateMove);
	}
	else
	{// プレイヤーが円の中にいない

		Info->move.x = 0.0f;
		Info->move.z = 0.0f;

		// 待機状態にする
		if (pMotion->GetType() != pEnemyWeak->MOTION_NEUTRAL)
			pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);

	}
}

//=================================================================
// 攻撃待ち状態の処理
//=================================================================
//===========================================================
// 攻撃待ち状態のコンストラクタ
//===========================================================
CEnemyWeakFarStateAttackWait::CEnemyWeakFarStateAttackWait()
{

}

//===========================================================
// 攻撃待ち状態の更新処理
//===========================================================
void CEnemyWeakFarStateAttackWait::Update(CEnemyWeakFar* pEnemyWeak)
{
	// モーション情報取得
	CMotion* pMotion = pEnemyWeak->GetMotion();

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	if (pMotion->GetType() != pEnemyWeak->MOTION_BATTLE_NEUTRAL)
		pMotion->Set(pEnemyWeak->MOTION_BATTLE_NEUTRAL);

	m_nAtcCounter++;

	if (m_nAtcCounter >= ATTACKAGAINCOUNT)
	{
		m_nAtcCounter = 0;

		// 攻撃状態にする
		pMotion->Set(pEnemyWeak->MOTION_ATTACK);
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateAttack);
	}
}

//===========================================================
// 攻撃状態の処理
//===========================================================
CEnemyWeakFarStateAttack::CEnemyWeakFarStateAttack()
{

}

//===========================================================
// 攻撃状態の更新処理
//===========================================================
void CEnemyWeakFarStateAttack::Update(CEnemyWeakFar* pEnemyWeak)
{
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	if (!m_bAttack)
	{
		D3DXMATRIX* mtx = pEnemyWeak->GetCharcter()[5]->GetMtxWorld();

		CBullet::Create(D3DXVECTOR3(mtx->_41, mtx->_42, mtx->_43), Info->rot);

		m_bAttack = true;
	}
	
	//pEnemyWeak->HitDetection(Info->pos, ATTACKLENGHT, pPlayer->GetRadius());

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		// プレイヤーとの距離
		float fLenght = utility::Distance(Info->pos, PlayerPos);

		Info->state = pEnemyWeak->STATE_NEUTRAL;
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		m_bAttack = false;

		// 状態の切り替え
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateMoveWait);
	}
}

//===========================================================
// 移動状態の処理
//===========================================================
CEnemyWeakFarStateMove::CEnemyWeakFarStateMove()
{
}

void CEnemyWeakFarStateMove::Update(CEnemyWeakFar* pEnemyWeak)
{
	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	// モーション情報取得
	CMotion* pMotion = pEnemyWeak->GetMotion();

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

		// 状態の切り替え
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateAttackWait);
	}
}

//===========================================================
// ダメージ状態の処理
//===========================================================
CEnemyWeakFarStateDamege::CEnemyWeakFarStateDamege()
{

}

void CEnemyWeakFarStateDamege::Update(CEnemyWeakFar* pEnemyWeak)
{
	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	if (Info == nullptr)
		return;

	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateMoveWait);
		pEnemyWeak->SetbDamage();
	}
}

//===========================================================
// 強い攻撃を受けた状態の処理
//===========================================================
CEnemyWeakFarStateHeavyDamege::CEnemyWeakFarStateHeavyDamege()
{

}

//===========================================================
// 強い攻撃を受けた状態の更新処理
//===========================================================
void CEnemyWeakFarStateHeavyDamege::Update(CEnemyWeakFar* pEnemyWeak)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションがノックダウン以外のとき
	if (pMotion->GetType() != pEnemyWeak->MOTION_FALLDOWN)
		pMotion->Set(pEnemyWeak->MOTION_FALLDOWN);

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		pEnemyWeak->ChangeState(new CEnemyWeakFarStateGetUp);
	}
}

//===========================================================
// 死亡状態の処理
//===========================================================
CEnemyWeakFarStateDeath::CEnemyWeakFarStateDeath()
{
}

void CEnemyWeakFarStateDeath::Update(CEnemyWeakFar* pEnemyWeak)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		// 敵の総数を減らす
		int nNum = CEnemyManager::GetNum() - 1;
		CEnemyManager::SetNum(nNum);
		pEnemyWeak->Uninit();

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{// ゲームの時

			// 敵を倒した数を増やす
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
	// モーションの情報取得
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyWeak->GetInfo();

	if (Info == nullptr)
		return;

	// モーションが起き上がり以外のとき
	if (pMotion->GetType() != pEnemyWeak->MOTION_GETUP)
		pMotion->Set(pEnemyWeak->MOTION_GETUP);

	else if (pMotion->IsFinish())
	{// モーションが終了している

		// 移動状態に切り替える
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