//===========================================================
//
// チュートリアル処理[tutorial.h]
// Author 大原怜将
//
//===========================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "manager.h"

//===========================================================
// 前方宣言
//===========================================================
class CField;
class CObject2D;
class CMap;
class CPlayer;
class CEnemy;
class CItemManager;
class CEnemyManager;
class CCollision;
class CTutorialUI;

//===========================================================
// チュートリアルクラス定義
//===========================================================
class CTutorial : public CScene
{
public:

	CTutorial();          // コンストラクタ
	~CTutorial();         // デストラクタ

	HRESULT Init(void);   // 初期化処理
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理
	void Draw(void);      // 描画処理

	static CTutorial *Create(void);  // 生成処理 

	// 取得
	static CPlayer *GetPlayer(void) { return m_pPlayer; }
	static CMap *GetMap(void) { return m_pMap; }
	static CEnemyManager* GetEnemyManager(void) { return m_pEnemyManager; }
	static CItemManager* GetItemManager(void) { return m_pItemManager; }

private:
	CField *m_pField;
	CObject2D *m_pBg;
	CObject2D *m_pStart;
	static CMap *m_pMap;
	CEnemy *m_pEnemy;
	static CItemManager *m_pItemManager;
	CCollision *m_Collision;
	static CTutorialUI *m_pUI;
	static CEnemyManager *m_pEnemyManager;
	static CPlayer *m_pPlayer;

};

#endif 