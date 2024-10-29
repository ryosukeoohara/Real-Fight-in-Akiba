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
class CHeatActionState;

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

	void SetAction(CHeatActionState* pState, CPlayer* pPlayer, CEnemy* pEnemy);  // アクションを設定

	// 取得系
	CPlayer* GetPlayer(void) { return m_pPlayer; }  // プレイヤー
	CEnemy* GetEnemy(void) { return m_pEnemy; }     // 敵

private:

	void ChangeState(CHeatActionState* pState);  // ビヘイビアの切り替え
	CHeatActionState *m_pState;                  // ステート
	CPlayer* m_pPlayer;                          // プレイヤーのポインタ
	CEnemy* m_pEnemy;                            // 敵のポインタ
};

//===========================================================
// ヒートアクションのステイト
//===========================================================
class CHeatActionState
{
public:
	CHeatActionState() {}
	~CHeatActionState() {}

	virtual void Update(CHeatAction *pHeatAct) = 0;

private:

};

//===========================================================
// 自転車を使用したアクション
//===========================================================
class BikeCrash : public CHeatActionState
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
class MicroWave : public CHeatActionState
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