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

//===========================================================
// 静的メンバ変数
//===========================================================
CMap *CMap::m_pMap = nullptr;

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const char* MAPTEXT = "data\\TEXT\\set_map_model.txt";  // マップのテキストファイル
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

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMap::Uninit(void)
{
	if (m_appObjectX != nullptr)
	{
		for (int nCount = 0; nCount < m_nNumModel; nCount++)
		{
			m_appObjectX[nCount]->Uninit();
			m_appObjectX[nCount] = nullptr;
		}

		delete m_appObjectX;
		m_appObjectX = nullptr;
	}
}

//===========================================================
// 更新処理
//===========================================================
void CMap::Update(void)
{

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

					m_appObjectX = new CObjectX*[MAX_MODEL];

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

					m_appObjectX[nCntItem] = CObjectX::Create(m_aTex[nType].aName);
					m_appObjectX[nCntItem]->SetPosition(pos);
					m_appObjectX[nCntItem]->SetRotition(rot);

					if (/*nType >= 43 && nType <= 51 || */nType == 56)
					{
						m_appObjectX[nCntItem]->SetbEnable(false);
					}
					else
					{
						m_appObjectX[nCntItem]->SetbEnable(true);
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