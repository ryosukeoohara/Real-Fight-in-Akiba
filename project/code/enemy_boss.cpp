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
	const int DAMEGECOUNT = 10;         // ダメージ状態
	const float ATTACKLENGTH = 120.0f;  // 攻撃可能な距離
	const float SPEED = 2.0f;           // 走る速さ
	const float ATTACKLENGHT = 100.0f;   // 攻撃可能範囲
	const int STAGGER_TIME = 600;       // よろめき状態でいる時間

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
	m_bDamage = false;
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
	m_bDamage = false;
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

	/*if (Info->nLife <= 0 && Info->state != STATE_DEATH)
	{
		Info->state = CEnemy::STATE_DEATH;
		pMotion->Set(CEnemy::MOTION_DEATH);
		ChangeState(new CEnemyBossStateDeath);
	}*/

	//キーボードを取得
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	if (InputKeyboard->GetTrigger(DIK_8))
	{
		Info->nLife = 51;
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

	if(pPlayer->GetbGrap() == true)
	   ChangeState(new CEnemyBossStateGrabbed);

	else if(pPlayer->GetbGrap() == false)
		ChangeState(new CEnemyBossStateMove);
}

//===========================================================
// ダメージ処理
//===========================================================
void CEnemyBoss::Damege(void)
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

	if (Info->nLife >= 30 && CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_NONE)
	{// 体力が５０以上のとき

		// ダメージ状態に切り替える
		ChangeState(new CEnemyBossStateDamege);
	}
	if (Info->nLife >= 30 && (CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_FIRE || CPlayer::GetInstance()->GetHeatAct() == CPlayer::HEAT_SMASH))
	{// 体力が５０以上のとき

		// ダメージ状態に切り替える
		ChangeState(new CEnemyBossStateHeavyDamege);
	}
	else if (Info->nLife < 30 && Info->nLife > 0)
	{// 体力が５０未満かつ0より高いとき
		
		// よろめき状態に切り替える
		ChangeState(new CEnemyBossStateKnockDown);
	}
	else if (Info->nLife <= 0)
	{// 体力が０以下のとき

		// 死亡状態に切り替える
		CGame::GetInstance()->SetbFinish(true);
		CCamera::GetInstance()->ChangeState(new FinalBlowCamera);
		ChangeState(new CEnemyBossStateDeath);
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
			CGame::GetPlayer()->Damage(GetMotion()->GetAttackDamege(), D3DXVECTOR3(sinf(GetRotition().y) * GetMotion()->GetKnockBack(), 0.0f, cosf(GetRotition().y) * GetMotion()->GetKnockBack()));
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

	// プレイヤーとの距離
	float fLenght = utility::Distance(Info->pos, PlayerPos);

	if (fLenght <= ATTACKLENGHT)
	{// 攻撃可能範囲内に入った

		Info->state = CEnemy::STATE_NEUTRAL;
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
	if(!m_bAttack)
	   m_nAtcCounter++;
	
	if (m_nAtcCounter >= 60)
	{
		m_nAtcCounter = 0;
		m_bAttack = true;

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
	else if(m_nAtcCounter == 0 && m_bAttack == true)
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

	if (pMotion->IsFinish())
	{
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
		m_bAttack = false;
	}
		
	// 攻撃しているときは処理を抜ける
	if (m_bAttack)
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
CEnemyBossStateDamege::CEnemyBossStateDamege()
{
}

//===========================================================
// ダメージ状態の更新処理
//===========================================================
void CEnemyBossStateDamege::Update(CEnemyBoss* pEnemyBoss)
{// 更新処理

	// 敵の情報取得
	CEnemy::INFO* Info = pEnemyBoss->GetInfo();

	if (Info == nullptr)
		return;

	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		pMotion->Set(pEnemyBoss->MOTION_NEUTRAL);
		pEnemyBoss->ChangeState(new CEnemyBossStateMove);
		pEnemyBoss->SetbDamage();
	}
}

//================================================================
// 強い攻撃を受けた状態の処理
//================================================================
//===========================================================
// 強い攻撃を受けた状態のコンストラクタ
//===========================================================
CEnemyBossStateHeavyDamege::CEnemyBossStateHeavyDamege()
{
}

//===========================================================
// 強い攻撃を受けた状態の更新処理
//===========================================================
void CEnemyBossStateHeavyDamege::Update(CEnemyBoss* pEnemyBoss)
{
	// モーションの情報取得
	CMotion* pMotion = pEnemyBoss->GetMotion();

	if (pMotion == nullptr)
		return;

	// モーションがノックダウン以外のとき
	if (pMotion->GetType() != pEnemyBoss->MOTION_HEATDAMEGE)
		pMotion->Set(pEnemyBoss->MOTION_HEATDAMEGE);

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
		pEnemyBoss->ChangeState(new CEnemyBossStateGetUp);
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
	if (pMotion->GetType() != pEnemyBoss->MOTION_HEATDAMEGE)
		pMotion->Set(pEnemyBoss->MOTION_HEATDAMEGE);

	// モーションが終了していたら
	if (pMotion->IsFinish())
	{
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
		//pEnemyBoss->Uninit();

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
		//CManager::GetInstance()->GetCamera()->ChangeState(new FollowPlayerCamera);
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

CEnemyBossStateGrabbed::CEnemyBossStateGrabbed()
{
}

void CEnemyBossStateGrabbed::Update(CEnemyBoss* pEnemyBoss)
{
}
