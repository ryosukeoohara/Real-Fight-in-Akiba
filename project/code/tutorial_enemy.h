//===========================================================
//
// 敵の処理 [tutorial_enemy.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _TUTORIAL_ENEMY_H_
#define _TUTORIAL_ENEMY_H_

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
class CTutorialEnemyState;
class CEnemy;

//===========================================================
// ザコ敵クラス定義
//===========================================================
class CTutorial_Enemy : public CEnemy
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

	CTutorial_Enemy();                                  //コンストラクタ
	CTutorial_Enemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //コンストラクタ
	~CTutorial_Enemy();                                 //デストラクタ

	HRESULT Init(void) override;                        //初期化
	void Uninit(void) override;                         //終了
	void Update(void) override;                         //更新
	void Draw(void) override;                           //描画

	static CTutorial_Enemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //生成
	void ChangeState(CTutorialEnemyState* pState);  // ステイトの切り替え

	void Damage(void) override;
	void Grabbed(void) override;
	void Denial(void) override;
	bool GetbDeathFlag(void) override;
	bool GetbHeatDamageFlag(void) override { return m_bHeatDamage; }

	// 設定系
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }

	void RestHeatDamageFrag(void) { m_bHeatDamage = false; }

private:

	CGage3D* m_pLife3D;            // ゲージのポインタ
	CTutorialEnemyState* m_pState;         // ステイト
	int m_nDamageCounter;          // ダメージ状態でいるカウント
	int m_nBiriBiriCount;
	int m_nAtcCounter;             // 攻撃のインターバル
	int m_nIdx;
	bool m_bDamage;               // 攻撃を受けたかどうか
	bool m_bHeatDamage;           // ヒートアクションをくらったかどうか
};

//=================================================================
// ステート
//=================================================================
class CTutorialEnemyState
{
public:
	CTutorialEnemyState() {};
	~CTutorialEnemyState() {};

	virtual void Update(CTutorial_Enemy* pEnemy) = 0;

private:

};

//===========================================================
// ニュートラル状態
//===========================================================
class CTutorialEnemyNeutral : public CTutorialEnemyState
{
public:
	CTutorialEnemyNeutral();
	~CTutorialEnemyNeutral() {}

	void Update(CTutorial_Enemy* pEnemy) override;

private:

};

//===========================================================
// 攻撃をくらった状態
//===========================================================
class CTutorialEnemyStateDamage : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateDamage();
	~CTutorialEnemyStateDamage() {};

	void Update(CTutorial_Enemy* pEnemy) override;

private:

	int m_nRecoverDamageTime = 0;  // ダメージ状態から復帰する時間

};

//===========================================================
// つよい攻撃をくらった状態
//===========================================================
class CTutorialEnemyStateHeavyDamage : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateHeavyDamage();
	~CTutorialEnemyStateHeavyDamage() {};

	void Update(CTutorial_Enemy* pEnemy) override;

private:

};

//===========================================================
// 起き上がり状態
//===========================================================
class CTutorialEnemyStateGetUp : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateGetUp();
	~CTutorialEnemyStateGetUp() {}

	void Update(CTutorial_Enemy* pEnemy) override;

private:

};

//===========================================================
// 捕まれている状態
//===========================================================
class CTutorialEnemyStateGrabbed : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateGrabbed();
	~CTutorialEnemyStateGrabbed() {}

	void Update(CTutorial_Enemy* pEnemy) override;

private:

};

//===========================================================
// 拒否状態
//===========================================================
class CTutorialEnemyStateDenial : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateDenial();
	~CTutorialEnemyStateDenial() {}

	void Update(CTutorial_Enemy* pEnemy) override;

private:

};

//===========================================================
// 死亡状態
//===========================================================
class CTutorialEnemyStateDeath : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateDeath();
	~CTutorialEnemyStateDeath() {};

	void Update(CTutorial_Enemy* pEnemy) override;

private:


};

#endif