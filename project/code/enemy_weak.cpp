//===========================================================
//
// 敵の処理 [enemy.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "enemy.h"
#include "enemy_weak.h"
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
// 定数定義
//===========================================================
namespace
{
	const int DAMAGECOUNT = 20;             // ダメージ状態
	const int ATTACKAGAINCOUNT = 60;   // 再攻撃できるまでの時間
	const float SPEED = 2.0f;          // 走る速さ
	const float ATTACKLENGHT = 50.0f;  // 攻撃可能範囲
	const float SEARCHRANGE = 400.0f;  // 探索範囲
	const int RECOVER_DAMAGE_TIME = 15;  // ダメージ状態でいる時間
	const char* TEXT_NAME = "data\\TEXT\\motion_enemy.txt";  // テキストファイルの名前
}

//===========================================================
// コンストラクタ
//===========================================================
CEnemyWeak::CEnemyWeak()
{
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;

	
	m_Mobility = Immobile;
	m_nDamageCounter = 0;
	/*m_pCurrent = nullptr;
	m_pNext = nullptr;
	m_pLife2D = nullptr;*/
	m_pLife3D = nullptr;
	//m_bDeath = false;
	m_bDamage = false;
	m_bStagger = false;
	ChangeState(new CEnemyWeakStateMoveWait);
}

//===========================================================
// コンストラクタ
//===========================================================
CEnemyWeak::CEnemyWeak(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority) : CEnemy(pos, rot, nlife, nPriority)
{
	// 値をクリア
	//SetState(CEnemy::STATE_NONE);
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;
	m_Mobility = Immobile;
	m_nDamageCounter = 0;
	m_pLife3D = nullptr;
	m_bDamage = false;
	m_bStagger = false;
	ChangeState(new CEnemyWeakStateMoveWait);
}

//===========================================================
// デストラクタ
//===========================================================
CEnemyWeak::~CEnemyWeak()
{

}

