//===========================================================
//
// エフェクト処理[effect.cpp]
// Author 大原怜将
//
//===========================================================
#include "main.h"
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "effect.h"
#include "debugproc.h"
#include "texture.h"

//===========================================================
// 静的メンバ変数宣言
//===========================================================
const char *CEffect::m_apTexName[TYPE_MAX] =
{
	"data\\TEXTURE\\smoke_05.png",
	"data\\TEXTURE\\smoke_05.png",
	"data\\TEXTURE\\shadow000.jpg",
	"data\\TEXTURE\\smoke_05.png",
};

//===========================================================
// コンストラクタ
//===========================================================
CEffect::CEffect()
{
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_Info.nLife = 0;
	m_Info.fRadius = 0.0f;
	m_Info.bAlphaBlend = false;
	m_nIdxTexture = -1;
}

//===========================================================
// コンストラクタ
//===========================================================
CEffect::CEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, TYPE type, int nPriority) : CBillBoard(nPriority)
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
CEffect::~CEffect()
{

}

//===========================================================
// 生成処理
//===========================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, TYPE type, int nPriority)
{
	//オブジェクト2Dのポインタ
	CEffect *pEffect = nullptr;

	if (pEffect == nullptr)
	{
		//オブジェクト2Dの生成
		pEffect = new CEffect(pos, move, col, fRadius, nLife, type);

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
HRESULT CEffect::Init(void)
{
	//初期化処理
	CBillBoard::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CEffect::Uninit(void)
{
	//終了処理
	CBillBoard::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CEffect::Update(void)
{
	m_Info.nLife--;

	//更新処理
	CBillBoard::Update();

	switch (m_type)
	{
	case TYPE_GROUND:
		Ground();
		break;

	case TYPE_SMOOK:
		Smook();
		break;

	case TYPE_BLOOD:

		Blood();
		break;

	case TYPE_BREAKDOWN:
		BreakDown();
		break;

	case TYPE_MAX:
		break;

	default:
		break;
	}

	m_Info.pos.x += m_Info.move.x;
	m_Info.pos.y += m_Info.move.y;
	m_Info.pos.z += m_Info.move.z;

	SetPosition(m_Info.pos);

	if (m_Info.nLife <= 0)
	{
		CEffect::Uninit();
	}
}

//===========================================================
// 描画処理
//===========================================================
void CEffect::Draw(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	////αブレンディングを加算合計に設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//描画処理
	CBillBoard::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================================================
// 土煙のエフェクト
//===========================================================
void CEffect::Ground(void)
{
	m_Info.move.x += (0.0f - m_Info.move.x) * 0.1f;
	m_Info.move.z += (0.0f - m_Info.move.z) * 0.1f;

	if(m_Info.fRadius >= 0.0f)
	   m_Info.fRadius -= 0.5f;

	SetColor(m_Info.col);
	SetSize(m_Info.fRadius, m_Info.fRadius);
}

//===========================================================
// 煙のエフェクト
//===========================================================
void CEffect::Smook(void)
{
	if (m_Info.col.a >= 0.0f)
	{
		m_Info.col.a -= 0.05f;
	}

	m_Info.move.x += (0.0f - m_Info.move.x) * 0.1f;
	m_Info.move.z += (0.0f - m_Info.move.z) * 0.1f;

	m_Info.fRadius += 0.5f;

	SetColor(m_Info.col);
	SetSize(m_Info.fRadius, m_Info.fRadius);
}

//===========================================================
// 故障
//===========================================================
void CEffect::BreakDown(void)
{
	if (m_Info.col.a >= 0.0f)
	{
		m_Info.col.a -= 0.005f;
	}

	m_Info.fRadius += 1.0f;

	SetColor(m_Info.col);
	SetSize(m_Info.fRadius, m_Info.fRadius);
}

//===========================================================
// 血液
//===========================================================
void CEffect::Blood(void)
{
	D3DXVECTOR3 pos = GetPosition();  //位置取得

	if(m_Info.fRadius < 0.0f)
	   m_Info.fRadius -= 0.5f;

	if (pos.y >= 60.0f)
		m_Info.move.y *= -1;

}