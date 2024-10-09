//===========================================================
//
// マップのモデル[map.cpp]
// Author 大原怜将
//
//===========================================================
#include "mapobject.h"
#include "objectX.h"
#include "debugproc.h"
#include "mapobject_can.h"
#include "mapobject_trashbox.h"
#include "mapobject_fance.h"
#include "mapobject_dramcan.h"
#include "mapobject_light.h"
#include "mapobject_sign.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CMapObject* CMapObject::m_pMap = nullptr;


//===========================================================
// 定数定義
//===========================================================
namespace
{
	const char* MAPTEXT = "data\\TEXT\\model_set.txt";  // マップのテキストファイル
}

//===========================================================
// コンストラクタ
//===========================================================
CMapObject::CMapObject()
{
	m_nNumModel = 0;
	m_nNumItem = 0;

	m_pMap = this;
}

//===========================================================
// コンストラクタ
//===========================================================
CMapObject::CMapObject(const char* aModelFliename, int nPriority) : CObjectX(aModelFliename, nPriority)
{
	m_nNumModel = 0;
	m_nNumItem = 0;

	m_pMap = this;
}

//===========================================================
// デストラクタ
//===========================================================
CMapObject::~CMapObject()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMapObject::Init(void)
{
	// マップのテキストファイル読込
	//TextLoad();

	CObjectX::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject::Uninit(void)
{
	// 自身をリストから消す
	//ListOut();

	CObjectX::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CMapObject::Update(void)
{
	CObjectX::Update();
}

//===========================================================
// 描画処理
//===========================================================
void CMapObject::Draw(void)
{
	CObjectX::Draw();
}

//===========================================================
// 生成処理
//===========================================================
CMapObject* CMapObject::Create(void)
{
	// マップのポインタ
	CMapObject* pMap = nullptr;

	if (pMap == nullptr)
	{
		// 生成処理
		pMap = new CMapObject();

		// 初期化処理
		pMap->Init();
	}

	return pMap;
}

////===========================================================
//// リストから自分を消す処理
////===========================================================
//void CMapObject::ListOut(void)
//{
//	if (m_pTop == this)
//	{// 自身が先頭
//
//		if (m_pNext != nullptr)
//		{// 次が存在している
//
//			m_pTop = m_pNext;	// 次を先頭にする
//			m_pNext->m_pPrev = nullptr;	// 次の前のポインタを覚えていないようにする
//		}
//		else
//		{// 存在していない
//
//			m_pTop = nullptr;	// 先頭がない状態にする
//			m_pCur = nullptr;	// 最後尾がない状態にする
//		}
//	}
//	else if (m_pCur == this)
//	{// 自身が最後尾
//
//		if (m_pPrev != nullptr)
//		{// 次が存在している
//
//			m_pCur = m_pPrev;			// 前を最後尾にする
//			m_pPrev->m_pNext = nullptr;	// 前の次のポインタを覚えていないようにする
//		}
//		else
//		{// 存在していない
//
//			m_pTop = nullptr;	// 先頭がない状態にする
//			m_pCur = nullptr;	// 最後尾がない状態にする
//		}
//	}
//	else
//	{
//		if (m_pNext != nullptr)
//		{
//			m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
//		}
//		if (m_pPrev != nullptr)
//		{
//			m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
//		}
//	}
//}

//===========================================================
// テキストファイル読込処理
//===========================================================
void CMapObject::TextLoad(void)
{
	char aString[128] = {};    // 文字読み取り用
	char aComment[128] = {};   // テキストファイル内のコメント読み取り用
	char aFileName[128] = {};  // ファイルの名前読み取り用
	int nCntItem = 0;
	int nCntFileName = 0;
	int nType = -1;
	D3DXVECTOR3 pos, rot;      // 読み取り用

	FILE* pFile;   //ファイルポインタを宣言

	pFile = fopen(MAPTEXT, "r");

	if (pFile != NULL)
	{//ファイルを開けた場合

		fscanf(pFile, "%s", &aString[0]);

		if (strcmp("SCRIPT", aString) == 0)
		{
			while (strcmp("END_SCRIPT", aString) != 0)
			{
				fscanf(pFile, "%s", &aString[0]);

				if (strcmp("NUM_ITEM", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);      //=
					fscanf(pFile, "%d", &m_nNumItem);  //モデルの総数

				}  // NUM_ITEMのかっこ

				if (strcmp("MODEL_FILENAME", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);       //=
					fscanf(pFile, "%s", &aFileName[0]);  //モデルの名前

					strcpy(m_aTex[nCntFileName].aName, aFileName);
					nCntFileName++;

					m_appObjectX = new CObjectX * [MAX_MODEL];

				}  // MODEL_LILENAMEのかっこ

				if (strcmp("MODELSET", aString) == 0)
				{
					while (strcmp("END_MODELSET", aString) != 0)
					{
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp("TYPE", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      //=
							fscanf(pFile, "%d", &nType);  //モデルの総数
						}

						if (strcmp("POS", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      //=
							fscanf(pFile, "%f", &pos.x);  //モデルの総数
							fscanf(pFile, "%f", &pos.y);  //モデルの総数
							fscanf(pFile, "%f", &pos.z);  //モデルの総数
						}

						if (strcmp("ROT", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      //=
							fscanf(pFile, "%f", &rot.x);  //モデルの総数
							fscanf(pFile, "%f", &rot.y);  //モデルの総数
							fscanf(pFile, "%f", &rot.z);  //モデルの総数
						}
					}

					if (nType == 3)
					{
						m_appObjectX[nCntItem] = CObjectX::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
							m_appObjectX[nCntItem]->SetVtx();
						}
					}

					if (nType == 0)
					{
						m_appObjectX[nCntItem] = CMapObject_Fance::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}

					if (nType == 1)
					{
						m_appObjectX[nCntItem] = CMapObject_TrashBox::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}

					if (nType == 2)
					{
						m_appObjectX[nCntItem] = CMapObject_Can::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}

					if (nType == 4)
					{
						m_appObjectX[nCntItem] = CMapObject_Dramcan::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}

					if (nType == 5)
					{
						m_appObjectX[nCntItem] = CMapObject_Sign::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}

					if (nType == 6)
					{
						m_appObjectX[nCntItem] = CMapObject_Light::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}
					
					nCntItem++;

					m_nNumModel++;
				}
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