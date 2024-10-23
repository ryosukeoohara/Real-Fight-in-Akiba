//===========================================================
//
// 敵の処理 [enemy_weak.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _ENEMY_WEAK_H_
#define _ENEMY_WEAK_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "enemy.h"

//===========================================================
// 前方宣言
//===========================================================
class CMotion;
class CCharacter;
class CGage3D;
class CEnemyWeakState;
class CEnemy;

//===========================================================
// ザコ敵クラス定義
//===========================================================
class CEnemyWeak : public CEnemy
{
public:

	// モーション
	enum MOTIONTYPE
	{
		MOTION_NEUTRAL = 0,          // 待機
		MOTION_DASH,                 // 移動
		MOTION_ATTACK,               // 攻撃
		MOTION_DAMAGE,               // 攻撃受けた
		MOTION_GRAP,                 // 投げられ待ち
		MOTION_FALLDOWN,             // 転ぶ

		MOTION_HEATACTELECTROWAIT,   // ヒートアクション:電子レンジ待機
		MOTION_HEATACTELECTRO,       // ヒートアクション:電子レンジびりびり
		MOTION_HEATACTFAINTING,      // ヒートアクション:電子レンジ気絶

		MOTION_GETUP,                // 起き上がり
		MOTION_DEATH,                // 死亡
		MOTION_DENIAL,               // ヒートアクション：自転車受け待ち

		MOTION_PAINFULDAMAGE,        // 痛い攻撃を受けた
		MOTION_HEATDAMAGE,           // 攻撃受けた
		MOTION_MAX
	};

	CEnemyWeak();                                  //コンストラクタ
	CEnemyWeak(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //コンストラクタ
	~CEnemyWeak();                                 //デストラクタ

	HRESULT Init(void) override;                        //初期化
	void Uninit(void) override;                         //終了
	void Update(void) override;                         //更新
	void Draw(void) override;                           //描画

	static CEnemyWeak *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //生成
	void ChangeState(CEnemyWeakState* pState);  // ステイトの切り替え
	void DashEffect(void);         // 走っているときに出すエフェクト
	
	void Damage(void) override;
	void Grabbed(void) override;
	void Denial(void) override;
	bool GetbDeathFlag(void) override { return m_bDeath; }
	bool GetbHeatDamageFlag(void) override { return m_bHeatDamage; }
	bool GetbStaggerFlag(void) override { return m_bStagger; }


	 // 設定系
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }
	void RestHeatDamageFrag(void) { m_bHeatDamage = false; }
	

private:

	CGage3D *m_pLife3D;            // ゲージのポインタ
	CEnemyWeakState* m_pState;         // ステイト
	int m_nDamageCounter;          // ダメージ状態でいるカウント
	int m_nBiriBiriCount;
	int m_nAtcCounter;             // 攻撃のインターバル
	int m_nIdx;
	bool m_bDamage;               // 攻撃を受けたかどうか
	bool m_bHeatDamage;           // ヒートアクションをくらったかどうか
	bool m_bDeath;                // 死亡フラグ
	bool m_bStagger;              // よろけフラグ
};

//=================================================================
// ステート
//=================================================================
class CEnemyWeakState
{
public:
	CEnemyWeakState() {};
	~CEnemyWeakState() {};

	virtual void Update(CEnemyWeak* pEnemyWeak) = 0;

private:

};

//===========================================================
// 移動待機状態
//===========================================================
class CEnemyWeakStateMoveWait : public CEnemyWeakState
{
public:
	CEnemyWeakStateMoveWait();
	~CEnemyWeakStateMoveWait() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// 移動状態
//===========================================================
class CEnemyWeakStateMove : public CEnemyWeakState
{
public:
	CEnemyWeakStateMove();
	~CEnemyWeakStateMove() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// 攻撃待機状態
//===========================================================
class CEnemyWeakStateAttackWait : public CEnemyWeakState
{
public:
	CEnemyWeakStateAttackWait();
	~CEnemyWeakStateAttackWait() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:
	int m_nAtcCounter = 0;  // 攻撃するまでのカウント

};

//===========================================================
// 攻撃状態
//===========================================================
class CEnemyWeakStateAttack : public CEnemyWeakState
{
public:
	CEnemyWeakStateAttack();
	~CEnemyWeakStateAttack() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// 攻撃をくらった状態
//===========================================================
class CEnemyWeakStateDamage : public CEnemyWeakState
{
public:
	CEnemyWeakStateDamage();
	~CEnemyWeakStateDamage() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

	int m_nRecoverDamageTime = 0;  // ダメージ状態から復帰する時間

};

//===========================================================
// つよい攻撃をくらった状態
//===========================================================
class CEnemyWeakStateHeavyDamage : public CEnemyWeakState
{
public:
	CEnemyWeakStateHeavyDamage();
	~CEnemyWeakStateHeavyDamage() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// 起き上がり状態
//===========================================================
class CEnemyWeakStateGetUp : public CEnemyWeakState
{
public:
	CEnemyWeakStateGetUp();
	~CEnemyWeakStateGetUp() {}

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// 捕まれている状態
//===========================================================
class CEnemyWeakStateGrabbed : public CEnemyWeakState
{
public:
	CEnemyWeakStateGrabbed();
	~CEnemyWeakStateGrabbed() {}

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// 拒否状態
//===========================================================
class CEnemyWeakStateDenial : public CEnemyWeakState
{
public:
	CEnemyWeakStateDenial();
	~CEnemyWeakStateDenial() {}

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// 死亡状態
//===========================================================
class CEnemyWeakStateDeath : public CEnemyWeakState
{
public:
	CEnemyWeakStateDeath();
	~CEnemyWeakStateDeath() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

	
};

#endif