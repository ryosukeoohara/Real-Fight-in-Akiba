//===========================================================
//
// 敵の管理 [enemymanager.cpp]
// Author : 大原　怜将
//
//===========================================================

//===========================================================
// インクルードファイル
//===========================================================
#include "enemymanager.h"
#include "enemy.h"
#include "enemy_weak.h"
#include "enemy_boss.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CEnemy *CEnemyManager::m_pTop = nullptr;
CEnemy *CEnemyManager::m_pCur = nullptr;
CEnemy *CEnemyManager::m_pSNext = nullptr;
int CEnemyManager::m_nNum = 0;
CEnemyManager *CEnemyManager::m_pEnemyManager;

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const char* TUTORIALENEMYTEXT = "data\\TEXT\\enemy\\tutorialenemyinformation.txt";  // チュートリアルの敵の配置情報
	const char* ENEMYTEXT = "data\\TEXT\\enemy\\enemyinformation.txt";  // 雑魚敵の配置情報
	const char* BOSSTEXT = "data\\TEXT\\enemy\\enemyboss.txt";          // ボス敵の配置情報
}

//===========================================================
// コンストラクタ
//===========================================================
CEnemyManager::CEnemyManager()
{
	//初期化
	m_appEnemy = nullptr;
	m_nEnemyAll = 0;
	m_nNum = 0;
	m_Readpos = { 0.0f,0.0f,0.0f };
	m_Readrot = { 0.0f,0.0f,0.0f };

	m_pEnemyManager = this;
}

//===========================================================
// デストラクタ
//===========================================================
CEnemyManager::~CEnemyManager()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CEnemyManager::Init(void)
{
	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		ReadText(ENEMYTEXT);
	}
	else if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TUTORIAL)
	{
		ReadText(TUTORIALENEMYTEXT);
	}

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CEnemyManager::Uninit(void)
{
	
}

//===========================================================
// 更新処理
//===========================================================
void CEnemyManager::Update(void)
{
	//デバッグプロックの情報を取得
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("敵の数：%d\n", m_nNum);
}

//===========================================================
// 生成処理
//===========================================================
CEnemyManager * CEnemyManager::Create(void)
{
	CEnemyManager *pEnemyManager = nullptr;

	if (pEnemyManager == nullptr)
	{
		pEnemyManager = new CEnemyManager();

		pEnemyManager->Init();
	}

	return pEnemyManager;
}

//===========================================================
// テキストファイル読み込み
//===========================================================
void CEnemyManager::ReadText(const char *text)
{
	int nLife = 0;
	int nType = -1;
	char aString[128] = {};

	FILE *pFile;   //ファイルポインタを宣言

	pFile = fopen(text, "r");

	if (pFile != NULL)
	{//ファイルを開けた場合

		fscanf(pFile, "%s", &aString[0]);

		if (strcmp("SCRIPT", aString) == 0)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp("ALL_ENEMY", aString) == 0)
			{
				fscanf(pFile, "%s", &aString);          //=
				fscanf(pFile, "%d", &m_nEnemyAll);  //モデルの総数

				m_appEnemy = new CEnemy*[m_nEnemyAll];
				//m_nNum = m_nEnemyAll;

			}  //NUM_MODELのかっこ

			while (strcmp("END_SCRIPT", aString) != 0)
			{
				fscanf(pFile, "%s", &aString[0]);

				if (strcmp("ENEMYSET", aString) == 0)
				{
					while (strcmp("ENEMYSET_END", aString) != 0)
					{
						fscanf(pFile, "%s", &aString);

						if (strcmp("POS", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      //=
							fscanf(pFile, "%f", &m_Readpos.x);  //敵の位置
							fscanf(pFile, "%f", &m_Readpos.y);  //敵の位置
							fscanf(pFile, "%f", &m_Readpos.z);  //敵の位置
						}

						if (strcmp("ROT", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      //=
							fscanf(pFile, "%f", &m_Readrot.x);  //敵の向き
							fscanf(pFile, "%f", &m_Readrot.y);  //敵の向き
							fscanf(pFile, "%f", &m_Readrot.z);  //敵の向き
						}

						if (strcmp("LIFE", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);       //=
							fscanf(pFile, "%d", &nLife);       //敵の体力
						}

						if (strcmp("TYPE", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);       //=
							fscanf(pFile, "%d", &nType);       //敵の体力
						}

					}//ENEMYSET_ENDのかっこ 

					if (nType == CEnemy::WEAK)
					{// 雑魚敵

						CEnemyWeak::Create(m_Readpos, m_Readrot, nLife);

					}
					else if (nType == CEnemy::BOSS)
					{// ボス敵

						CEnemyBoss::Create(m_Readpos, m_Readrot, nLife);
					}

					m_nNum++;
					
				}//ENEMYSETのかっこ 
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		return;
	}
}

//===========================================================
// 全消去処理
//===========================================================
void CEnemyManager::AllDelete(void)
{
	CEnemy* pEnemy = CEnemy::GetTop();

	while (pEnemy != nullptr)
	{
		CEnemy* pEnemyNext = pEnemy->GetNext();

		pEnemy->SetLife(0);

		pEnemy = pEnemyNext;
	}
}

//===========================================================
// ヒートアクションのターゲット以外の描画を止める処理
//===========================================================
void CEnemyManager::SetTarget(int idx)
{
	CEnemy *pEnemy = CEnemy::GetTop();

	while (pEnemy != nullptr)
	{
		CEnemy *pEnemyNext = pEnemy->GetNext();

		// 敵の番号取得
 		int Idx = pEnemy->GetIdxID();

		if (Idx != idx)
		{
			pEnemy->SetImmobile();

			// 描画停止
			pEnemy->SetDraw(false);
		}

		pEnemy = pEnemyNext;
	}
}

//===========================================================
// 描画を停止している敵の描画を再開する処理
//===========================================================
void CEnemyManager::SetTrue(int idx)
{
	CEnemy *pEnemy = CEnemy::GetTop();

	while (pEnemy != nullptr)
	{
		CEnemy *pEnemyNext = pEnemy->GetNext();

		// 敵の番号取得
		int Idx = pEnemy->GetIdxID();

		if (Idx != idx)
		{
			// 描画再開
			pEnemy->SetDraw(true);
		}

		pEnemy = pEnemyNext;
	}
}

//===========================================================
// 敵が行動できるようにする処理
//===========================================================
void CEnemyManager::SetMobility(void)
{
	CEnemy *pEnemy = CEnemy::GetTop();

	while (pEnemy != nullptr)
	{
		CEnemy *pEnemNext = pEnemy->GetNext();

		// 行動可能にする
		pEnemy->SetMobile();

		pEnemy = pEnemNext;
	}
}

//===========================================================
// ボス敵を生成
//===========================================================
void CEnemyManager::SetBossEnemy(void)
{
	ReadText(BOSSTEXT);
}

//===========================================================
// 描画を再開する処理
//===========================================================
void CEnemyManager::RestartDrawing(void)
{
	CEnemy* pEnemy = CEnemy::GetTop();

	while (pEnemy != nullptr)
	{
		CEnemy* pEnemyNext = pEnemy->GetNext();

		// 描画再開
		pEnemy->SetDraw(true);

		pEnemy = pEnemyNext;
	}
}