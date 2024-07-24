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
#include <time.h>
#include  <assert.h>

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const int DAMEGE = 10;             // ダメージ状態
	const int ATTACKAGAINCOUNT = 60;   // 再攻撃できるまでの時間
	const float SPEED = 2.0f;          // 走る速さ
	const float ATTACKLENGHT = 50.0f;  // 攻撃可能範囲
	const float SEARCHRANGE = 400.0f;  // 探索範囲
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
}

//===========================================================
// コンストラクタ
//===========================================================
CEnemyWeak::CEnemyWeak(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife)
{
	// 値をクリア
	SetPosition(pos);
	SetRotition(rot);
	SetLife(nlife);
	SetState(CEnemy::STATE_NONE);
	m_pLife3D = nullptr;
	m_nBiriBiriCount = 0;
	m_Chase = CHASE_ON;
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
CEnemyWeak * CEnemyWeak::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife)
{
	CEnemyWeak *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		pEnemy = new CEnemyWeak(pos, rot, nlife);

		pEnemy->Init();
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

	ReadText(TEXT_NAME);

	if (m_pLife3D == nullptr)
	{
		m_pLife3D = CGage3D::Create(D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y, m_Info.pos.z), 5.0f, (float)((m_Info.nLife * 0.01f) * 20), CGage3D::TYPE_LIFE);
		m_pLife3D->SetPos(&m_Info.pos);
		m_pLife3D->SetUpHeight(80.0f);
		m_pLife3D->GetBill()->SetEdgeCenter((float)((m_Info.nLife * 0.01f) * 20), 5.0f);
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

	CObject::Release();
}

//===========================================================
// 更新処理
//===========================================================
void CEnemyWeak::Update(void)
{
	CEnemy::Update();

	if (m_pLife3D != nullptr)
	{
		m_pLife3D->GetBill()->SetEdgeCenter((float)((m_Info.nLife * 0.01f) * 20), 5.0f);
	}
}

//===========================================================
// 描画処理
//===========================================================
void CEnemyWeak::Draw(void)
{
	CEnemy::Draw();
}

//===========================================================
// 制御処理
//===========================================================
void CEnemyWeak::Attack(void)
{
	//プレイヤーの情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	// プレイヤーが使用されていなかった場合処理を抜ける
	if (pPlayer == nullptr)
		return;

	// モーションが使用されていなかった場合処理を抜ける
	if (GetMotion() == nullptr)
		return;

	// 当たり判定が使用されていなかった場合処理を抜ける
	if (CGame::GetCollision() == nullptr)
		return;
	
	if (m_Info.state != STATE_ATTACK)
	{
		m_nAtcCounter++;

		if (m_nAtcCounter >= ATTACKAGAINCOUNT)
		{
			m_nAtcCounter = 0;

			// 攻撃状態にする
			m_Info.state = STATE_ATTACK;
			GetMotion()->Set(TYPE_ATTACK);

			if (GetMotion()->GetAttackOccurs() <= GetMotion()->GetNowFrame() && GetMotion()->GetAttackEnd() >= GetMotion()->GetNowFrame())
			{// 現在のフレームが攻撃判定発生フレーム以上かつ攻撃判定終了フレームない

				if (CGame::GetCollision()->Circle(m_Info.pos, pPlayer->GetPosition(), ATTACKLENGHT, pPlayer->GetRadius()) == true)
				{
					pPlayer->Damage(GetMotion()->GetAttackDamege(), GetMotion()->GetKnockBack());
				}
			}
		}
	}
}

