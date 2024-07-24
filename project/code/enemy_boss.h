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

//===========================================================
// ボス敵クラス定義
//===========================================================
class CEnemyBoss : public CEnemy
{
public:

	//モーション
	enum MOTIONTYPE
	{
		TYPE_PUNCH = CEnemy::MOTIONTYPE::TYPE_MAX,                    // パンチ
		TYPE_GURUGURU,                 // ぐるぐるパンチ
		TYPE_MAX
	};

	enum ATTACKTYPE
	{
		ATTACKTYPE_GURUGURU = 0,
		ATTACKTYPE_PUNCH,
		ATTACKTYPE_FLY,
		ATTACKTYPE_MAX
	};

public:

	CEnemyBoss();                                  //コンストラクタ
	CEnemyBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife);                   //コンストラクタ
	~CEnemyBoss();                                 //デストラクタ

	HRESULT Init(void) override;                        //初期化
	void Uninit(void) override;                         //終了
	void Update(void) override;                         //更新
	void Draw(void) override;                           //描画

	static CEnemyBoss *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife);    //生成

	// 設定系
	void SetChase(CHASE cha) { m_Chase = cha; }

private:

	// 敵の動き制御
	//void Controll(void) override;
	void Attack(void) override;
	void Move(void) override;
	void Damege(int damege, float blowaway, CPlayer::ATTACKTYPE act) override;
	void RollingPunch(void);
	void NormalPunch(void);
	void Fly(void);
	void HitDetection(D3DXVECTOR3 MyPos, float attackrange, float targetradius);
	//void MicroWave(void) override;

	CGage2D *m_pLife2D;                          // ゲージのポインタ

	int m_nDamegeCounter;         // ダメージ状態でいるカウント
	int m_nBiriBiriCount;         // 電子レンジびりびりカウント
	int m_nAtcCounter;            // 攻撃のインターバル
	int m_nReceivedAttack;        // 攻撃を受けた回数
	int m_nAttackType;
	int m_nIdx;
	

};

#endif // !_ENEMY_H_