//===========================================================
// 生成処理
//===========================================================
CEnemyWeak * CEnemyWeak::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
{
	CEnemyWeak *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CEnemyWeak(pos, rot, nlife, nPriority);

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
HRESULT CEnemyWeak::Init(void)
{
	CEnemy::Init();
	SetType(WEAK);

	// 敵の情報取得
	CEnemy::INFO *Info = GetInfo();

	ReadText(TEXT_NAME);

	if (m_pLife3D == nullptr)
	{
		m_pLife3D = CGage3D::Create(D3DXVECTOR3(Info->pos.x, Info->pos.y, Info->pos.z), 5.0f, (float)((Info->nLife * 0.01f) * 20), CGage3D::TYPE_LIFE);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		m_pLife3D->SetPos(D3DXVECTOR3(mtx->_41, mtx->_42, mtx->_43));
		m_pLife3D->SetUpHeight(90.0f);
		m_pLife3D->GetBill()->SetEdgeCenter((float)((Info->nLife * 0.01f) * 20), 5.0f);
	}
	
	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CEnemyWeak::Uninit(void)
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
void CEnemyWeak::Update(void)
{
	// 敵の情報取得
	CEnemy::INFO *Info = GetInfo();

	if (Info == nullptr)
		return;

	// モーション情報取得
	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
		return;

	CEnemy::Update();

	if (m_pLife3D != nullptr)
	{
		m_pLife3D->GetBill()->SetEdgeCenter((float)((Info->nLife * 0.01f) * 20), 5.0f);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		m_pLife3D->SetPos(D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43));
	}

	if (GetMobility() == Immobile)
		return;

	// ステイトの更新
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (Info->state == STATE_DEATH)
		return;

	CManager::GetInstance()->GetDebugProc()->Print("敵の位置：[%f, %f, %f]\n", Info->pos.x, Info->pos.y, Info->pos.z);
}

//===========================================================
// 描画処理
//===========================================================
void CEnemyWeak::Draw(void)
{
	CEnemy::Draw();
}

//===========================================================
// ステイトの切り替え
//===========================================================
void CEnemyWeak::ChangeState(CEnemyWeakState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//================================================================
// 走っているときのエフェクト
//================================================================
void CEnemyWeak::DashEffect(void)
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

//===========================================================
// ダメージ処理
//===========================================================
void CEnemyWeak::Damage(void)
{
	// すでにダメージ状態
	if (m_bDamage || m_bHeatDamage)
		return;

	// 敵の情報取得
	CEnemy::INFO* Info = GetInfo();

	if (Info == nullptr)
		return;

	if (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_NONE)
	{
		GetMotion()->Set(MOTION_DAMAGE);
		m_bDamage = true;
	}
	else if (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH)
	{
		m_bHeatDamage = true;
	}

	Info->nLife -= CPlayer::GetInstance()->GetMotion()->GetAttackDamage();
	
	MyEffekseer::EffectCreate(CMyEffekseer::TYPE_HIT, false, D3DXVECTOR3(Info->pos.x, Info->pos.y + 50.0f, Info->pos.z));

	if (Info->nLife > 0 && CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_NONE)
	{// 体力が５０以上のとき

		// ダメージ状態に切り替える
		ChangeState(new CEnemyWeakStateDamage);
	}
	if (Info->nLife > 0 && (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH))
 	{// 体力が５０以上のとき

		// ダメージ状態に切り替える
		ChangeState(new CEnemyWeakStateHeavyDamage);
	}
	else if (Info->nLife <= 0)
	{// 体力が０以下のとき

		// 死亡状態に切り替える
		ChangeState(new CEnemyWeakStateDeath);

		m_bDeath = true;
	}
}

//===========================================================
// 捕まれたときの処理
//===========================================================
void CEnemyWeak::Grabbed(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	// 死亡フラグが立っているとき
	if (m_bDeath)
		return;

	if (pPlayer->GetbGrap() == true)
		ChangeState(new CEnemyWeakStateGrabbed);

	else if (pPlayer->GetbGrap() == false)
	{
		ChangeState(new CEnemyWeakStateMoveWait);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		GetInfo()->pos = D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43);
		m_bDamage = false;

		// 敵の情報取得
		CEnemy::INFO* Info = GetInfo();
		D3DXVECTOR3 ForwardVector = utility::CalculateDirection(Info->pos, pPlayer->GetPosition());
		float angle = atan2f(ForwardVector.x, ForwardVector.z);
		angle -= D3DX_PI;
		angle = utility::CorrectAngle(angle);
		Info->rot.y = angle;
	}
		
}

//===========================================================
// ヒートアクション：自転車受け待ち
//===========================================================
void CEnemyWeak::Denial(void)
{
	// ステートの切り替え
	ChangeState(new CEnemyWeakStateDenial);
}

//======================================================================
// ステイト
//======================================================================
//===========================================================
// 行動待ち状態の処理
//===========================================================
CEnemyWeakStateMoveWait::CEnemyWeakStateMoveWait()
{

}

void CEnemyWeakStateMoveWait::Update(CEnemyWeak* pEnemyWeak)
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
	CEnemy::INFO *Info = pEnemyWeak->GetInfo();

	if (/*m_Chase == CHASE_ON && */CGame::GetCollision()->Circle(Info->pos, pPlayer->GetPosition(), SEARCHRANGE, pPlayer->GetRadius()) == true)
	{//円の中にプレイヤーが入った

		Info->state = pEnemyWeak->STATE_DASH;
		pMotion->Set(pEnemyWeak->MOTION_DASH);
		pEnemyWeak->ChangeState(new CEnemyWeakStateMove);
	}
	else
	{// プレイヤーが円の中にいない

		Info->move.x = 0.0f;
		Info->move.z = 0.0f;

		// 待機状態にする
		if (Info->state != pEnemyWeak->STATE_NEUTRAL)
		{
			Info->state = pEnemyWeak->STATE_NEUTRAL;
			pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		}
	}
}

//===========================================================
// 攻撃待ち状態の処理
//===========================================================
CEnemyWeakStateAttackWait::CEnemyWeakStateAttackWait()
{

}

void CEnemyWeakStateAttackWait::Update(CEnemyWeak* pEnemyWeak)
{
	// モーション情報取得
	CMotion* pMotion = pEnemyWeak->GetMotion();

	// 敵の情報取得
	CEnemy::INFO *Info = pEnemyWeak->GetInfo();

	m_nAtcCounter++;

	if (m_nAtcCounter >= ATTACKAGAINCOUNT)
	{
		m_nAtcCounter = 0;

		// 攻撃状態にする
		Info->state = pEnemyWeak->STATE_ATTACK;
		pMotion->Set(pEnemyWeak->MOTION_ATTACK);
		pEnemyWeak->ChangeState(new CEnemyWeakStateAttack);
	}
}

//===========================================================
// 攻撃状態の処理
//===========================================================
CEnemyWeakStateAttack::CEnemyWeakStateAttack()
{

}

void CEnemyWeakStateAttack::Update(CEnemyWeak* pEnemyWeak)
{
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// 敵の情報取得
	CEnemy::INFO *Info = pEnemyWeak->GetInfo();

	pEnemyWeak->HitDetection(Info->pos, ATTACKLENGHT, pPlayer->GetRadius());

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		// プレイヤーとの距離
		float fLenght = utility::Distance(Info->pos, PlayerPos);

		Info->state = pEnemyWeak->STATE_NEUTRAL;
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		
		pEnemyWeak->ChangeState(new CEnemyWeakStateMoveWait);
	}
}

