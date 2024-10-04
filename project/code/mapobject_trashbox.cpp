//===========================================================
//
// マップに配置れているゴミ箱処理[mapobject_trashbox.cpp]
// Author 大原怜将
//
//===========================================================
#include "mapobject_trashbox.h"

//===========================================================
// コンストラクタ
//===========================================================
CMapObject_TrashBox::CMapObject_TrashBox()
{

}

//===========================================================
// コンストラクタ
//===========================================================
CMapObject_TrashBox::CMapObject_TrashBox(const char* aModelFliename, int nPriority) : CObjectX(aModelFliename, nPriority)
{

}

//===========================================================
// デストラクタ
//===========================================================
CMapObject_TrashBox::~CMapObject_TrashBox()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMapObject_TrashBox::Init(void)
{
	CObjectX::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject_TrashBox::Uninit(void)
{
	CObjectX::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CMapObject_TrashBox::Update(void)
{
}

//===========================================================
// 描画処理
//===========================================================
void CMapObject_TrashBox::Draw(void)
{
	CObjectX::Draw();
}

//===========================================================
// 生成処理
//===========================================================
CMapObject_TrashBox* CMapObject_TrashBox::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_TrashBox* pTrashBox = new CMapObject_TrashBox(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}