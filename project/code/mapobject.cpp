//===========================================================
//
// マップのモデル[mapobject.cpp]
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
#include "mapobject_electricbox.h"

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
	CObjectX::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject::Uninit(void)
{
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