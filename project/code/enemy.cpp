//===========================================================
//
// 敵の処理 [enemy.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "enemy.h"
#include "enemy_weak.h"
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
#include "MyEffekseer.h"
#include "utility.h"
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
	const int DAMAGECOUNT = 15;     // ダメージ状態
	const float RADIUS = 20.0f;     // 横幅
	const float ATTACKLENGHT = 50.0f;  // 攻撃可能範囲
	const float ENEMY_MOVE = 2.0f;  // 移動量
	const float SPEED = 2.0f;          // 走る速さ
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
	m_Mobility = Immobile;
	m_Info.nLife = 0;
	m_Info.nIdxID = -1;
	m_Info.bDraw = true;
	m_nDamageCounter = 0;
	m_pCurrent = nullptr;
	m_pNext = nullptr;
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
	m_Mobility = Immobile;
	m_Info.nLife = nlife;
	m_Info.nIdxID = -1;
	m_Info.bDraw = true;
	m_nDamageCounter = 0;
	m_pCurrent = nullptr;
	m_pNext = nullptr;
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

	m_nDamageCounter = DAMAGECOUNT;

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

	// 自分をリストから消す
	ListOut();

	// 親のポインタ
	if (m_pCurrent != nullptr)
		m_pCurrent = nullptr;

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
	// モーションの更新
	if (m_pMotion != nullptr)
		m_pMotion->Update();

	// モデルの更新
	if (m_apModel == nullptr)
		return;
	
	for (int nCount = 0; nCount < m_nNumModel; nCount++)
	{
		if (m_apModel[nCount] != nullptr)
			m_apModel[nCount]->Update();

	}

	CManager::GetInstance()->GetDebugProc()->Print("\n敵の位置：%f,%f,%f\n", m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("敵の向き：%f,%f,%f\n", m_Info.rot.x, m_Info.rot.y, m_Info.rot.z);
}

//===========================================================
// 描画処理
//===========================================================
void CEnemy::Draw(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 描画を切っていたら処理を抜ける
	if (!m_Info.bDraw)
		return;
	
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

	// 描画処理
	for (int nCount = 0; nCount < m_nNumModel; nCount++)
	{
		if (m_apModel[nCount] == nullptr)
			continue;

		m_apModel[nCount]->Draw();
	}
}

//===========================================================
// リストから自分を消す処理
//===========================================================
void CEnemy::ListOut(void)
{
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

		m_pMotion->InitPose(MOTION_NEUTRAL);
	}
}

void CEnemy::SetChase(CHASE cha)
{
}

//===========================================================
// 攻撃が発生しているかどうか判定
//===========================================================
void CEnemy::HitDetection(D3DXVECTOR3 MyPos, float attackrange, float targetradius)
{
	if (m_pMotion == nullptr)
		return;

	if (m_pMotion->GetNowFrame() == m_pMotion->GetAttackOccurs())
	{
		D3DXMATRIX mtx = *GetCharcter()[0]->GetMtxWorld();
		MyEffekseer::EffectCreate(CMyEffekseer::TYPE_ATTACK, false, D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	}

	if (m_pMotion->GetAttackOccurs() <= m_pMotion->GetNowFrame() && m_pMotion->GetAttackEnd() >= m_pMotion->GetNowFrame())
	{// 現在のフレームが攻撃判定発生フレーム以上かつ攻撃判定終了フレームない

		if (CGame::GetCollision()->Circle(MyPos, CGame::GetPlayer()->GetPosition(), attackrange, targetradius) == true)
			CPlayer::GetInstance()->Damage(GetMotion()->GetAttackDamage(), D3DXVECTOR3(sinf(GetRotition().y) * GetMotion()->GetKnockBack(), 0.0f, cosf(GetRotition().y) * GetMotion()->GetKnockBack()));
	}
}