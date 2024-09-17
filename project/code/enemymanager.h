//===========================================================
//
// 敵の管理 [enemymanager.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

//===========================================================
// 前方宣言
//===========================================================
class CEnemy;

//===========================================================
// 敵のマネージャークラス定義
//===========================================================
class CEnemyManager
{
public:
	CEnemyManager();              //コンストラクタ
	~CEnemyManager();             //デストラクタ
							       
	HRESULT Init(void);            //初期化処理
	void Uninit(void);             //終了処理
	void Update(void);             //更新処理

	static CEnemyManager *Create(void);   // 生成処理
	void ReadText(const char *text);      // テキストファイル読み込み
	void AllDelete(void);                 // 全消去

	// 設定系
	static void SetNum(int num) { m_nNum = num; }
	void SetTarget(int idx);
	void SetTrue(int idx);
	void SetMobility(void);
	void SetBossEnemy(void);
	void RestartDrawing(void);
	
	// 取得系
	CEnemy **GetEnemy(void) { return m_appEnemy; }
	static int GetNum(void) { return m_nNum; }
	static CEnemyManager* GetInstance(void) { return m_pEnemyManager; }

	static CEnemyManager* m_pEnemyManager;
							       
private:
	
	int m_nEnemyAll;               // 敵の総数
	static int m_nNum;
	int m_nCntWave;
	CEnemy **m_appEnemy;   //エネミーへのポインタ

	D3DXVECTOR3 m_Readpos;           //テキストファイルの位置読み取り用
	D3DXVECTOR3 m_Readrot;           //テキストファイルの向き読み取り用

	static CEnemy *m_pTop;  //先頭のオブジェクトへのポインタ
	static CEnemy *m_pCur;  //最後尾のオブジェクトへのポインタ
	static CEnemy *m_pSNext;
	CEnemy *m_pPrev;       //前のオブジェクトへのポインタ
	CEnemy *m_pNext;       //次のオブジェクトへのポインタ
	bool m_bDeath;         //死亡フラグ
};

#endif // !_ENEMYMANAGER_H_