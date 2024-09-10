//===========================================================
//
// ヒートアクション[heataction.h]
// Author 大原怜将
//
//===========================================================
#ifndef _HIEATACTION_H_
#define _HIEATACTION_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "player.h"

//===========================================================
// 前方宣言
//===========================================================
class CEnemy;
class CItem;
class CHeatActionBehaviour;

//===========================================================
// ヒートアクションクラス定義
//===========================================================
class CHeatAction
{
public:

	CHeatAction();
	~CHeatAction();
	void Init(void);
	void Uninit(void);
	void Updata(void);

	void BikeCrash(CPlayer* pPlayer, CEnemy* pEnemy);                         // 自転車を使用したアクション
	void MicroWave(CPlayer* pPlayer, CEnemy* pEnemy);                         // 電子レンジを使用したアクション
	void SetAction(CHeatActionBehaviour* Behaviour, CPlayer* pPlayer, CEnemy* pEnemy);  // アクションを設定

	// 取得系
	CPlayer* GetPlayer(void) { return m_pPlayer; }  // プレイヤー
	CEnemy* GetEnemy(void) { return m_pEnemy; }     // エネミー

private:

	void ChangeBehaviour(CHeatActionBehaviour* Behaviour);  // ビヘイビアの切り替え
	CHeatActionBehaviour *m_pBehaviour;
	CPlayer::HEAT m_HeatAct;
	CPlayer* m_pPlayer;
	CEnemy* m_pEnemy;

	int m_nHeatActTime;

	bool m_bInMicroWave = false;;  // ヒートアクション:電子レンジで敵が電子レンジの中に入ったかどうか
};

//===========================================================
// ビヘイビア
//===========================================================
class CHeatActionBehaviour
{
public:
	CHeatActionBehaviour() {}
	~CHeatActionBehaviour() {}

	virtual void Update(CHeatAction *pHeatAct) = 0;

private:

};

//===========================================================
// 自転車を使用したアクション
//===========================================================
class BikeCrash : public CHeatActionBehaviour
{
public:
	BikeCrash();
	~BikeCrash() {}

	void Update(CHeatAction* pHeatAct) override;

private:

};

//===========================================================
// 電子レンジを使用したアクション
//===========================================================
class MicroWave : public CHeatActionBehaviour
{
public:
	MicroWave();
	~MicroWave() {}

	void Update(CHeatAction* pHeatAct) override;

private:

	bool m_bInMicroWave = false;
	int m_nHeatActTime = 0;
};

#endif