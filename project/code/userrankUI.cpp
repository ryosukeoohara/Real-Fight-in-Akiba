//===========================================================
//
// 敵登場時のUI[appearanceUI.cpp]
// Author 大原怜将
//
//===========================================================
#include "userrankUI.h"
#include "object2D.h"
#include "manager.h"
#include "texture.h"
#include "camera.h"
#include "number.h"
#include "utility.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const char *TEXTURENAME[CUserRankUI::TYPE_MAX] =
	{
		"",                             // なんもなし
		"data\\TEXTURE\\00_Result.png",	// 最高
		"data\\TEXTURE\\01_Result.png", // 中間
		"data\\TEXTURE\\02_Result.png", // 下っ端
		"data\\TEXTURE\\03_Result.png", // 最弱
	};  // テクスチャの名前
}

//===========================================================
// コンストラクタ
//===========================================================
CUserRankUI::CUserRankUI()
{
	// 初期化
	m_type = TYPE_NOEN;
	m_pObject = nullptr;
}

//===========================================================
// コンストラクタ
//===========================================================
CUserRankUI::CUserRankUI(TYPE type, int nPriority)
{
	// 初期化
	m_type = type;
	m_pObject = nullptr;
}

//===========================================================
// デストラクタ
//===========================================================
CUserRankUI::~CUserRankUI()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CUserRankUI::Init(void)
{
	if (m_pObject == nullptr)
	{// 使用していないとき

		// 生成して、位置、サイズ、テクスチャ、描画するか設定
		m_pObject = CObject2D::Create();
		m_pObject->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f));
		m_pObject->SetSize(600.0f, 150.0f);
		m_pObject->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME[m_type]));
		m_pObject->SetDraw();
	}

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CUserRankUI::Uninit(void)
{
	// 破棄
	if (m_pObject != nullptr)
	{
		m_pObject->Uninit();
		m_pObject = nullptr;
	}

	Release();
}

//===========================================================
// 更新処理
//===========================================================
void CUserRankUI::Update(void)
{
	if (m_pObject != nullptr)
	{
		// 幅と高さ取得
		float fHeight = m_pObject->GetHeight();
		float fWidth = m_pObject->GetWidth();

		if (m_pObject->GetHeight() >= 100.0f && m_pObject->GetWidth() >= 30.0f)
		{// この数値以下になるまで小さくする

			// ポリゴンを小さくする
			utility::Shrink(m_pObject, 25.0f, 50.0f);
		}
	}
}

//===========================================================
// 描画処理
//===========================================================
void CUserRankUI::Draw(void)
{

}

//===========================================================
// 生成処理
//===========================================================
CUserRankUI * CUserRankUI::Create(TYPE type)
{
	CUserRankUI *pAppearUI = nullptr;
	pAppearUI = new CUserRankUI(type);

	if (pAppearUI != nullptr)
	{
		pAppearUI->Init();
	}

	return pAppearUI;
}