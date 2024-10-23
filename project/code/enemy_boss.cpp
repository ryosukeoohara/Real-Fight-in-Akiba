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
#include "InputKeyBoard.h"
#include "fade.h"
#include "MyEffekseer.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const int DAMAGECOUNT = 20;         // ダメージ状態
	const float ATTACKLENGTH = 120.0f;  // 攻撃可能な距離
	const float SPEED = 2.0f;           // 走る速さ
	const float ATTACKLENGHT = 100.0f;   // 攻撃可能範囲
	const int STAGGER_TIME = 600;       // よろめき状態でいる時間
	const int RECOVER_DAMAGE_TIME = 15;  // ダメージ状態でいる時間
	const int NUM_DAMAGE_TIME = 60;      // ダメージ状態だった合計時間

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
	m_nIdleTime = 0;
	m_bDamage = false;
	m_bHeatDamage = false;
	m_bAttack = false;
	m_IsAttackReady = false;
	m_bDeath = false;
	m_bStagger = false;
}

//===========================================================
// コンストラクタ
//===========================================================
CEnemyBoss::CEnemyBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority) : CEnemy(pos, rot, nlife, nPriority)
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
	m_nIdleTime = 0;
	m_bDamage = false;
	m_bHeatDamage = false;
	m_bAttack = false;
	m_IsAttackReady = false;
	m_bDeath = false;
	m_bStagger = false;
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
CEnemyBoss * CEnemyBoss::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
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
// 初期化処理
//===========================================================
HRESULT CEnemyBoss::Init(void)
{
	CEnemy::Init();
	SetType(BOSS);
	ReadText(ENEMY_TEXT);

	CMotion* pMotion = GetMotion();

	if (pMotion != nullptr)
	{
		//pMotion->Set(MOTION_ONSTEGE);
		ChangeState(new CEnemyBossAppear);
	}

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

	if (Info == nullptr)
		return;

	CMotion* pMotion = GetMotion();

	if (pMotion == nullptr)
		return;

	// ステイトの更新
	if (m_pState != nullptr)
		m_pState->Update(this);

	if (m_pLife2D != nullptr)
		m_pLife2D->GetObj2D()->SetEdgeCenterTex((float)((Info->nLife * 0.1f) * 20.0f));

	// ダメージ状態ではないとき処理を抜ける
	if (!m_bDamage || m_bHeatDamage || m_bStagger || Info->nLife < 0)
		return;

	m_nIdleTime++;

	if (m_nIdleTime >= NUM_DAMAGE_TIME)
	{
		m_nIdleTime = 0;

		// プレイヤーの情報取得
		CPlayer* pPlayer = CPlayer::GetInstance();

		if (pPlayer == nullptr)
			return;

		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		// プレイヤーとの距離
		float fLenght = utility::Distance(Info->pos, PlayerPos);

		if (fLenght <= ATTACKLENGHT)
		{// 攻撃可能範囲内に入った

			m_IsAttackReady = true;
			m_bDamage = false;

			ChangeState(new CEnemyBossStateAttackWait);
		}
	}
}

//===========================================================
// 描画処理
//===========================================================
void CEnemyBoss::Draw(void)
{
	CEnemy::Draw();
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

//===========================================================
// 捕まれた時の処理
//===========================================================
void CEnemyBoss::Grabbed(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer->GetbGrap() == true)
	{
		ChangeState(new CEnemyBossStateGrabbed);
		m_bAttack = false;
	}
	else if (pPlayer->GetbGrap() == false)
	{
		ChangeState(new CEnemyBossStateMove);
		D3DXMATRIX* mtx = GetCharcter()[0]->GetMtxWorld();
		GetInfo()->pos = D3DXVECTOR3(mtx->_41, 0.0f, mtx->_43);

		// 敵の情報取得
		CEnemy::INFO* Info = GetInfo();
		D3DXVECTOR3 ForwardVector = utility::CalculateDirection(Info->pos, pPlayer->GetPosition());
		float angle = atan2f(ForwardVector.x, ForwardVector.z);
		angle -= D3DX_PI;
		angle = utility::CorrectAngle(angle);
		Info->rot.y = angle;
		m_bAttack = false;
		m_bDamage = false;
	}
		
}

//===========================================================
// ヒートアクション：自転車受け待ち
//===========================================================
void CEnemyBoss::Denial(void)
{
	// ステートの切り替え
	ChangeState(new CEnemyBossStateDenial);

	m_bAttack = false;
}

