//===========================================================
//
// UIマネージャ[UImanager.cpp]
// Author 大原怜将
//
//===========================================================

//*==========================================================
// インクルードファイル
//*==========================================================
#include "UImanager.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "camera.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{

}

//===========================================================
// コンストラクタ
//===========================================================
CUIManager::CUIManager()
{
	// 初期化
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_Info.type = TYPE_NONE;
	m_nIdxTexture = -1;
}

//===========================================================
// コンストラクタ
//===========================================================
CUIManager::CUIManager(D3DXVECTOR3 pos, TYPE type) : CObject2D::CObject2D(pos)
{
	// 初期化
	m_Info.pos = pos;
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_Info.type = type;
	m_nIdxTexture = -1;
}

//===========================================================
// デストラクタ
//===========================================================
CUIManager::~CUIManager()
{

}

//===========================================================
// 生成処理
//===========================================================
CUIManager * CUIManager::Create(D3DXVECTOR3 pos, TYPE type)
{
	// UIマネージャへのポインタ
	CUIManager *pUI = nullptr;

	// 生成
	pUI = new CUIManager(pos, type);

	if (pUI != nullptr)
	{
		// 初期化処理
		pUI->Init();
	}

	return pUI;
}

//===========================================================
// 線の移動
//===========================================================
void CUIManager::LineMove(void)
{
	m_Info.move.x = 10.0f;
	m_Info.pos.x += m_Info.move.x;

	CObject2D::SetPosition(D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y, m_Info.pos.z));
	CObject2D::SetSize(640.0f, 10.0f);
}

//===========================================================
// ちっちゃくないよ！
//===========================================================
void CUIManager::ShrinkSize(void)
{
	float fHeight = CObject2D::GetHeight();
	float fWidth = CObject2D::GetWidth();

	if (CObject2D::GetHeight() >= 100.0f && CObject2D::GetWidth() >= 30.0f)
	{
		fHeight = fHeight - 25.0f;
		fWidth = fWidth - 50.0f;
	}
	else
	{

	}
	
	CObject2D::SetSize(fWidth, fHeight);

	if (CManager::GetInstance()->GetCamera()->GetMode() == CCamera::MODE_GAME)
	{
		Uninit();
	}
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CUIManager::Init(void)
{
	//テクスチャの情報取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//初期化処理
	CObject2D::Init();

	// テクスチャ、サイズ、位置、描画するかどうか設定
	switch (m_Info.type)
	{
	case TYPE_NONE:     //なんもない
		break;

	case TYPE_TITLE:    //タイトル

		CObject2D::SetIdxTex(pTexture->Regist("data\\TEXTURE\\title.png"));
		CObject2D::SetPosition(m_Info.pos);
		CObject2D::SetSize(500.0f, 400.0f);
		CObject2D::SetDraw();
		break;

	case TYPE_TUTORIAL: //チュートリアル

		CObject2D::SetIdxTex(pTexture->Regist("data\\TEXTURE\\tutorial.jpg"));
		CObject2D::SetPosition(m_Info.pos);
		CObject2D::SetSize(500.0f, 400.0f);
		CObject2D::SetDraw();
		break;

	case TYPE_RESULT:   //リザルト

		CObject2D::SetIdxTex(pTexture->Regist("data\\TEXTURE\\res.jpg"));
		CObject2D::SetPosition(m_Info.pos);
		CObject2D::SetSize(500.0f, 400.0f);
		CObject2D::SetDraw();
		break;

	case TYPE_WEAKNAME:

		CObject2D::SetIdxTex(pTexture->Regist("data\\TEXTURE\\underling.png"));
		CObject2D::SetPosition(m_Info.pos);
		CObject2D::SetSize(1200.0f, 600.0f);
		CObject2D::SetDraw();
		break;

	case TYPE_BOSSNAME:

		CObject2D::SetIdxTex(pTexture->Regist("data\\TEXTURE\\ishii.png"));
		CObject2D::SetPosition(m_Info.pos);
		CObject2D::SetSize(1200.0f, 600.0f);
		CObject2D::SetDraw();
		break;

	case TYPE_LINE:

		CObject2D::SetIdxTex(pTexture->Regist("data\\TEXTURE\\line.png"));
		CObject2D::SetPosition(m_Info.pos);
		CObject2D::SetSize(640.0f, 10.0f);
		CObject2D::SetDraw(true);
		break;

	case TYPE_MAX:
		break;

	default:
		break;
	}

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CUIManager::Uninit(void)
{
	//終了処理
	CObject2D::Uninit();

	//自身の破棄
	Release();
}

//===========================================================
// 更新処理
//===========================================================
void CUIManager::Update(void)
{
	//更新処理
	CObject2D::Update();

	// 大きさとか設定
	switch (m_Info.type)
	{
	case TYPE_NONE:      //なんもない

		break;

	case TYPE_TITLE:     //タイトル

		break;

	case TYPE_TUTORIAL:  //チュートリアル

		break;

	case TYPE_RESULT:    //リザルト

		break;

	case TYPE_WEAKNAME:  // 雑魚の名前
		ShrinkSize();
		
		break;

	case TYPE_BOSSNAME:  // ボスの名前
		ShrinkSize();
		
		break;

	case TYPE_LINE:      // 線の移動
		LineMove();
		
		break;

	case TYPE_MAX:
		break;

	default:
		break;
	}
}

//===========================================================
// 描画処理
//===========================================================
void CUIManager::Draw(void)
{
	//テクスチャの情報を取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//レンダラーの情報を取得
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	//デバイスの情報を取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャ設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//描画処理
	CObject2D::Draw();
}