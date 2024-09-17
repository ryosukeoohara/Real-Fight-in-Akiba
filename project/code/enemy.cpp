//===========================================================
//
// 敵の処理 [enemy.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "enemy.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "motion.h"
#include "sound.h"
#include "player.h"
#include "game.h"
#include "debugproc.h"
#include "score.h"
#include "player.h"
#include "character.h"
#include "collision.h"
#include "enemymanager.h"
#include "gage.h"
#include "camera.h"
#include "particle.h"
#include "effect.h"
#include "item.h"
#include <assert.h>

//===========================================================
// 静的メンバ変数
//===========================================================
CEnemy *CEnemy::m_pTop = nullptr;
CEnemy *CEnemy::m_pCur = nullptr;
int CEnemy::m_nIdx = 0;
int CEnemy::m_nNumAll = 0;

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const int DAMEGECOUNT = 15;     // ダメージ状態
	const float RADIUS = 20.0f;     // 横幅
	const float ENEMY_MOVE = 2.0f;  // 移動量
	const char *ENEMY_TEXT = "data\\TEXT\\motion_set_enemy.txt";  // テキストファイル
}

//===========================================================
// コンストラクタ
//===========================================================
CEnemy::CEnemy()
{
	// 値をクリア
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_Info.mtxWorld);
	m_Info.state = STATE_NONE;
	m_Mobility = Immobile;
	m_Info.nLife = 0;
	m_Info.nIdxID = -1;
	m_Info.bDraw = true;
	m_nDamegeCounter = 0;
	m_pCurrent = nullptr;
	m_pNext = nullptr;
	m_pLife2D = nullptr;
	m_pLife3D = nullptr;
	m_bDeath = false;

	if (m_pTop != nullptr)
	{// 先頭が存在している場合

		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合

		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}
}

//===========================================================
// コンストラクタ
//===========================================================
CEnemy::CEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
{
	// 値をクリア
	m_Info.pos = pos;
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = rot;
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_Info.mtxWorld);
	m_Info.state = STATE_NONE;
	m_Mobility = Immobile;
	m_Info.nLife = nlife;
	m_Info.nIdxID = -1;
	m_Info.bDraw = true;
	m_nDamegeCounter = 0;
	m_pCurrent = nullptr;
	m_pNext = nullptr;
	m_pLife2D = nullptr;
	m_pLife3D = nullptr;
	m_bDeath = false;

	if (m_pTop != nullptr)
	{// 先頭が存在している場合

		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合

		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}
}

//===========================================================
// デストラクタ
//===========================================================
CEnemy::~CEnemy()
{

}