//===========================================================
// ダメージ処理
//===========================================================
void CEnemyBoss::Damage(void)
{
	// すでにダメージ状態
	if (m_bDamage || m_bAttack || m_IsAttackReady || m_bDeath)
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
	else if (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH)
	{
		GetMotion()->Set(MOTION_FALLDOWN);
		m_bHeatDamage = true;
	}
	else if (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE)
	{
		GetMotion()->Set(MOTION_HEATACTFAINTING); 
		m_bHeatDamage = true;
	}
	
	Info->nLife -= CPlayer::GetInstance()->GetMotion()->GetAttackDamage();
	m_bStagger = false;
	CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_HIT, ::Effekseer::Vector3D(Info->pos.x, Info->pos.y + 50.0f, Info->pos.z));

	if (Info->nLife >= 30 && CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_NONE)
	{// 体力が５０以上のとき

		// ダメージ状態に切り替える
		ChangeState(new CEnemyBossStateDamage);
	}
	else if (Info->nLife >= 30 && (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH))
	{// 体力が５０以上のとき

		// ダメージ状態に切り替える
		ChangeState(new CEnemyBossStateHeavyDamage);
	}
	else if (Info->nLife < 30 && Info->nLife > 0)
	{// 体力が５０未満かつ0より高いとき
		
		// よろめき状態に切り替える
		ChangeState(new CEnemyBossStateKnockDown);
		m_bStagger = true;
	}
	else if (Info->nLife <= 0 && CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE)
	{// 体力が０以下のとき

		// 死亡状態に切り替える
		CGame::GetInstance()->SetbFinish(true);
		CCamera::GetInstance()->ChangeState(new FinalBlowCamera);
		ChangeState(new CEnemyBossStateDeathByMicrowave);
		m_bDeath = true;
	}
	else if (Info->nLife <= 0)
	{
		CGame::GetInstance()->SetbFinish(true);
		CCamera::GetInstance()->ChangeState(new FinalBlowCamera);
		ChangeState(new CEnemyBossStateDeath);

		m_bDeath = true;
	}
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
	int Now = GetMotion()->GetNowFrame();
	int Occurs = GetMotion()->GetAttackOccurs();

	if (Now == Occurs)
	{
		D3DXMATRIX mtx = *GetCharcter()[0]->GetMtxWorld();
		CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_ATTACK, ::Effekseer::Vector3D(mtx._41, mtx._42, mtx._43), ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(25.0f, 25.0f, 25.0f));
	}

	if (GetMotion()->GetAttackOccurs() <= GetMotion()->GetNowFrame() && GetMotion()->GetAttackEnd() >= GetMotion()->GetNowFrame())
	{// 現在のフレームが攻撃判定発生フレーム以上かつ攻撃判定終了フレームない

		if (CGame::GetCollision()->Circle(MyPos, CGame::GetPlayer()->GetPosition(), attackrange, targetradius) == true)
			CGame::GetPlayer()->Damage(GetMotion()->GetAttackDamage(), D3DXVECTOR3(sinf(GetRotition().y) * GetMotion()->GetKnockBack(), 0.0f, cosf(GetRotition().y) * GetMotion()->GetKnockBack()));
	}
}

//================================================================
// 走っているときのエフェクト
//================================================================
void CEnemyBoss::DashEffect(void)
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

//======================================================================
// ステート
//======================================================================
CEnemyBossState::CEnemyBossState()
{
}

//================================================================
// 待機待ち状態の処理
//================================================================
CEnemyBossStateMoveWait::CEnemyBossStateMoveWait()
{

}

void CEnemyBossStateMoveWait::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理


}


//================================================================
// 移動状態の処理
//================================================================
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

	if (pMotion->GetType() != pEnemyBoss->MOTION_DASH)
		pMotion->Set(pEnemyBoss->MOTION_DASH);

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

	// 走っているときのエフェクト
	pEnemyBoss->DashEffect();

	// プレイヤーとの距離
	float fLenght = utility::Distance(Info->pos, PlayerPos);

	if (fLenght <= ATTACKLENGHT)
	{// 攻撃可能範囲内に入った

		pMotion->Set(CEnemy::MOTION_NEUTRAL);
		pEnemyBoss->ChangeState(new CEnemyBossStateAttackWait);
	}
}

//================================================================
// 攻撃状態の処理
//================================================================
CEnemyBossStateAttack::CEnemyBossStateAttack()
{

}

void CEnemyBossStateAttack::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理


}


//================================================================
// 攻撃待ち状態の処理
//================================================================
CEnemyBossStateAttackWait::CEnemyBossStateAttackWait()
{

}