//===========================================================
// 制御処理
//===========================================================
void CEnemyWeak::Move(void)
{
	//プレイヤーの情報取得
	CPlayer *pPlayer = CPlayer::GetInstance();

	// プレイヤーが使用されていなかった場合処理を抜ける
	if (pPlayer == nullptr)
		return;

	if (CGame::GetCollision() == nullptr)
		return;
	
	if (m_Chase == CHASE_ON && CGame::GetCollision()->Circle(m_Info.pos, pPlayer->GetPosition(), SEARCHRANGE, pPlayer->GetRadius()) == true)
	{//円の中にプレイヤーが入った

		m_Info.posOld = m_Info.pos;

		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		float fDiffmove = 0.0f;

		if (m_Info.state != STATE_DAMEGE)
		{
			// 追尾
			fDiffmove = utility::MoveToPosition(m_Info.pos, PlayerPos, m_Info.rot.y);

			// 角度補正
			fDiffmove = utility::CorrectAngle(fDiffmove);

			m_Info.rot.y += fDiffmove * 0.05f;

			// 角度補正
			m_Info.rot.y = utility::CorrectAngle(m_Info.rot.y);

			//移動量を更新(減衰させる)
			m_Info.move.x = sinf(m_Info.rot.y + D3DX_PI) * SPEED;
			m_Info.move.z = cosf(m_Info.rot.y + D3DX_PI) * SPEED;
		}

		// プレイヤーとの距離
		float fLenght = utility::Distance(m_Info.pos, PlayerPos);

		if (fLenght <= ATTACKLENGHT)
		{// 攻撃可能範囲内に入った

			if (m_Info.state != STATE_NEUTRAL && m_Info.state != STATE_ATTACK && m_Info.state != STATE_DAMEGE && m_Info.state != STATE_PAINFULDAMAGE)
			{
				m_Info.state = STATE_NEUTRAL;
				GetMotion()->Set(TYPE_NEUTRAL);
			}

			// 移動をやめて攻撃する
			Attack();
			m_Info.move.x = 0.0f;
			m_Info.move.z = 0.0f;
		}
		else
		{// 範囲外

			if (m_Info.state != STATE_DASH && m_Info.state != STATE_DAMEGE && m_Info.state != STATE_PAINFULDAMAGE)
			{// プレイヤーを追いかけていないかつダメージ状態ではないとき

				m_Info.state = STATE_DASH;
				GetMotion()->Set(TYPE_DASH);
			}
		}
	}
	else
	{// プレイヤーが円の中にいない

		m_Info.move.x = 0.0f;
		m_Info.move.z = 0.0f;

		// 待機状態にする
		if (m_Info.state != STATE_NEUTRAL)
		{
			m_Info.state = STATE_NEUTRAL;
			GetMotion()->Set(TYPE_NEUTRAL);
		}
	}
	
	if (m_Info.state == STATE_PAINFULDAMAGE && GetMotion()->GetNowFrame() >= 0 && GetMotion()->GetNowFrame() <= 20)
	{
		m_Info.move.x = 0.0f;
		m_Info.move.z = 0.0f;
	}
}

//===========================================================
// 制御処理
//===========================================================
void CEnemyWeak::Damege(int damege, float blowaway, CPlayer::ATTACKTYPE act)
{
	if (m_Info.state != STATE_DAMEGE && m_Info.state != STATE_HEATDAMEGE && m_Info.state != STATE_PAINFULDAMAGE && m_Info.state != STATE_DEATH)
	{
		m_Info.nLife -= damege;
		m_Info.move = D3DXVECTOR3(sinf(CPlayer::GetInstance()->GetRotition().y) * -blowaway, blowaway, cosf(CPlayer::GetInstance()->GetRotition().y) * -blowaway);

		if (act == CPlayer::ATTACKTYPE::TYPE_HEATACTBIKE || act == CPlayer::ATTACKTYPE::TYPE_HEATACTREF || act == CPlayer::ATTACKTYPE::TYPE_HEATACTMICROWAVE)
		{
			if (m_Info.state != STATE_HEATDAMEGE)
			{
				m_Info.state = STATE_HEATDAMEGE;
				GetMotion()->Set(TYPE_HEATACTDAMEGE);
			}
		}
		else
		{
			if (m_Info.state != STATE_DAMEGE)
			{
				int a = rand() % 60;
				if (m_Info.nLife <= a && CPlayer::GetInstance()->GetActType() == CPlayer::TYPE_ATTACK3)
				{
					m_Info.state = STATE_PAINFULDAMAGE;
					GetMotion()->Set(TYPE_HEATACTDAMEGE);
					m_Chase = CHASE_OFF;
				}
				else
				{
					m_Info.state = STATE_DAMEGE;
					GetMotion()->Set(TYPE_DAMEGE);
				}
			}
		}

		CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_PUNCH);
	}
}