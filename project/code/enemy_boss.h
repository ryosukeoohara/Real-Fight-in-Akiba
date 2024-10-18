//===========================================================
//
// 敵の処理 [enemy.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _ENEMY_BOSS_H_
#define _ENEMY_BOSS_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "enemy.h"
#include "player.h"

//===========================================================
// 前方宣言
//===========================================================
class CMotion;
class CCharacter;
class CGage2D;
class CEnemyBossState;
class CEnemy;
class CAttackType;

//===========================================================
// ボス敵クラス定義
//===========================================================
class CEnemyBoss : public CEnemy
{
public:

	// 状態
	enum STATE
	{
		STATE_NONE = 0,             // なんもない
		STATE_NEUTRAL,              // 待機
		STATE_DASH,                 // 移動
		STATE_WALK,                 // 移動
		STATE_ATTACK,               // 攻撃
		STATE_GRAP,                 // 投げられ待ち
		STATE_DAMEGE,               // 攻撃受けた
		STATE_PAINFULDAMAGE,        // 痛い攻撃を受けた
		STATE_HEATDAMEGE,           // 攻撃受けた
		STATE_GETUP,                // 起き上がり
		STATE_HEATACTELECTROWAIT,   // ヒートアクション:電子レンジ待機
		STATE_HEATACTELECTRO,       // ヒートアクション:電子レンジびりびり
		STATE_HEATACTFAINTING,      // ヒートアクション:電子レンジ気絶
		STATE_FALLDOWN,             // 転ぶ
		STATE_ONSTEGE,
		STATE_DEATH,                // 死亡
		STATE_MAX
	};

	// モーション
	enum MOTIONTYPE
	{
		MOTION_NEUTRAL = 0,          // 待機
		MOTION_WALK,                 // 移動
		MOTION_ATTACK,               // 攻撃
		MOTION_DAMEGE,               // 攻撃受けた
		MOTION_GRAP,                 // 投げられ待ち
		MOTION_HEATDAMEGE,           // 攻撃受けた
		MOTION_HEATACTELECTROWAIT,   // ヒートアクション:電子レンジ待機
		MOTION_HEATACTELECTRO,       // ヒートアクション:電子レンジびりびり
		MOTION_HEATACTFAINTING,      // ヒートアクション:電子レンジ気絶MOTIONE_PAINFULDAMAGE,        // 痛い攻撃を受けた
		MOTION_GETUP,                // 起き上がり
		MOTION_DEATH,                // 死亡
		MOTION_FALLDOWN,             // 転ぶ
		MOTION_GURUGURU,              // ぐるぐるパンチ
		MOTION_PUNCH,                 // パンチ
		MOTION_SUTAN,                 // スタン
		MOTION_SUTANRECOVER,          // スタンからの復帰
		MOTION_ONSTEGE,
		MOTION_KUGIKIRI,
		MOTION_MAX
	};

	

public:

	CEnemyBoss();                                  //コンストラクタ
	CEnemyBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //コンストラクタ
	~CEnemyBoss();                                 //デストラクタ

	HRESULT Init(void) override;                        //初期化
	void Uninit(void) override;                         //終了
	void Update(void) override;                         //更新
	void Draw(void) override;                           //描画

	static CEnemyBoss *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //生成
	void ChangeState(CEnemyBossState* pState);  // ステイトの切り替え
	void RecoverFromDamage(void) override;

	// 設定系
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }

	void Grabbed(void) override;
	void RollingPunch(void);
	void NormalPunch(void);
	void Fly(void);

private:

	// 敵の動き制御
	//void Controll(void) override;
	
	void Damege(void) override;
	void HardDamege(void) override;
	
	void HitDetection(D3DXVECTOR3 MyPos, float attackrange, float targetradius);
	//void MicroWave(void) override;

	CGage2D *m_pLife2D;           // ゲージのポインタ
	CEnemyBossState* m_pState;         // ステイト
	int m_nDamegeCounter;         // ダメージ状態でいるカウント
	int m_nBiriBiriCount;         // 電子レンジびりびりカウント
	int m_nAtcCounter;            // 攻撃のインターバル
	int m_nReceivedAttack;        // 攻撃を受けた回数
	
	int m_nIdx;
	bool m_bDamage;               // 攻撃を受けたかどうか
	

};

//===========================================================
// ボスの行動ステート
//===========================================================
class CEnemyBossState
{
public:
	CEnemyBossState();
	~CEnemyBossState() {}

	virtual void Update(CEnemyBoss* pEnemyBoss) = 0;

private:
	
};

