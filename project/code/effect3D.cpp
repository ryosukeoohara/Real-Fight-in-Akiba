//===========================================================
//
// モデルのエフェクト[effect3D.h]
// Author 大原怜将
//
//===========================================================
#include "effect3D.h"

//===========================================================
// コンストラクタ
//===========================================================
CEffect3D::CEffect3D()
{
	// 値をクリア
	m_Info.pos = D3DXVECTOR3();
	m_Info.rot = D3DXVECTOR3();
	m_Info.move = D3DXVECTOR3();
	m_Info.col = D3DXCOLOR();
	m_Info.nLife = 0;
	m_type = TYPE_NONE;
}

//===========================================================
// コンストラクタ
//===========================================================
CEffect3D::CEffect3D(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, TYPE type, const char *modelfilename) : CObjectX(modelfilename)
{
	// 値をクリア
	m_Info.pos = pos;
	m_Info.rot = rot;
	m_Info.move = move;
	m_Info.nLife = nLife;
	m_type = type;
}

//===========================================================
// デストラクタ
//===========================================================
CEffect3D::~CEffect3D()
{
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CEffect3D::Init(void)
{
	CObjectX::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CEffect3D::Uninit(void)
{
	CObjectX::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CEffect3D::Update(void)
{
	m_Info.move.y -= 0.9f;

	CObjectX::Update();

	// 種類別
	switch (m_type)
	{
	case TYPE_NONE:
		break;

	case TYPE_BIKE:

		Bike();
		break;

	case TYPE_GLASS:

		Glass();
		break;

	default:
		break;
	}
	
	m_Info.pos.x += m_Info.move.x;
	m_Info.pos.y += m_Info.move.y;
	m_Info.pos.z += m_Info.move.z;

	if (m_Info.pos.y <= 0.0f)
	{
		m_Info.pos.y = 0.0f;
	}

	SetPosition(m_Info.pos);
	SetRotition(m_Info.rot);

	m_Info.nLife--;

	if (m_Info.nLife <= 0)
	{
		CEffect3D::Uninit();
	}
}

//===========================================================
// 描画処理
//===========================================================
void CEffect3D::Draw(void)
{
	CObjectX::Draw();
}

//===========================================================
// 自転車
//===========================================================
void CEffect3D::Bike(void)
{
	m_Info.move.x += (0.0f - m_Info.move.x) * 0.1f;
	m_Info.move.z += (0.0f - m_Info.move.z) * 0.1f;
}

//===========================================================
// ガラスの破片
//===========================================================
void CEffect3D::Glass(void)
{

}



//===========================================================
// コンストラクタ
//===========================================================
CEffect3D * CEffect3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, TYPE type, const char *modelfilename)
{
	CEffect3D *pEffect3D = nullptr;

	pEffect3D = new CEffect3D(pos, move, rot, nLife, type, modelfilename);

	if (pEffect3D != nullptr)
	{
		pEffect3D->Init();
		pEffect3D->SetPosition(pos);
		pEffect3D->SetRotition(rot);
	}

	return nullptr;
}