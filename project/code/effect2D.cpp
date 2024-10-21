//===========================================================
//
// 2Dエフェクト[effect2D.cpp]
// Author 大原怜将
//
//===========================================================
#include "main.h"
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "effect2D.h"
#include "debugproc.h"
#include "texture.h"

//===========================================================
// 静的メンバ変数宣言
//===========================================================
const char *CEffect2D::m_apTexName[TYPE_MAX] =
{
	"data\\TEXTURE\\smook.png",
	"data\\TEXTURE\\smook.png",
};

//===========================================================
// コンストラクタ
//===========================================================
CEffect2D::CEffect2D()
{
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_Info.nLife = 0;
	m_Info.fRadius = 0.0f;
	m_nIdxTexture = -1;
}

//===========================================================
// コンストラクタ
//===========================================================
CEffect2D::CEffect2D(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, TYPE type)
{
	m_Info.pos = pos;
	m_Info.move = move;
	m_Info.col = col;
	m_Info.nLife = nLife;
	m_Info.fRadius = fRadius;
	m_type = type;
	m_nIdxTexture = -1;
}

//===========================================================
// デストラクタ
//===========================================================
CEffect2D::~CEffect2D()
{

}

//===========================================================
// 生成処理
//===========================================================
CEffect2D *CEffect2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, TYPE type)
{
	//オブジェクト2Dのポインタ
	CEffect2D *pEffect = nullptr;

	if (pEffect == nullptr)
	{
		//オブジェクト2Dの生成
		pEffect = new CEffect2D(pos, move, col, fRadius, nLife, type);

		//初期化処理
		pEffect->Init();
		pEffect->SetPosition(pos);
		pEffect->SetSize(fRadius, fRadius);
		pEffect->SetColor(col);
		pEffect->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(m_apTexName[type]));
		pEffect->SetDraw(true);
	}
	
	return pEffect;
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CEffect2D::Init(void)
{
	//初期化処理
	CObject2D::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CEffect2D::Uninit(void)
{
	//終了処理
	CObject2D::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CEffect2D::Update(void)
{
	m_Info.nLife--;

	//更新処理
	CObject2D::Update();

	switch (m_type)
	{
	case TYPE_GROUND:
		Ground();
		break;

	case TYPE_SMOOK:
		Smook();
		break;

	case TYPE_MAX:
		break;

	default:
		break;
	}

	//移動量を更新(減衰させる)--------------------------------------------
	m_Info.move.x += (0.0f - m_Info.move.x) * 0.1f;
	m_Info.move.y += (0.0f - m_Info.move.y) * 0.1f;

	m_Info.pos.x += m_Info.move.x;
	m_Info.pos.y += m_Info.move.y;

	SetPosition(m_Info.pos);

	if (m_Info.nLife <= 0)
	{
		CEffect2D::Uninit();
	}
}

//===========================================================
// 描画処理
//===========================================================
void CEffect2D::Draw(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//αブレンディングを加算合計に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//描画処理
	CObject2D::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================================================
// 土煙のエフェクト
//===========================================================
void CEffect2D::Ground(void)
{
	m_Info.move.x += (0.0f - m_Info.move.x) * 0.1f;
	m_Info.move.y += (0.0f - m_Info.move.y) * 0.1f;
}

//===========================================================
// 血液のエフェクト
//===========================================================
void CEffect2D::Blood(void)
{

}

//===========================================================
// 煙のエフェクト
//===========================================================
void CEffect2D::Smook(void)
{
	if (m_Info.col.a >= 0.0f)
	{
		m_Info.col.a -= 0.005f;
	}

	m_Info.fRadius += 1.0f;

	SetColor(m_Info.col);
	SetSize(m_Info.fRadius, m_Info.fRadius);
}