void CEnemyBossStateAttackWait::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理

	// モーション情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// 攻撃していないとき
	if(!pEnemyBoss->GetbAttack())
	   m_nAtcCounter++;
	
	if (m_nAtcCounter >= 60 || pEnemyBoss->GetbAttackReady())
	{
		m_nAtcCounter = 0;
		pEnemyBoss->SetbAttack(true);
		pEnemyBoss->SetbAttackReady(false);

		m_nAttackType = rand() % ATTACKTYPE_MAX;  //攻撃の種類抽選

		switch (m_nAttackType)
		{
		case ATTACKTYPE_GURUGURU:  // ぐるぐるパンチ

			pMotion->Set(pEnemyBoss->MOTION_GURUGURU);
			pEnemyBoss->RollingPunch();

			break;

		case ATTACKTYPE_PUNCH:  // 普通のパンチ

			pMotion->Set(pEnemyBoss->MOTION_PUNCH);
			pEnemyBoss->NormalPunch();

			break;

		case ATTACKTYPE_FLY:  // 飛んでおなかから落ちる

			pMotion->Set(pEnemyBoss->MOTION_ATTACK);
			pEnemyBoss->Fly();

			break;

		default:
			break;
		}
	}
	else if(m_nAtcCounter == 0 && pEnemyBoss->GetbAttack())
	{
	switch (m_nAttackType)
	{
	case ATTACKTYPE_GURUGURU:  // ぐるぐるパンチ

 		pEnemyBoss->RollingPunch();

		break;

	case ATTACKTYPE_PUNCH:  // 普通のパンチ

		pEnemyBoss->NormalPunch();

		break;

	case ATTACKTYPE_FLY:  // 飛んでおなかから落ちる

		pEnemyBoss->Fly();

		break;

	default:
		break;
	}
	}

	// モーションが終了した
	if (pMotion->IsFinish())
	{
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
		pEnemyBoss->SetbAttack(false);
	}
		
	// 攻撃しているときは処理を抜ける
	if (pEnemyBoss->GetbAttack())
		return;

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	// プレイヤーの情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	// プレイヤーとの距離
	float fLenght = utility::Distance(Info->pos, PlayerPos);

	if (fLenght > ATTACKLENGHT)
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);

}

//================================================================
// ダメージ状態の処理
//================================================================
//===========================================================
// ダメージ状態のコンストラクタ
//===========================================================
CEnemyBossStateDamage::CEnemyBossStateDamage()
{
	m_nRecoverDamageTime = DAMAGECOUNT;
}

//===========================================================
// ダメージ状態の更新処理
//===========================================================
void CEnemyBossStateDamage::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	m_nRecoverDamageTime--;

	if (m_nRecoverDamageTime <= 0)
		pEnemyBoss->SetbDamage();

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		pMotion->Set(pEnemyBoss->MOTION_NEUTRAL);
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
		
	}
}

//================================================================
// 強い攻撃を受けた状態の処理
//================================================================
//===========================================================
// 強い攻撃を受けた状態のコンストラクタ
//===========================================================
CEnemyBossStateHeavyDamage::CEnemyBossStateHeavyDamage()
{
}

//===========================================================
// 強い攻撃を受けた状態の更新処理
//===========================================================
void CEnemyBossStateHeavyDamage::Update(CEnemyBoss* pEnemyBoss)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションがノックダウン以外のとき
	if (pMotion->GetType() != pEnemyBoss->MOTION_HEATDAMAGE)
		pMotion->Set(pEnemyBoss->MOTION_HEATDAMAGE);

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		pEnemyBoss->ChangeState(new CEnemyBossStateGetUp);

		// ヒートアクションをくらったフラグを折る
		pEnemyBoss->RestHeatDamageFrag();
	}
}

//================================================================
// よろめき状態の処理
//================================================================
//===========================================================
// よろめき状態のコンストラクタ
//===========================================================
CEnemyBossStateStagger::CEnemyBossStateStagger()
{
	m_nStaggerTime = STAGGER_TIME;  // よろめき状態でいる時間
}

//===========================================================
// よろめき状態の更新処理
//===========================================================
void CEnemyBossStateStagger::Update(CEnemyBoss* pEnemyBoss)
{
	pEnemyBoss->SetbDamage();

	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	// モーションがよろめき以外のとき
	if (pMotion->GetType() != pEnemyBoss->MOTION_SUTAN)
		pMotion->Set(pEnemyBoss->MOTION_SUTAN);

	m_nStaggerTime--;

	if (m_nStaggerTime <= 0)
	{
		pEnemyBoss->ChangeState(new CEnemyBossStateStaggerReCover);
	}
}

