//===========================================================
//
// 敵の処理 [enemy_weak.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _ENEMY_WEAK_FAR_H_
#define _ENEMY_WEAK_FAR_H_

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
class CEnemyWeakFarState;
class CEnemy;

//===========================================================
// ザコ敵クラス定義
//===========================================================
class CEnemyWeakFar : public CEnemy
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

	CEnemyWeakFar();                                  //コンストラクタ
	CEnemyWeakFar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //コンストラクタ
	~CEnemyWeakFar();                                 //デストラクタ

	HRESULT Init(void) override;                        // 初期化
	void Uninit(void) override;                         // 終了
	void Update(void) override;                         // 更新
	void Draw(void) override;                           // 描画

	static CEnemyWeakFar* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //生成
	void ChangeState(CEnemyWeakFarState* pState);  // ステイトの切り替え
	
	void Damege(void) override;
	void Grabbed(void) override;

	// 設定系
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }

private:

	CGage3D* m_pLife3D;            // ゲージのポインタ
	CEnemyWeakFarState* m_pState;         // ステイト
	int m_nDamegeCounter;          // ダメージ状態でいるカウント
	int m_nBiriBiriCount;
	int m_nAtcCounter;             // 攻撃のインターバル
	int m_nIdx;
	bool m_bDamage;               // 攻撃を受けたかどうか
};

//=================================================================
// ステート
//=================================================================
class CEnemyWeakFarState
{
public:
	CEnemyWeakFarState() {};
	~CEnemyWeakFarState() {};

	virtual void Update(CEnemyWeakFar* pEnemyWeak) = 0;

private:

};

//===========================================================
// 移動待機状態
//===========================================================
class CEnemyWeakFarStateMoveWait : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateMoveWait();
	~CEnemyWeakFarStateMoveWait() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// 移動状態
//===========================================================
class CEnemyWeakFarStateMove : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateMove();
	~CEnemyWeakFarStateMove() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// 攻撃待機状態
//===========================================================
class CEnemyWeakFarStateAttackWait : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateAttackWait();
	~CEnemyWeakFarStateAttackWait() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:
	int m_nAtcCounter = 0;  // 攻撃するまでのカウント

};

//===========================================================
// 攻撃状態
//===========================================================
class CEnemyWeakFarStateAttack : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateAttack();
	~CEnemyWeakFarStateAttack() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

	bool m_bAttack = false;

};

//===========================================================
// 攻撃をくらった状態
//===========================================================
class CEnemyWeakFarStateDamege : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateDamege();
	~CEnemyWeakFarStateDamege() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// つよい攻撃をくらった状態
//===========================================================
class CEnemyWeakFarStateHeavyDamege : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateHeavyDamege();
	~CEnemyWeakFarStateHeavyDamege() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// 起き上がり状態
//===========================================================
class CEnemyWeakFarStateGetUp : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateGetUp();
	~CEnemyWeakFarStateGetUp() {}

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// 捕まれている状態
//===========================================================
class CEnemyWeakFarStateGrabbed : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateGrabbed();
	~CEnemyWeakFarStateGrabbed() {}

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// 死亡状態
//===========================================================
class CEnemyWeakFarStateDeath : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateDeath();
	~CEnemyWeakFarStateDeath() {}

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:


};

#endif