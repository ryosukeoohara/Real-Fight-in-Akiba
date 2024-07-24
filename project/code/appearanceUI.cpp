//===========================================================
//
// 敵登場時のUI[appearanceUI.cpp]
// Author 大原怜将
//
//===========================================================
#include "appearanceUI.h"
#include "object2D.h"
#include "manager.h"
#include "texture.h"
#include "camera.h"
#include "number.h"
#include "utility.h"
#include "sound.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const char *TEXTURENAME[CAppearanceUI::TYPE_MAX] =
	{
		"",
		"data\\TEXTURE\\underling.png", // ザコ
		"data\\TEXTURE\\ishii.png",     // ボス
		"data\\TEXTURE\\number002.png", // 数字
	};  // テクスチャの名前
}

//===========================================================
// コンストラクタ
//===========================================================
CAppearanceUI::CAppearanceUI()
{
	// 初期化
	m_type = TYPE_NONE;
	m_pFrontObj2D = nullptr;
	m_pBackObj2D = nullptr;
	m_pNumber = nullptr;
	m_bSound = false;
}

//===========================================================
// コンストラクタ
//===========================================================
CAppearanceUI::CAppearanceUI(TYPE type, int nPriority) : CObject(nPriority)
{
	// 初期化
	m_type = type;
	m_pFrontObj2D = nullptr;
	m_pBackObj2D = nullptr;
	m_pNumber = nullptr;
	m_bSound = false;
}

//===========================================================
// デストラクタ
//===========================================================
CAppearanceUI::~CAppearanceUI()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CAppearanceUI::Init(void)
{
	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		// それぞれ生成して、位置、サイズ、テクスチャ、描画状態を設定
		if (m_pBackObj2D == nullptr)
		{
			m_pBackObj2D = CObject2D::Create();
			m_pBackObj2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			m_pBackObj2D->SetSize(0.0f, 0.0f);
			m_pBackObj2D->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME[m_type]));
			m_pBackObj2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pBackObj2D->SetDraw(false);
		}

		if (m_pFrontObj2D == nullptr)
		{
			m_pFrontObj2D = CObject2D::Create();
			m_pFrontObj2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			m_pFrontObj2D->SetSize(1200.0f, 600.0f);
			m_pFrontObj2D->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME[m_type]));
			m_pFrontObj2D->SetDraw();
		}
	}
	
	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_RESULT)
	{
		// それぞれ生成して、位置、サイズ、テクスチャ、描画状態を設定
		if (m_pBackObj2D == nullptr)
		{
			m_pBackObj2D = CObject2D::Create();
			m_pBackObj2D->SetPosition(D3DXVECTOR3(750.0f, 225.0f, 0.0f));
			m_pBackObj2D->SetSize(0.0f, 0.0f);
			m_pBackObj2D->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME[m_type]));
			m_pBackObj2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_pBackObj2D->SetTex((float)CManager::GetInstance()->GetDefeat());
			m_pBackObj2D->SetDraw(false);
		}

		if (m_pNumber == nullptr)
		{
			m_pNumber = CObject2D::Create();
			m_pNumber->SetPosition(D3DXVECTOR3(750.0f, 225.0f, 0.0f));
			m_pNumber->SetSize(200.0f, 200.0f);
			m_pNumber->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME[m_type]));
			m_pNumber->SetTex((float)CManager::GetInstance()->GetDefeat());
			m_pNumber->SetDraw();
		}
	}

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CAppearanceUI::Uninit(void)
{
	// 破棄
	if (m_pFrontObj2D != nullptr)
	{
		m_pFrontObj2D->Uninit();
		m_pFrontObj2D = nullptr;
	}

	if (m_pBackObj2D != nullptr)
	{
		m_pBackObj2D->Uninit();
		m_pBackObj2D = nullptr;
	}

	if (m_pNumber != nullptr)
	{
		m_pNumber->Uninit();
		m_pNumber = nullptr;
	}

	Release();
}

//===========================================================
// 更新処理
//===========================================================
void CAppearanceUI::Update(void)
{
	if (m_pFrontObj2D != nullptr && m_pBackObj2D != nullptr)
	{// 前と後ろ両方使用していたら

		// 幅と高さ取得
		float fHeight = m_pFrontObj2D->GetHeight();
		float fWidth = m_pFrontObj2D->GetWidth();

		if (m_pFrontObj2D->GetHeight() >= 100.0f && m_pFrontObj2D->GetWidth() >= 30.0f)
		{// この数値以下になるまで小さくする

			// ポリゴンを小さくする
			utility::Shrink(m_pFrontObj2D, 25.0f, 50.0f);

		}
		else
		{// 小さくなった

			if (m_bSound == false)
			{
				CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL_SE_IMPACT);
				m_bSound = true;
			}

			// ポリゴンを大きくする
			utility::Enlarge(m_pBackObj2D, 5.0f, 10.0f);

			// ポリゴンを透明にする
			utility::Color_A2D(m_pBackObj2D, -0.03f);
		}

		if (CManager::GetInstance()->GetCamera()->GetMode() == CCamera::MODE_GAME)
		{
			D3DXVECTOR3 pos = m_pFrontObj2D->GetPosition();

			pos.x = pos.x + 60.0f;

			m_pFrontObj2D->SetPosition(D3DXVECTOR3(pos.x, pos.y, pos.z));

			// サイズ設定
			m_pFrontObj2D->SetSize(fWidth, fHeight);

			if (pos.x >= 1500.0f)
			{
				Uninit();
			}
		}
	}

	if (m_pNumber != nullptr && m_pBackObj2D != nullptr)
	{
		// 幅と高さ取得
		float fHeight = m_pNumber->GetHeight();
		float fWidth = m_pNumber->GetWidth();

		if (m_pNumber->GetHeight() >= 50.0f && m_pNumber->GetWidth() >= 50.0f)
		{// この数値以下になるまで小さくする

			// ポリゴンを小さくする
			utility::Shrink(m_pNumber, 10.0f, 10.0f);
		}
		else
		{// 小さくなった

			// ポリゴンを大きくする
			utility::Enlarge(m_pBackObj2D, 5.0f, 10.0f);

			// ポリゴンを透明にする
			utility::Color_A2D(m_pBackObj2D, -0.03f);
		}
	}
}

//===========================================================
// 描画処理
//===========================================================
void CAppearanceUI::Draw(void)
{

}

//===========================================================
// 生成処理
//===========================================================
CAppearanceUI * CAppearanceUI::Create(TYPE type, int nPriority)
{
	CAppearanceUI *pAppearUI = nullptr;
	pAppearUI = new CAppearanceUI(type, nPriority);

	if (pAppearUI != nullptr)
	{
		pAppearUI->Init();
	}

	return pAppearUI;
}