//===========================================================
// 移動状態の処理
//===========================================================
CEnemyWeakStateMove::CEnemyWeakStateMove()
{
}

void CEnemyWeakStateMove::Update(CEnemyWeak* pEnemyWeak)
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

	D3DXVECTOR3 ForwardVector = utility::CalculateDirection(Info->pos, PlayerPos);

	float angle = atan2f(ForwardVector.x, ForwardVector.z);

	angle -= D3DX_PI;

	angle = utility::CorrectAngle(angle);

	Info->rot.y = angle;

	Info->move = ForwardVector * SPEED;

	Info->pos += Info->move;

	// 走っているときのエフェクト
	pEnemyWeak->DashEffect();

	// プレイヤーとの距離
	float fLenght = utility::Distance(Info->pos, PlayerPos);

	if (fLenght <= ATTACKLENGHT)
	{// 攻撃可能範囲内に入った

		Info->state = pEnemyWeak->STATE_NEUTRAL;
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		pEnemyWeak->ChangeState(new CEnemyWeakStateAttackWait);
	}

	// 敵との当たり判定
	CEnemy* pEnemy = CEnemy::GetTop();

	// 当たり判定取得
	CCollision* pCollision = CGame::GetCollision();

	while (pEnemy != nullptr)
	{
		CEnemy* pEnemyNext = pEnemy->GetNext();

		if (Info->nIdxID != pEnemy->GetIdxID() && utility::CheckCirclePushOut(&Info->pos, &pEnemy->GetPosition(), 20.0f, ForwardVector))
		{
			pEnemyWeak->ChangeState(new CEnemyWeakStateAttackWait);
		}
			
		pEnemy = pEnemyNext;
	}
}

//===========================================================
// ダメージ状態の処理
//===========================================================
CEnemyWeakStateDamage::CEnemyWeakStateDamage()
{
	m_nRecoverDamageTime = DAMAGECOUNT;
}

void CEnemyWeakStateDamage::Update(CEnemyWeak* pEnemyWeak)
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

	m_nRecoverDamageTime--;

	if (m_nRecoverDamageTime <= 0)
		pEnemyWeak->SetbDamage();

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		pMotion->Set(pEnemyWeak->MOTION_NEUTRAL);
		pEnemyWeak->ChangeState(new CEnemyWeakStateMoveWait);
		pEnemyWeak->SetbDamage();
	}
}

//================================================================
// 拒否状態の処理
//================================================================
//===========================================================
// 拒否状態のコンストラクタ
//===========================================================
CEnemyWeakStateDenial::CEnemyWeakStateDenial()
{

}

//===========================================================
// 拒否状態の更新処理
//===========================================================
void CEnemyWeakStateDenial::Update(CEnemyWeak* pEnemyWeak)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションが拒否以外のとき
	if (pMotion->GetType() != pEnemyWeak->MOTION_DENIAL)
		pMotion->Set(pEnemyWeak->MOTION_DENIAL);

}

//===========================================================
// 強い攻撃を受けた状態の処理
//===========================================================
CEnemyWeakStateHeavyDamage::CEnemyWeakStateHeavyDamage()
{

}

//===========================================================
// 強い攻撃を受けた状態の更新処理
//===========================================================
void CEnemyWeakStateHeavyDamage::Update(CEnemyWeak* pEnemyWeak)
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
		pEnemyWeak->ChangeState(new CEnemyWeakStateGetUp);

		// ヒートアクションをくらったフラグを折る
		pEnemyWeak->RestHeatDamageFrag();
	}
}

//===========================================================
// 死亡状態の処理
//===========================================================
CEnemyWeakStateDeath::CEnemyWeakStateDeath()
{
}

void CEnemyWeakStateDeath::Update(CEnemyWeak* pEnemyWeak)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyWeak->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションがノックダウン以外のとき
	if (pMotion->GetType() != pEnemyWeak->MOTION_DEATH)
		pMotion->Set(pEnemyWeak->MOTION_DEATH);

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

CEnemyWeakStateGetUp::CEnemyWeakStateGetUp()
{
}

void CEnemyWeakStateGetUp::Update(CEnemyWeak* pEnemyWeak)
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
		pEnemyWeak->ChangeState(new CEnemyWeakStateMoveWait);
		pEnemyWeak->SetbDamage();
	}
}

CEnemyWeakStateGrabbed::CEnemyWeakStateGrabbed()
{

}

void CEnemyWeakStateGrabbed::Update(CEnemyWeak* pEnemyWeak)
{

}