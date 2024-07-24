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

//===========================================================
// ザコ敵クラス定義
//===========================================================
class CEnemyWeak : public CEnemy
{
public:

	CEnemyWeak();                                  //コンストラクタ
	CEnemyWeak(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife);                   //コンストラクタ
	~CEnemyWeak();                                 //デストラクタ

	HRESULT Init(void) override;                        //初期化
	void Uninit(void) override;                         //終了
	void Update(void) override;                         //更新
	void Draw(void) override;                           //描画

	static CEnemyWeak *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife);    //生成

	 // 設定系
	void SetChase(CHASE cha) { m_Chase = cha; }
	
private:

	// 敵の動き制御
	//void Controll(void) override;
	void Attack(void) override;
	void Move(void) override;
	void Damege(int damege, float blowaway, CPlayer::ATTACKTYPE act) override;
	//void MicroWave(void) override;

	CGage3D *m_pLife3D;                          // ゲージのポインタ

	int m_nDamegeCounter;                      // ダメージ状態でいるカウント
	int m_nBiriBiriCount;
	int m_nAtcCounter;                         // 攻撃のインターバル
	int m_nIdx;
};

#endif