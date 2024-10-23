//===========================================================
//
// チュートリアルのUI処理[tutorialUI.cpp]
// Author 大原怜将
//
//===========================================================
#include "tutorialUI.h"
#include "manager.h"
#include "texture.h"
#include "object2D.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const char *TEXTURENAME[CTutorialUI::TYPE_MAX] =
	{
		"data\\TEXTURE\\10_tutorial.png",  // 見出し
		"data\\TEXTURE\\00_tutorial.png",  // 移動
		"data\\TEXTURE\\01_tutorial.png",  // 視点移動
		"data\\TEXTURE\\02_tutorial.png",  // 攻撃
		"data\\TEXTURE\\03_tutorial.png",  // 回避
		"data\\TEXTURE\\04_tutorial.png",  // 掴み
		"data\\TEXTURE\\05_tutorial.png",  // 激アツ
	};  // 使用するテクスチャ
}

//===========================================================
// コンストラクタ
//===========================================================
CTutorialUI::CTutorialUI()
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		m_apObject2D[nCount] = nullptr;
	}
}

//===========================================================
// デストラクタ
//===========================================================
CTutorialUI::~CTutorialUI()
{
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CTutorialUI::Init(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_apObject2D[nCount] == nullptr)
		{// 使用していなかったら

			// 生成して、位置、サイズ、テクスチャ、描画するか設定
			m_apObject2D[nCount] = CObject2D::Create(7);
			m_apObject2D[nCount]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.85f, 150.0f + nCount * 40, 0.0f));
			m_apObject2D[nCount]->SetSize(150.0f, 20.0f);
			m_apObject2D[nCount]->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME[nCount]));
			m_apObject2D[nCount]->SetDraw();
		}
	}

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CTutorialUI::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		if (m_apObject2D[nCount] != nullptr)
		{// 使用していたら

			m_apObject2D[nCount]->Uninit();
			m_apObject2D[nCount] = nullptr;
		}
	}
}

//===========================================================
// 更新処理
//===========================================================
void CTutorialUI::Update(void)
{
}

//===========================================================
// 描画処理
//===========================================================
void CTutorialUI::Draw(void)
{
}

//===========================================================
// 生成処理
//===========================================================
CTutorialUI * CTutorialUI::Create(void)
{
	CTutorialUI *pUI = nullptr;
	pUI = new CTutorialUI;

	if (pUI != nullptr)
	{
		pUI->Init();
	}

	return pUI;
}
