//===========================================================
//
// 敵の処理 [enemy.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "enemy.h"
#include "tutorial_enemy.h"
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
CTutorial_Enemy::CTutorial_Enemy()
{
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;


	m_Mobility = Immobile;
	m_nDamageCounter = 0;
	m_pLife3D = nullptr;
	m_bDamage = false;
	m_bStagger = false;
	ChangeState(new CTutorialEnemyNeutral);
}

//===========================================================
// コンストラクタ
//===========================================================
CTutorial_Enemy::CTutorial_Enemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority) : CEnemy(pos, rot, nlife, nPriority)
{
	// 値をクリア
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;
	m_Mobility = Immobile;
	m_nDamageCounter = 0;
	m_pLife3D = nullptr;
	m_bDamage = false;
	m_bStagger = false;
	ChangeState(new CTutorialEnemyNeutral);
}

//===========================================================
// デストラクタ
//===========================================================
CTutorial_Enemy::~CTutorial_Enemy()
{

}

//===========================================================
// 生成処理
//===========================================================
CTutorial_Enemy* CTutorial_Enemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
{
	CTutorial_Enemy* pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CTutorial_Enemy(pos, rot, nlife, nPriority);

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
HRESULT CTutorial_Enemy::Init(void)
{
	CEnemy::Init();
	SetType(WEAK);

	// 敵の情報取得
	CEnemy::INFO* Info = GetInfo();

	ReadText(TEXT_NAME);

	if (m_pLife3D == nullptr)
	{
		m_pLife3D = CGage3D::Create(D3DXVECTOR3(Info->pos.x, Info->pos.y, Info->pos.z), 5.0f, (float)((Info->nLife * 0.01f) * 20), CGage3D::TYPE_LIFE);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		m_pLife3D->SetPos(GetInfo()->pos);
		m_pLife3D->SetMatrix(mtx);
		m_pLife3D->SetUpHeight(90.0f);
		m_pLife3D->GetBill()->SetEdgeCenter((float)((Info->nLife * 0.01f) * 20), 5.0f);
	}

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CTutorial_Enemy::Uninit(void)
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
void CTutorial_Enemy::Update(void)
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

	// ステイトの更新
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (Info->state == STATE_DEATH)
		return;

	if (m_pLife3D != nullptr)
	{
		m_pLife3D->GetBill()->SetEdgeCenter((float)((Info->nLife * 0.01f) * 20), 5.0f);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		m_pLife3D->SetPos(D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43));
	}

	CManager::GetInstance()->GetDebugProc()->Print("敵の位置：[%f, %f, %f]\n", Info->pos.x, Info->pos.y, Info->pos.z);
}

//===========================================================
// 描画処理
//===========================================================
void CTutorial_Enemy::Draw(void)
{
	CEnemy::Draw();
}

//===========================================================
// ステイトの切り替え
//===========================================================
void CTutorial_Enemy::ChangeState(CTutorialEnemyState* pState)
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
void CTutorial_Enemy::Damage(void)
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
		ChangeState(new CTutorialEnemyStateDamage);
	}
	if (Info->nLife > 0 && (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH))
	{// 体力が５０以上のとき

		// ダメージ状態に切り替える
		ChangeState(new CTutorialEnemyStateHeavyDamage);
	}
	else if (Info->nLife <= 0)
	{// 体力が０以下のとき

		// 死亡状態に切り替える
		ChangeState(new CTutorialEnemyStateDeath);

		m_bDeath = true;
	}
}

//===========================================================
// 捕まれたときの処理
//===========================================================
void CTutorial_Enemy::Grabbed(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer->GetbGrap() == true)
		ChangeState(new CTutorialEnemyStateGrabbed);

	else if (pPlayer->GetbGrap() == false)
	{
		ChangeState(new CTutorialEnemyNeutral);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		GetInfo()->pos = D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43);

		// 敵の情報取得
		CEnemy::INFO* Info = GetInfo();
		D3DXVECTOR3 ForwardVector = utility::CalculateDirection(Info->pos, pPlayer->GetPosition());
		float angle = atan2f(ForwardVector.x, ForwardVector.z);
		angle -= D3DX_PI;
		angle = utility::CorrectAngle(angle);
		Info->rot.y = angle;

		m_bDamage = false;
	}
}

//===========================================================
// ヒートアクション：自転車受け待ち
//===========================================================
void CTutorial_Enemy::Denial(void)
{
	// ステートの切り替え
	ChangeState(new CTutorialEnemyStateDenial);
}

//======================================================================
// ステイト
//======================================================================
//===========================================================
// ダメージ状態の処理
//===========================================================
CTutorialEnemyStateDamage::CTutorialEnemyStateDamage()
{
	m_nRecoverDamageTime = DAMAGECOUNT;
}

void CTutorialEnemyStateDamage::Update(CTutorial_Enemy* pEnemyWeak)
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
		pEnemyWeak->ChangeState(new CTutorialEnemyNeutral);
		pEnemyWeak->SetbDamage();
	}
}

//===========================================================
// 強い攻撃を受けた状態の処理
//===========================================================
CTutorialEnemyStateHeavyDamage::CTutorialEnemyStateHeavyDamage()
{

}

//===========================================================
// 強い攻撃を受けた状態の更新処理
//===========================================================
void CTutorialEnemyStateHeavyDamage::Update(CTutorial_Enemy* pEnemyWeak)
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
		pEnemyWeak->ChangeState(new CTutorialEnemyStateGetUp);

		// ヒートアクションをくらったフラグを折る
		pEnemyWeak->RestHeatDamageFrag();
	}
}

//===========================================================
// 死亡状態の処理
//===========================================================
CTutorialEnemyStateDeath::CTutorialEnemyStateDeath()
{
}

void CTutorialEnemyStateDeath::Update(CTutorial_Enemy* pEnemyWeak)
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

CTutorialEnemyStateGetUp::CTutorialEnemyStateGetUp()
{
}

void CTutorialEnemyStateGetUp::Update(CTutorial_Enemy* pEnemyWeak)
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
		pEnemyWeak->ChangeState(new CTutorialEnemyNeutral);
		pEnemyWeak->SetbDamage();
	}
}

CTutorialEnemyStateGrabbed::CTutorialEnemyStateGrabbed()
{
}

void CTutorialEnemyStateGrabbed::Update(CTutorial_Enemy* pEnemyWeak)
{
}

CTutorialEnemyNeutral::CTutorialEnemyNeutral()
{
}

void CTutorialEnemyNeutral::Update(CTutorial_Enemy* pEnemy)
{
	CMotion* pMotion = pEnemy->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションが起き上がり以外のとき
	if (pMotion->GetType() != pEnemy->MOTION_NEUTRAL)
		pMotion->Set(pEnemy->MOTION_NEUTRAL);

}

//================================================================
// 拒否状態の処理
//================================================================
//===========================================================
// 拒否状態のコンストラクタ
//===========================================================
CTutorialEnemyStateDenial::CTutorialEnemyStateDenial()
{

}

//===========================================================
// 拒否状態の更新処理
//===========================================================
void CTutorialEnemyStateDenial::Update(CTutorial_Enemy* pEnemy)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemy->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションが拒否以外のとき
	if (pMotion->GetType() != pEnemy->MOTION_DENIAL)
		pMotion->Set(pEnemy->MOTION_DENIAL);

}