//================================================================
// 転ぶ状態の処理
//================================================================
//===========================================================
// 転ぶ状態のコンストラクタ
//===========================================================
CEnemyBossStateKnockDown::CEnemyBossStateKnockDown()
{

}

//===========================================================
// 転ぶ状態の更新処理
//===========================================================
void CEnemyBossStateKnockDown::Update(CEnemyBoss* pEnemyBoss)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションがノックダウン以外のとき
	if (pMotion->GetType() != pEnemyBoss->MOTION_HEATDAMAGE)
		pMotion->Set(pEnemyBoss->MOTION_HEATDAMAGE);

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		pEnemyBoss->SetbHeatDamage(false);
		pEnemyBoss->ChangeState(new CEnemyBossStateGetUp);
	}
}

//================================================================
// 起き上がり状態の処理
//================================================================
//===========================================================
// 起き上がり状態のコンストラクタ
//===========================================================
CEnemyBossStateGetUp::CEnemyBossStateGetUp()
{

}

//===========================================================
// 起き上がり状態の更新処理
//===========================================================
void CEnemyBossStateGetUp::Update(CEnemyBoss* pEnemyBoss)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	// モーションが起き上がり以外のとき
	if (pMotion->GetType() != pEnemyBoss->MOTION_GETUP)
		pMotion->Set(pEnemyBoss->MOTION_GETUP);


	if (pMotion->IsFinish() && Info->nLife < 50)
	{// モーションが終了していてかつ、体力が５０未満だったら

		// よろめき状態に切り替える
		pEnemyBoss->ChangeState(new CEnemyBossStateStagger);
		pEnemyBoss->SetbDamage();
	}
	else if (pMotion->IsFinish())
	{// モーションが終了している

		// 移動状態に切り替える
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
		pEnemyBoss->SetbDamage();
	}
}

//================================================================
// よろめき状態からの復帰する状態
//================================================================
//===========================================================
// よろめき状態からの復帰する状態のコンストラクタ
//===========================================================
CEnemyBossStateStaggerReCover::CEnemyBossStateStaggerReCover()
{

}

//===========================================================
// よろめき状態からの復帰する状態の更新処理
//===========================================================
void CEnemyBossStateStaggerReCover::Update(CEnemyBoss* pEnemyBoss)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	// モーションが起き上がり以外のとき
	if (pMotion->GetType() != pEnemyBoss->MOTION_SUTANRECOVER)
		pMotion->Set(pEnemyBoss->MOTION_SUTANRECOVER);

	// モーションが終了している
	if (pMotion->IsFinish())
	{
		Info->nLife += 100;
		pEnemyBoss->SetbDamage();

		// 移動状態に切り替える
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
	}
}

//================================================================
// 死亡状態の処理
//================================================================
//===========================================================
// 死亡状態のコンストラクタ
//===========================================================
CEnemyBossStateDeath::CEnemyBossStateDeath()
{

}

//===========================================================
// 死亡状態の更新処理
//===========================================================
void CEnemyBossStateDeath::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理

	//フェードの情報を取得
	CFade* pFade = CManager::GetInstance()->GetFade();

	if (m_bDeath)
	{
		if (pFade->Get() != pFade->FADE_OUT)
			pFade->Set(CScene::MODE_RESULT);

		return;
	}

	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションが死亡以外だったら
	if(pMotion->GetType() != CEnemyBoss::MOTION_DEATH)
		pMotion->Set(CEnemyBoss::MOTION_DEATH);

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		// 敵の総数を減らす
		int nNum = CEnemyManager::GetNum() - 1;
		CEnemyManager::SetNum(nNum);

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{// ゲームの時

			// 敵を倒した数を増やす
			int nDefeat = CManager::GetInstance()->GetDefeat();
			nDefeat++;
			CManager::GetInstance()->SetDefeat(nDefeat);
			m_bDeath = true;
			
		}
	}
}

//================================================================
// 電子レンジで死亡した状態の処理
//================================================================
//===========================================================
// 電子レンジで死亡した状態のコンストラクタ
//===========================================================
CEnemyBossStateDeathByMicrowave::CEnemyBossStateDeathByMicrowave()
{
}

