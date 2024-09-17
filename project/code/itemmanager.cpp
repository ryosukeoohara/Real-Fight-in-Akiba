//===========================================================
//
// アイテムの管理 [itemmanager.cpp]
// Author : 大原　怜将
//
//===========================================================

//===========================================================
// インクルードファイル
//===========================================================
#include "itemmanager.h"
#include "item.h"

//===========================================================
// 静的メンバ変数
//===========================================================
const char *CItemManager::m_apTexName[CItem::TYPE_MAX] =
{
	"data\\MODEL\\refrigerator.x",  // 冷蔵庫
	"data\\MODEL\\bike.x",          // 自転車
	"data\\MODEL\\microwave.x",     // 電子レンジ
	"data\\MODEL\\table.x",         // デーブル
	"data\\MODEL\\poster.x",        // 看板
};
CItemManager* CItemManager::m_pItemManager = nullptr;

//===========================================================
// コンストラクタ
//===========================================================
CItemManager::CItemManager()
{
	// 値をクリア
	m_nNum = 0;
	m_pItemManager = this;
}

//===========================================================
// デストラクタ
//===========================================================
CItemManager::~CItemManager()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CItemManager::Init(void)
{
	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CItemManager::Uninit(void)
{
	
}

//===========================================================
// 更新処理
//===========================================================
void CItemManager::Update(void)
{
	
}

//===========================================================
// 生成処理
//===========================================================
CItemManager * CItemManager::Create(const char* filename)
{
	CItemManager *pItemManager = nullptr;

	if (pItemManager == nullptr)
	{
		pItemManager = new CItemManager();

		pItemManager->Init();

		pItemManager->ReadText(filename);
	}

	return pItemManager;
}

//===========================================================
// 消す
//===========================================================
void CItemManager::Release(int idx)
{
	CItem *pItem = CItem::GetTop();

	while (pItem != nullptr)
	{
		CItem *pItemNext = pItem->GetNext();

		if (pItem->GetID() == idx)
		{
			pItem->Uninit();
		}

		pItem = pItemNext;
	}
}

//===========================================================
// テキストファイル読み込み
//===========================================================
void CItemManager::ReadText(const char *text)
{
	char aString[128] = {};              // 文字読み取り用
	char aFileName[128] = {};            // ファイルの名前読み取り用
	int nIdx = 0;                        // 番号読み取り用
	int nType = -1;                      // 種類読み取り用
	D3DXVECTOR3 pos = {}, rot = {};      // 読み取り用

	FILE *pFile;   // ファイルポインタを宣言

	pFile = fopen(text, "r");

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
					fscanf(pFile, "%s", &aString);  // =読込
					fscanf(pFile, "%d", &m_nNum);   // モデルの総数
					
				}  // NUM_ITEMのかっこ

				if (strcmp("MODEL_FILENAME", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);       // =読込
					fscanf(pFile, "%s", &aFileName[0]);  // モデルの名前

				}  // MODEL_LILENAMEのかっこ

				if (strcmp("ITEMSET", aString) == 0)
				{
					while (strcmp("END_ITEMSET", aString) != 0)
					{
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp("TYPE", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      // =読込
							fscanf(pFile, "%d", &nType);        // 種類の総数
						}

						if (strcmp("POS", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      // =読込
							fscanf(pFile, "%f", &pos.x);        // 位置X
							fscanf(pFile, "%f", &pos.y);        // 位置Y
							fscanf(pFile, "%f", &pos.z);        // 位置Z
						}

						if (strcmp("ROT", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      // =読込
							fscanf(pFile, "%f", &rot.x);        // 向きX
							fscanf(pFile, "%f", &rot.y);        // 向きY
							fscanf(pFile, "%f", &rot.z);        // 向きZ
						}
					}

					// アイテム生成
					CItem::Create(pos, rot, (CItem::TYPE)nType, nIdx, m_apTexName[nType]);

					// 番号加算
					nIdx++;
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