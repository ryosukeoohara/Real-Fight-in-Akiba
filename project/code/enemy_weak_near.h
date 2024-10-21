//===========================================================
//
// 敵の処理 [enemy_weak.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _ENEMY_WEAK_NEAR_H_
#define _ENEMY_WEAK_NEAR_H_

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
class CEnemyWeakNearState;
class CEnemy;

//===========================================================
// ザコ敵クラス定義
//===========================================================
class CEnemyWeakNear : public CEnemy
{
public:

	// モーション
	enum MOTIONTYPE
	{
		MOTION_NEUTRAL = 0,          // 待機
		MOTION_DASH,                 // 移動
		MOTION_ATTACK,               // 攻撃
		MOTION_BATTLE_NEUTRAL,       // 先頭ニュートラル
		MOTION_DAMEGE,               // 攻撃受けた
		MOTION_GRAP,                 // 投げられ待ち
		MOTION_FALLDOWN,             // 転ぶ

		MOTION_HEATACTELECTROWAIT,   // ヒートアクション:電子レンジ待機
		MOTION_HEATACTELECTRO,       // ヒートアクション:電子レンジびりびり
		MOTION_HEATACTFAINTING,      // ヒートアクション:電子レンジ気絶

		MOTION_GETUP,                // 起き上がり
		MOTION_DEATH,                // 死亡

		MOTION_PAINFULDAMAGE,        // 痛い攻撃を受けた
		MOTION_HEATDAMEGE,           // 攻撃受けた
		MOTION_MAX
	};

	CEnemyWeakNear();                                  //コンストラクタ
	CEnemyWeakNear(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //コンストラクタ
	~CEnemyWeakNear();                                 //デストラクタ

	HRESULT Init(void) override;                        // 初期化
	void Uninit(void) override;                         // 終了
	void Update(void) override;                         // 更新
	void Draw(void) override;                           // 描画

	static CEnemyWeakNear* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //生成
	void ChangeState(CEnemyWeakNearState* pState);  // ステイトの切り替え

	void Damege(void) override;
	void Grabbed(void) override;

	// 設定系
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }

private:

	CGage3D* m_pLife3D;            // ゲージのポインタ
	CEnemyWeakNearState* m_pState;         // ステイト
	int m_nDamegeCounter;          // ダメージ状態でいるカウント
	int m_nBiriBiriCount;
	int m_nAtcCounter;             // 攻撃のインターバル
	int m_nIdx;
	bool m_bDamage;               // 攻撃を受けたかどうか
};

//=================================================================
// ステート
//=================================================================
class CEnemyWeakNearState
{
public:
	CEnemyWeakNearState() {};
	~CEnemyWeakNearState() {};

	virtual void Update(CEnemyWeakNear* pEnemyWeak) = 0;

private:

};

//===========================================================
// 移動待機状態
//===========================================================
class CEnemyWeakNearStateMoveWait : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateMoveWait();
	~CEnemyWeakNearStateMoveWait() {}

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// 移動状態
//===========================================================
class CEnemyWeakNearStateMove : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateMove();
	~CEnemyWeakNearStateMove() {};

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// 攻撃待機状態
//===========================================================
class CEnemyWeakNearStateAttackWait : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateAttackWait();
	~CEnemyWeakNearStateAttackWait() {};

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:
	int m_nAtcCounter = 0;  // 攻撃するまでのカウント

};

//===========================================================
// 攻撃状態
//===========================================================
class CEnemyWeakNearStateAttack : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateAttack();
	~CEnemyWeakNearStateAttack() {};

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// 攻撃をくらった状態
//===========================================================
class CEnemyWeakNearStateDamege : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateDamege();
	~CEnemyWeakNearStateDamege() {};

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// つよい攻撃をくらった状態
//===========================================================
class CEnemyWeakNearStateHeavyDamege : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateHeavyDamege();
	~CEnemyWeakNearStateHeavyDamege() {};

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// 起き上がり状態
//===========================================================
class CEnemyWeakNearStateGetUp : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateGetUp();
	~CEnemyWeakNearStateGetUp() {}

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// 捕まれている状態
//===========================================================
class CEnemyWeakNearStateGrabbed : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateGrabbed();
	~CEnemyWeakNearStateGrabbed() {}

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// 死亡状態
//===========================================================
class CEnemyWeakNearStateDeath : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateDeath();
	~CEnemyWeakNearStateDeath() {}

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

#endif