////===========================================================
//// 生成処理
////===========================================================
//CEnemy * CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
//{
//	CEnemy *pEnemy = nullptr;
//
//	if (pEnemy == nullptr)
//	{
//		pEnemy = new CEnemy(pos, rot, nlife);
//
//		pEnemy->Init();
//	}
//
//	return pEnemy;
//}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CEnemy::Init(void)
{
	if (m_pMotion == nullptr)
	{// 使用されていなかったら

		// 生成
		m_pMotion = new CMotion;

		// 初期化処理
		m_pMotion->Init();
	}

	m_nDamegeCounter = DAMEGECOUNT;

	m_Info.nIdxID = m_nNumAll;
	m_nNumAll++;

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CEnemy::Uninit(void)
{
	m_nNumAll = 0;

	m_bDeath = true;

	// リストから自分自身を削除する
	if (m_pTop == this)
	{// 自身が先頭
		if (m_pNext != nullptr)
		{// 次が存在している
			m_pTop = m_pNext;	// 次を先頭にする
			m_pNext->m_pPrev = nullptr;	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == this)
	{// 自身が最後尾
		if (m_pPrev != nullptr)
		{// 次が存在している
			m_pCur = m_pPrev;			// 前を最後尾にする
			m_pPrev->m_pNext = nullptr;	// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else
	{
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
		}
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
		}
	}

	// モーションの破棄
	if (m_pMotion != nullptr)
	{
		//終了処理
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// パーツの破棄
	if (m_apModel != nullptr)
	{
		for (int nCount = 0; nCount < m_nNumModel; nCount++)
		{
			if (m_apModel[nCount] != nullptr)
			{
				m_apModel[nCount]->Uninit();
				m_apModel[nCount] = nullptr;
			}
		}

		delete m_apModel;
		m_apModel = nullptr;
	}

	CObject::Release();
}

//===========================================================
// 更新処理
//===========================================================
void CEnemy::Update(void)
{
	if (m_Info.bDraw == true)
	{
		if (m_Info.state != STATE_HEATDAMEGE && m_Info.state != STATE_HEATACTELECTROWAIT && m_Info.state != STATE_HEATACTELECTRO && m_Info.state != STATE_HEATACTFAINTING && m_Info.state != STATE_DEATH)
		{
			Controll();
		}
		
		if (m_pMotion != nullptr)
		{
			// 更新処理
			m_pMotion->Update();
		}

		if (m_apModel != nullptr)
		{
			for (int nCount = 0; nCount < m_nNumModel; nCount++)
			{
				if (m_apModel[nCount] != nullptr)
				{
					m_apModel[nCount]->Update();
				}
			}
		}

		// 敵同士の当たり判定
		CEnemy* pEnemy = CEnemy::GetTop();
		while (pEnemy != nullptr)
		{
			CEnemy* pEnemyNext = pEnemy->GetNext();

			if(pEnemy != this)
			m_Info.pos = *CGame::GetCollision()->CheckEnemy(&m_Info.pos, &m_Info.posOld, &pEnemy->GetPosition(), RADIUS);

			pEnemy = pEnemyNext;
		}
	}

	if (GetMotion()->IsFinish() == false)
		return;

	if ((m_Info.state == STATE_HEATDAMEGE || m_Info.state == STATE_PAINFULDAMAGE || m_Info.state == STATE_HEATACTFAINTING) && m_Info.state != STATE_GETUP)
	{
		m_Info.state = STATE_GETUP;
		GetMotion()->Set(TYPE_GETUP);
		
		if (m_Chase == CHASE_OFF)
			m_Chase = CHASE_ON;
	}
	else if (m_Info.state == STATE_DEATH)
	{// 死亡

		// 敵の総数を減らす
		int nNum = CEnemyManager::GetNum() - 1;
		CEnemyManager::SetNum(nNum);
		Uninit();

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{// ゲームの時

			// 敵を倒した数を増やす
			int nDefeat = CPlayer::GetInstance()->GetDefeat() + 1;
			CPlayer::GetInstance()->SetDefeat(nDefeat);
		}

		return;
	}
	else
	{
		m_Info.state = STATE_NEUTRAL;
		GetMotion()->Set(TYPE_NEUTRAL);

		if (m_Chase == CHASE_OFF)
			m_Chase = CHASE_ON;
	}
}

//===========================================================
// 描画処理
//===========================================================
void CEnemy::Draw(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_Info.bDraw == true)
	{
		//計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_Info.mtxWorld);

		if (m_pCurrent != nullptr)
		{
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);

			// マトリックスと親のマトリックスをかけ合わせる
			D3DXMatrixMultiply(&m_Info.mtxWorld,
				&m_Info.mtxWorld, m_pCurrent);
		}
		else
		{
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);
		}

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_Info.mtxWorld);

		for (int nCount = 0; nCount < m_nNumModel; nCount++)
		{
			if (m_apModel[nCount] != nullptr)
			{
				//描画処理
				m_apModel[nCount]->Draw();
			}
		}
	}
}

