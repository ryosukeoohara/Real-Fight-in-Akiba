//===========================================================
//
//ポリゴンを出すやつ[game.h]
//Author 大原怜将
//
//===========================================================
#ifndef _GAME_H_
#define _GAME_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "manager.h"

//===========================================================
// 前方宣言
//===========================================================
class CPause;
class CEnemyManager;
class CItemManager;
class CMap;
class CEdit;

//===========================================================
// ゲームクラス定義
//===========================================================
class CGame : public CScene
{
public:

	// ウェーブ管理
	enum WAVE
	{
		WAVE_00 = 0,
		WAVE_01,
		MAVE_CLEAR,
		WAVE_MAX
	};

	static WAVE m_Wave;

public:
	CGame();
	CGame(CScene::MODE mode);
	~CGame();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGame *Create(void);

	// 設定系
	static void SetbPause(bool bpause) { m_bPause = bpause; }
	void SetbFinish(bool bValue) { m_bFinish = bValue; }

	// 取得系
	static bool GetbPause(void) { return m_bPause; }
	static CPlayer *GetPlayer(void) { return m_pPlayer; }
	static CEnemy *GetEnemy(void) { return m_pEnemy; }
	static CEnemyManager *GetEnemyManager(void) { return m_pEnemyManager; }
	static CCollision *GetCollision(void) { return m_Collision; }
	static CItemManager *GetItemManager(void) { return m_pItemManager; }
	static CMap *GetMap(void) { return m_pMap; }
	static WAVE GetWave(void) { return m_Wave; }
	static CGame* GetInstance(void) { return m_pInstance; }

private:

	static void WaveControll(void);

	static CGame* m_pInstance;
	static CPause *m_pPause;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	static CEnemyManager *m_pEnemyManager;
	static CCollision *m_Collision;
	static CItemManager *m_pItemManager;
	static CMap *m_pMap;
	CEdit* m_pEdit;
	int m_nOnStageCounter;
	static bool m_bPause;
	bool m_bUse;  //エディットモード中かどうか
	bool m_bFinish;
	static bool m_bOnStage;
	static int m_nCounter;
};

#endif // !_GAME_H_