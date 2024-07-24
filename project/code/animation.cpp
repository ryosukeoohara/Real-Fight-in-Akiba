//===========================================================
//
// テクスチャアニメーション[animation.cpp]
// Author 大原怜将
//
//===========================================================
#include "animation.h"
#include "manager.h"
#include "texture.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const char *TEXTURENAME[CBillBoard::TYPE_MAX] =
	{
		"data\\TEXTURE\\.png",
		"data\\TEXTURE\\hit1.png",
	};  // テクスチャの名前
}

//===========================================================
// コンストラクタ
//===========================================================
CAnimation::CAnimation()
{
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_Info.type = TYPE_NONE;
	m_Info.nLife = 0;
	m_Info.fRadius = 0.0f;
}

//===========================================================
// コンストラクタ
//===========================================================
CAnimation::CAnimation(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, TYPE type)
{
	m_Info.pos = pos;
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.col = col;
	m_Info.type = type;
	m_Info.nLife = 0;
	m_Info.fRadius = fRadius;
}

//===========================================================
// デストラクタ
//===========================================================
CAnimation::~CAnimation()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CAnimation::Init(void)
{
	CBillBoard::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CAnimation::Uninit(void)
{
	CBillBoard::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CAnimation::Update(void)
{
	CBillBoard::Update();

	// アニメーション設定
	CBillBoard::SetAnim();
}

//===========================================================
// 描画処理
//===========================================================
void CAnimation::Draw(void)
{
	CBillBoard::Draw();
}

//===========================================================
// 生成
//===========================================================
CAnimation * CAnimation::Create(D3DXVECTOR3 pos, float fRadius, CBillBoard::TYPE type)
{
	CAnimation *pAnim = nullptr;
	pAnim = new CAnimation;

	if (pAnim != nullptr)
	{
		// 種類、位置、サイズ、テクスチャ、描画設定
		pAnim->SetType(type);
		pAnim->Init();
		pAnim->SetPosition(pos);
		pAnim->SetSize(fRadius, fRadius);
		pAnim->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME[type]));
		pAnim->SetDraw();
	}

	return pAnim;
}