//===========================================================
// 制御処理
//===========================================================
void CEnemy::Controll(void)
{
	int nNum = 0;
	CEnemy **ppEnemy = nullptr;

	if (m_Info.state == STATE_DAMEGE || m_Info.state == STATE_HEATDAMEGE)
	{
		m_nDamegeCounter--;

		if (m_nDamegeCounter <= 0)
		{
			m_Info.state = STATE_NONE;
			m_nDamegeCounter = DAMEGECOUNT;
		}
	}
	else
	{
		if (m_Info.state != STATE_GRAP && m_Mobility == Mobile)
			Move();
	}

	if (m_Info.nLife <= 0)
	{
		m_Info.state = STATE_DEATH;
		GetMotion()->Set(TYPE_DETH);
		return;
	}

	if (m_Info.state != STATE_GRAP && CPlayer::GetInstance()->GetHeatActFlag() == false)
	{
		m_Info.move.y -= 0.9f;

		// 移動量
		m_Info.pos.x += m_Info.move.x;
		m_Info.pos.y += m_Info.move.y;
		m_Info.pos.z += m_Info.move.z;

		if (m_Info.pos.y <= 0.0f)
			m_Info.pos.y = 0.0f;

	}

	//デバッグプロックの情報を取得
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("\n敵の位置：%f,%f,%f\n", m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	pDebugProc->Print("敵の向き：%f,%f,%f\n", m_Info.rot.x, m_Info.rot.y, m_Info.rot.z);
	pDebugProc->Print("敵の体力：%d\n", m_Info.nLife);
	pDebugProc->Print("敵の番号：%d\n", m_Info.nIdxID);
}

//===========================================================
// 制御処理
//===========================================================
void CEnemy::Attack(void)
{
	
}

//===========================================================
// 制御処理
//===========================================================
void CEnemy::Move(void)
{
	
}

//===========================================================
// 制御処理
//===========================================================
void CEnemy::Damege(int damege, float blowaway, CPlayer::ATTACKTYPE act)
{
	
}

//===========================================================
// テキストファイル読み込み
//===========================================================
void CEnemy::ReadText(const char *fliename)
{
	char aString[128] = {};
	char aComment[128] = {};
	int nCntParts = 0, nCntParts2 = 0, nCntParts3 = 0;
	int nCntMotion = 0;
	int nCntKeySet = 0;
	int nCntKey = 0;
	int nCntModel = 0;
	int nCntMotionIdx = 0;

	//テクスチャの情報取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	FILE *pFile;   //ファイルポインタを宣言

	pFile = fopen(fliename, "r");

	if (pFile != NULL)
	{//ファイルを開けた場合

		fscanf(pFile, "%s", &aString[0]);

		if (strcmp("SCRIPT", aString) == 0)
		{
			while (strcmp("END_SCRIPT", aString) != 0)
			{
				fscanf(pFile, "%s", &aString[0]);

				if (strcmp("NUM_MODEL", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);          //=
					fscanf(pFile, "%d", &m_nNumModel);  //モデルの総数

					m_apModel = new CCharacter*[m_nNumModel];

				}  //NUM_MODELのかっこ

				if (strcmp("MODEL_FILENAME", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);          //=
					fscanf(pFile, "%s", &m_filename[0]);  //モデルの名前

					m_apModel[nCntModel] = CCharacter::Create(m_filename);
					nCntModel++;

					nCntParts++;

				}  //MODEL_LILENAMEのかっこ

				if (strcmp("CHARACTERSET", aString) == 0)
				{
					while (strcmp("END_CHARACTERSET", aString) != 0)
					{
						fscanf(pFile, "%s", &aString);

						if (strcmp("PARTSSET", aString) == 0)
						{
							while (strcmp("END_PARTSSET", aString) != 0)
							{
								fscanf(pFile, "%s", &aString);

								if (strcmp("INDEX", aString) == 0)
								{
									fscanf(pFile, "%s", &aString);          //=
									fscanf(pFile, "%d", &m_nIdx);  //モデルの番号
								}

								if (strcmp("PARENT", aString) == 0)
								{
									fscanf(pFile, "%s", &aString);          //=
									fscanf(pFile, "%d", &m_nParent);  //親モデルの番号

									if (m_nParent > -1 && m_nNumModel > m_nParent)
									{
										m_apModel[nCntParts2]->SetParent(m_apModel[m_nParent]);
									}
									else
									{
										m_apModel[nCntParts2]->SetParent(nullptr);
									}
								}

								if (strcmp("POS", aString) == 0)
								{
									fscanf(pFile, "%s", &aString);      //=
									fscanf(pFile, "%f", &m_Readpos.x);  //モデルの総数
									fscanf(pFile, "%f", &m_Readpos.y);  //モデルの総数
									fscanf(pFile, "%f", &m_Readpos.z);  //モデルの総数

									m_apModel[nCntParts2]->SetPositionOri(m_Readpos);

									m_apModel[nCntParts2]->SetPosition(m_Readpos);
								}

								if (strcmp("ROT", aString) == 0)
								{
									fscanf(pFile, "%s", &aString);      //=
									fscanf(pFile, "%f", &m_Readrot.x);  //モデルの総数
									fscanf(pFile, "%f", &m_Readrot.y);  //モデルの総数
									fscanf(pFile, "%f", &m_Readrot.z);  //モデルの総数

									m_apModel[nCntParts2]->SetRotOrigin(m_Readrot);

									m_apModel[nCntParts2]->SetRot(m_Readrot);
								}

							}//END_PARTSSETのかっこ

							nCntParts2++;

						}//PARTSSETのかっこ

					}//END_CHARACTERSETのかっこ 

				}//CHARACTERSETのかっこ 
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		return;
	}

	if (m_pMotion != nullptr)
	{
		//モデルの設定
		m_pMotion->SetModel(m_apModel, m_nNumModel);

		//初期化処理
		m_pMotion->ReadText(fliename);

		m_pMotion->InitPose(TYPE_NEUTRAL);

		//m_pMotion->Set(TYPE_NEUTRAL);
	}
}

void CEnemy::SetChase(CHASE cha)
{
}