//===========================================================
// ステージ登場状態
//===========================================================
class CEnemyBossOnStege : public CEnemyBossState
{
public:
	CEnemyBossOnStege();
	~CEnemyBossOnStege() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// 出現してから移動開始するまでの状態
//===========================================================
class CEnemyBossAppear : public CEnemyBossState
{
public:
	CEnemyBossAppear();
	~CEnemyBossAppear() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// 出現してから移動開始するまでの状態
//===========================================================
class CEnemyBossAppearMove : public CEnemyBossState
{
public:
	CEnemyBossAppearMove();
	~CEnemyBossAppearMove() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// 攻撃している状態
//===========================================================
class CEnemyBossStateAttack : public CEnemyBossState
{
public:
	CEnemyBossStateAttack();
	~CEnemyBossStateAttack() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

	//void ChangeState(CAttackType* pAttackType);

private:

	CAttackType* m_pAttackType = nullptr;

};

//===========================================================
// 移動待機状態
//===========================================================
class CEnemyBossStateMoveWait : public CEnemyBossState
{
public:
	CEnemyBossStateMoveWait();
	~CEnemyBossStateMoveWait() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// 移動状態
//===========================================================
class CEnemyBossStateMove : public CEnemyBossState
{
public:
	CEnemyBossStateMove();
	~CEnemyBossStateMove() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// 攻撃待機状態
//===========================================================
class CEnemyBossStateAttackWait : public CEnemyBossState
{
public:
	CEnemyBossStateAttackWait();
	~CEnemyBossStateAttackWait() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

	enum ATTACKTYPE
	{
		ATTACKTYPE_GURUGURU = 0,
		ATTACKTYPE_PUNCH,
		ATTACKTYPE_FLY,
		ATTACKTYPE_MAX
	};

private:
	int m_nAtcCounter = 0;  // 攻撃するまでのカウント
	int m_nAttackType = 0;
	bool m_bAttack = false;
};

//===========================================================
// 攻撃をくらった状態
//===========================================================
class CEnemyBossStateDamege : public CEnemyBossState
{
public:
	CEnemyBossStateDamege();
	~CEnemyBossStateDamege() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// つよい攻撃をくらった状態
//===========================================================
class CEnemyBossStateHeavyDamege : public CEnemyBossState
{
public:
	CEnemyBossStateHeavyDamege();
	~CEnemyBossStateHeavyDamege() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// 捕まれている状態
//===========================================================
class CEnemyBossStateGrabbed : public CEnemyBossState
{
public:
	CEnemyBossStateGrabbed();
	~CEnemyBossStateGrabbed() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// 死亡状態
//===========================================================
class CEnemyBossStateDeath : public CEnemyBossState
{
public:
	CEnemyBossStateDeath();
	~CEnemyBossStateDeath() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// よろめき状態
//===========================================================
class CEnemyBossStateStagger : public CEnemyBossState
{
public:
	CEnemyBossStateStagger();
	~CEnemyBossStateStagger() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

	int m_nStaggerTime = 0;  // よろめき状態でいる時間
};

//===========================================================
// よろめき状態からの復帰
//===========================================================
class CEnemyBossStateStaggerReCover : public CEnemyBossState
{
public:
	CEnemyBossStateStaggerReCover();
	~CEnemyBossStateStaggerReCover() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

	int m_nStaggerTime = 0;  // よろめき状態でいる時間
};

//===========================================================
// 転ぶ状態
//===========================================================
class CEnemyBossStateKnockDown : public CEnemyBossState
{
public:
	CEnemyBossStateKnockDown();
	~CEnemyBossStateKnockDown() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// 起き上がり状態
//===========================================================
class CEnemyBossStateGetUp : public CEnemyBossState
{
public:
	CEnemyBossStateGetUp();
	~CEnemyBossStateGetUp() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// ボスの行動ステイト
//===========================================================
class CAttackType
{
public:
	CAttackType();
	~CAttackType() {};

	virtual void Update(CEnemy* pEnemy) = 0;

private:

};

class CAttackTypeNormalPunch : public CAttackType
{
public:
	CAttackTypeNormalPunch();
	~CAttackTypeNormalPunch() {};

	void Update(CEnemy* pEnemy) override;

private:

};

class CAttackTypeRollingPunch : public CAttackType
{
public:
	CAttackTypeRollingPunch();
	~CAttackTypeRollingPunch() {};

	void Update(CEnemy* pEnemy) override;

private:

};

class CAttackTypeBodyPress : public CAttackType
{
public:
	CAttackTypeBodyPress();
	~CAttackTypeBodyPress() {};

	void Update(CEnemy* pEnemy) override;

private:

};

#endif // !_ENEMY_H_