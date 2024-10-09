//===========================================================
//
// マップのモデル[map.cpp]
// Author 大原怜将
//
//===========================================================
#include "renderer.h"
#include "manager.h"
#include "map.h"
#include "objectX.h"
#include "debugproc.h"
#include "object.h"
#include "mapobject.h"
#include "mapobject_can.h"
#include "mapobject_dramcan.h"
#include "mapobject_fance.h"
#include "mapobject_trashbox.h"
#include "mapobject_electricbox.h"
#include "mapobject_light.h"
#include "mapobject_sign.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CMap *CMap::m_pMap = nullptr;
CMapObject* CMap::m_pTop = nullptr;
CMapObject* CMap::m_pCur = nullptr;
CObjectX* CMap::m_pObjXTop = nullptr;
CObjectX* CMap::m_pObjXCur = nullptr;

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const char* MAPTEXT = "data\\TEXT\\set_map_model.txt";  // マップのテキストファイル
	const char* MAP_OBJECCT_TEXT = "data\\TEXT\\model_set.txt";  // マップのテキストファイル
}

//===========================================================
// コンストラクタ
//===========================================================
CMap::CMap()
{
	m_nNumModel = 0;
	m_nNumItem = 0;

	m_pMap = this;
}

//===========================================================
// デストラクタ
//===========================================================
CMap::~CMap()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMap::Init(void)
{
	// マップのテキストファイル読込
	TextLoad();

	// マップのテキストファイル読込
	MapObjectLoad();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMap::Uninit(void)
{
	/*if (m_appObjectX != nullptr)
	{
		for (int nCount = 0; nCount < m_nNumModel; nCount++)
		{
			m_appObjectX[nCount]->Uninit();
			m_appObjectX[nCount] = nullptr;
		}

		delete m_appObjectX;
		m_appObjectX = nullptr;
	}*/
}

//===========================================================
// 更新処理
//===========================================================
void CMap::Update(void)
{
	// マップオブジェクトの先頭アドレスを取得
	CMapObject* pMapObject = GetTop();

	while (pMapObject != nullptr)
	{
		CMapObject* pNext = pMapObject->GetNext();

		//pMapObject->Update();

		pMapObject = pNext;
	}

}

//===========================================================
// 描画処理
//===========================================================
void CMap::Draw(void)
{
	// マップオブジェクトの先頭アドレスを取得
	CMapObject* pMapObject = GetTop();

	while (pMapObject != nullptr)
	{
		CMapObject* pNext = pMapObject->GetNext();

		//pMapObject->Draw();

		pMapObject = pNext;
	}
}

//===========================================================
// 生成処理
//===========================================================
CMap *CMap::Create(void)
{
	// マップのポインタ
	CMap *pMap = nullptr;

	if (pMap == nullptr)
	{
		// 生成処理
		pMap = new CMap();

		// 初期化処理
		pMap->Init();
	}

	return pMap;
}

//===========================================================
// テキストファイル読込処理
//===========================================================
void CMap::TextLoad(void)
{
	char aString[128] = {};    // 文字読み取り用
	char aComment[128] = {};   // テキストファイル内のコメント読み取り用
	char aFileName[128] = {};  // ファイルの名前読み取り用
	int nCntItem = 0;
	int nCntFileName = 0;
	int nType = -1;
	D3DXVECTOR3 pos, rot;      // 読み取り用

	FILE *pFile;   //ファイルポインタを宣言

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

					CObjectX* pObjectX = CObjectX::Create(m_aTex[nType].aName);
					pObjectX->SetPosition(pos);
					pObjectX->SetRotition(rot);
					pObjectX->SetIdxModel(nType);

					if (/*nType >= 43 && nType <= 51 || */nType == 56)
					{
						pObjectX->SetbEnable(false);
					}
					else
					{
						pObjectX->SetbEnable(true);
					}

					if (m_pObjXTop != nullptr)
					{// 先頭が存在している場合

						m_pObjXCur->SetNext(pObjectX);
						m_pObjXTop->SetPrev(m_pObjXCur);
						m_pObjXCur = pObjectX;
					}
					else
					{// 存在しない場合

						m_pObjXTop = pObjectX;
						m_pObjXCur = pObjectX;
					}

					//if (m_pObjXTop != nullptr)
					//{// 先頭が存在している場合

					//	m_pCur[nPriority]->m_pNext = this;
					//	m_pPrev = m_pCur[nPriority];
					//	m_pCur[nPriority] = this;
					//}
					//else
					//{// 存在しない場合

					//	m_pObjXTop = pObjectX;
					//	m_pObjXCur = pObjectX;
					//}

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

//===========================================================
// テキストファイル読込処理
//===========================================================
void CMap::MapObjectLoad(void)
{
	char aString[128] = {};    // 文字読み取り用
	char aComment[128] = {};   // テキストファイル内のコメント読み取り用
	char aFileName[128] = {};  // ファイルの名前読み取り用
	int nCntItem = 0;
	int nCntFileName = 0;
	int nType = -1;
	D3DXVECTOR3 pos, rot;      // 読み取り用

	FILE* pFile;   //ファイルポインタを宣言

	pFile = fopen(MAP_OBJECCT_TEXT, "r");

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
					fscanf(pFile, "%s", &aString);      //=
					fscanf(pFile, "%d", &m_nNumItem);  //モデルの総数

				}  // NUM_ITEMのかっこ

				if (strcmp("MODEL_FILENAME", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);       //=
					fscanf(pFile, "%s", &aFileName[0]);  //モデルの名前

					strcpy(m_aTex[nCntFileName].aName, aFileName);
					nCntFileName++;

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

					CMapObject* pMapObject = nullptr;

					if (nType == 0)
					{
						pMapObject = CMapObject_Fance::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
							pMapObject->SetVtx();
						}
					}

					if (nType == 1)
					{
						pMapObject = CMapObject_TrashBox::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
							pMapObject->SetVtx();
						}
					}

					if (nType == 2)
					{
						pMapObject = CMapObject_Can::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
						}
					}

					if (nType == 3)
					{
						pMapObject = CMapObject_ElectricBox::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
							pMapObject->SetVtx();
						}
					}

					if (nType == 4)
					{
						pMapObject = CMapObject_Dramcan::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
						}
					}

					if (nType == 6)
					{
						pMapObject = CMapObject_Sign::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
						}
					}

					if (nType == 5)
					{
						pMapObject = CMapObject_Light::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
							pMapObject->SetVtx();
						}
					}

					if (m_pTop != nullptr)
					{// 先頭が存在している場合

						m_pCur->SetNext(pMapObject);
						m_pTop->SetPrev(m_pCur);
						m_pCur = pMapObject;
					}
					else
					{// 存在しない場合

						m_pTop = pMapObject;
						m_pCur = pMapObject;
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