//===========================================================
// 電子レンジで死亡した状態の更新処理
//===========================================================
void CEnemyBossStateDeathByMicrowave::Update(CEnemyBoss* pEnemyBoss)
{
	////フェードの情報を取得
	//CFade* pFade = CManager::GetInstance()->GetFade();

	//if (pFade->Get() != pFade->FADE_OUT)
	//	pFade->Set(CScene::MODE_RESULT);

	// 死亡した状態のとき処理を抜ける
	if (m_bDeath)
		return;

	// 敵の総数を減らす
	int nNum = CEnemyManager::GetNum() - 1;
	CEnemyManager::SetNum(nNum);

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{// ゲームの時

		// 敵を倒した数を増やす
		int nDefeat = CManager::GetInstance()->GetDefeat();
		nDefeat++;
		CManager::GetInstance()->SetDefeat(nDefeat);
		m_bDeath = true;
	}
}

//================================================================
// 捕まれている状態の処理
//================================================================
//===========================================================
// 捕まれている状態のコンストラクタ
//===========================================================
CEnemyBossStateGrabbed::CEnemyBossStateGrabbed()
{

}

//===========================================================
// 捕まれている状態の更新処理
//===========================================================
void CEnemyBossStateGrabbed::Update(CEnemyBoss* pEnemyBoss)
{

}

//================================================================
// 登場してから移動開始するまでの状態の処理
//================================================================
//===========================================================
// 移動開始待ち状態のコンストラクタ
//===========================================================
CEnemyBossAppear::CEnemyBossAppear()
{

}

//===========================================================
// 移動開始待ち状態の更新処理
//===========================================================
void CEnemyBossAppear::Update(CEnemyBoss* pEnemyBoss)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションがくびきり以外のとき
	if(pMotion->GetType() != pEnemyBoss->MOTION_KUGIKIRI)
	   pMotion->Set(pEnemyBoss->MOTION_KUGIKIRI);

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		pMotion->Set(pEnemyBoss->MOTION_ONSTEGE);
		pEnemyBoss->ChangeState(new CEnemyBossAppearMove);
	}
}

//================================================================
// 登場状態の処理
//================================================================
//===========================================================
// 登場状態のコンストラクタ
//===========================================================
CEnemyBossAppearMove::CEnemyBossAppearMove()
{

}

void CEnemyBossAppearMove::Update(CEnemyBoss* pEnemyBoss)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	CPlayer* pPlayer = CPlayer::GetInstance();
	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (pPlayer != nullptr)
		posPlayer = pPlayer->GetPosition();

	float fDiffmove = 0.0f;

	// 追尾
	fDiffmove = utility::MoveToPosition(Info->pos, posPlayer, Info->rot.y);

	// 角度補正
	fDiffmove = utility::CorrectAngle(fDiffmove);

	Info->rot.y += fDiffmove * 0.05f;

	// 角度補正
	Info->rot.y = utility::CorrectAngle(Info->rot.y);

	//移動量を更新(減衰させる)
	Info->move.x = sinf(Info->rot.y + D3DX_PI) * 2.0f;
	Info->move.z = cosf(Info->rot.y + D3DX_PI) * 2.0f;

	Info->pos += Info->move;

	float Distance = utility::Distance(Info->pos, posPlayer);

	if (Distance <= 400.0f)
	{
		pMotion->Set(CEnemyBoss::MOTION_KUGIKIRI);
		pEnemyBoss->ChangeState(new CEnemyBossOnStege);
		CCamera::GetInstance()->ChangeState(new CutSceneCamera);
	}
}

//================================================================
// 登場状態の処理
//================================================================
//===========================================================
// 登場状態のコンストラクタ
//===========================================================
CEnemyBossOnStege::CEnemyBossOnStege()
{
}

//===========================================================
// 登場状態の更新処理
//===========================================================
void CEnemyBossOnStege::Update(CEnemyBoss* pEnemyBoss)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	if (pMotion->IsFinish())
	{
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
		pMotion->Set(pEnemyBoss->MOTION_NEUTRAL);
	}
}

//================================================================
// 拒否状態の処理
//================================================================
//===========================================================
// 拒否状態のコンストラクタ
//===========================================================
CEnemyBossStateDenial::CEnemyBossStateDenial()
{

}

//===========================================================
// 拒否状態の更新処理
//===========================================================
void CEnemyBossStateDenial::Update(CEnemyBoss* pEnemyBoss)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションが拒否以外のとき
	if (pMotion->GetType() != pEnemyBoss->MOTION_DENIAL)
		pMotion->Set(pEnemyBoss->MOTION_DENIAL);

}