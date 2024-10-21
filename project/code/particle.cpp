//===========================================================
//
// パーティクル処理[particl.cpp]
// Author 大原怜将
//
//===========================================================
#include "main.h"
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "effect.h"
#include "effect3D.h"
#include "particle.h"
#include "player.h"
#include "utility.h"
#include <time.h>

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const int BLOOD = 20;        // 血液のパーティクル
	const int GROUND = 20;      // 土煙のパーティクル
	const int CIRCLE = 314;     // 円形のパーティクル
	const float SPEED = 10.0f;  // 移動量
}

//===========================================================
//静的メンバ変数宣言
//===========================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = nullptr;

//===========================================================
// コンストラクタ
//===========================================================
CParticle::CParticle()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRadius = 0;
	m_nLife = 0;
}

//===========================================================
// コンストラクタ
//===========================================================
CParticle::CParticle(D3DXVECTOR3 pos, TYPE type)
{
	m_pos = pos;
	m_fRadius = 0;
	m_nLife = 0;
	m_type = type;
}

//===========================================================
// デストラクタ
//===========================================================
CParticle::~CParticle()
{

}

//===========================================================
// 生成処理
//===========================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, TYPE type)
{
	//オブジェクト2Dのポインタ
	CParticle *pParticl = NULL;

	if (pParticl == NULL)
	{
		//オブジェクト2Dの生成
		pParticl = new CParticle(pos, type);

		//初期化処理
		pParticl->Init();
	}
	
	return pParticl;
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CParticle::Init(void)
{
	switch (m_type)
	{
	case TYPE_GROUND:
		Ground();
		break;

	case TYPE_BLOOD:
		Blood();
		break;

	case TYPE_SMOOK:
		Smook();
		break;

	case TYPE_CIRCLE:
		Circle();
		break;

	case TYPE_GLASS:
		Glass();
		break;

	case TYPE_BREAKDOWN:
		BreakDown();
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
void CParticle::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//===========================================================
// 更新処理
//===========================================================
void CParticle::Update(void)
{
	
}

//===========================================================
// 描画処理
//===========================================================
void CParticle::Draw(void)
{
	
}

//===========================================================
// 描画処理
//===========================================================
void CParticle::Move(void)
{
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		float fMove, fRot;

		fMove = (float)(rand() % 50) / 100.0f * 8.0f;
		fRot = (float)(rand() % 629 - 314) / 100.0f;

		m_move.x = sinf(fRot) * fMove;
		m_move.y = cosf(fRot) * fMove;
		m_move.z = cosf(fRot) * fMove;

		//CEffect::Create(pos, { m_move.x, m_move.y, m_move.z }, { 0.8f, 1.0f, 1.0f, 1.0f }, m_fRadius, 30, CEffect::TYPEEFF_NONE);
	}
}

//===========================================================
// 血液
//===========================================================
void CParticle::Blood(void)
{
	for (int nCnt = 0; nCnt < BLOOD; nCnt++)
	{
		float fMove, fRange;

		fMove = (float)(rand() % 50) / 100.0f * 8.0f;
		
		fRange = (float)(rand() % 10) - D3DX_PI * 2;

		m_move.x = sinf(fRange) * fMove;
		m_move.y = cosf(fRange) * fMove;
		m_move.z = cosf(fRange) * fMove;

		if (m_move.z > 0.0f)
		{
			m_move.z *= -1;
		}

		CEffect::Create(m_pos, { m_move.x, m_move.y, m_move.z }, { 1.0f, 0.0f, 0.0f, 1.0f }, 10.0f, 30, CEffect::TYPE_BLOOD);
	}
}

//===========================================================
// 土埃
//===========================================================
void CParticle::Ground(void)
{
	float fRot = 0.0f;

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	for (int i = 0; i < 3; i++)
	{
		float fMove = 0.0f, fMoveY = 0.0f, fRot = 0.0f;

		fMove = (float)(rand() % 25 * 0.1f);
		fMoveY = (float)(rand() % 10 * 0.1f);

		fRot = pPlayer->GetRotition().y + (float)(rand() % 10 * 0.1f);

		fRot = utility::CorrectAngle(fRot);

		// 移動量設定
		m_move.x = sinf(fRot) * fMove;
		m_move.y = fMoveY;
		m_move.z = cosf(fRot) * fMove;

		CEffect::Create({ m_pos.x, -10.0f, m_pos.z }, m_move, { 0.5f, 0.3f, 0.2f, 0.7f }, 25.0f, 30, CEffect::TYPE_GROUND);
	}
}

//===========================================================
// 煙 
//===========================================================
void CParticle::Smook(void)
{
	float fRot = 0.0f;

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	for (int i = 0; i < 1; i++)
	{
		float fMove = 0.0f, fMoveY = 0.0f, fRot = 0.0f;

		fMove = (float)(rand() % 25 * 0.1f);
		fMoveY = (float)(rand() % 10 * 0.1f);

		fRot = pPlayer->GetRotition().y + (float)(rand() % 10 * 0.1f);

		fRot = utility::CorrectAngle(fRot);

		// 移動量設定
		m_move.x = sinf(fRot) * fMove;
		m_move.y = fMoveY;
		m_move.z = cosf(fRot) * fMove;

		CEffect::Create({ m_pos.x, -10.0f, m_pos.z }, m_move, { 0.5f, 0.3f, 0.2f, 0.7f }, 25.0f, 30, CEffect::TYPE_SMOOK);
	}
}

//===========================================================
// 円形
//===========================================================
void CParticle::Circle(void)
{
	
}

//===========================================================
// ガラスの破片
//===========================================================
void CParticle::Glass(void)
{
	for (int nCnt = 0; nCnt < 30; nCnt++)
	{
		int nLife = 0;
		float fRadius = 0;
		D3DXVECTOR3 move = {};
		D3DXVECTOR3 rot = {};

		//移動量の設定
		move.x = sinf((float)(rand() % 50) / 100.0f * 8.0f) * 5.0f;
		move.y = 10.0f;
		move.z = cosf((float)(rand() % 50) / 100.0f * 8.0f);

		if (move.x < 0.0f)
		{
			move.x *= -1.0f;
		}

		//寿命の設定
		nLife = 25;

		CEffect3D::Create(D3DXVECTOR3(m_pos.x + 30.0f, m_pos.y, m_pos.z), move, rot, nLife, CEffect3D::TYPE_GLASS, "data\\MODEL\\glass.x");
	}
}

//===========================================================
// 自転車の残骸
//===========================================================
void CParticle::BikeScrap(void)
{
	float fMove, fRot;

	fMove = (float)(rand() % 50) / 100.0f * 8.0f;
	fRot = (float)(rand() % 629 - 314) / 100.0f;

	m_move.x = sinf(fRot) * fMove;
	m_move.y = cosf(fRot) * fMove;
	m_move.z = cosf(fRot) * fMove;

	CEffect3D::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 20.0f, m_pos.z), D3DXVECTOR3(m_move.x, m_move.y + 10.0f, m_move.z), D3DXVECTOR3(fRot, fRot, fRot), 60, CEffect3D::TYPE_BIKE, "data\\MODEL\\bike\\tier.x");
	CEffect3D::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 20.0f, m_pos.z), D3DXVECTOR3(m_move.x, m_move.y + 10.0f, m_move.z), D3DXVECTOR3(fRot, fRot, fRot), 60, CEffect3D::TYPE_BIKE, "data\\MODEL\\bike\\basket.x");
	CEffect3D::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 20.0f, m_pos.z), D3DXVECTOR3(m_move.x, m_move.y + 10.0f, m_move.z), D3DXVECTOR3(fRot, fRot, fRot), 60, CEffect3D::TYPE_BIKE, "data\\MODEL\\bike\\handle.x");
	CEffect3D::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 20.0f, m_pos.z), D3DXVECTOR3(m_move.x, m_move.y + 10.0f, m_move.z), D3DXVECTOR3(fRot, fRot, fRot), 60, CEffect3D::TYPE_BIKE, "data\\MODEL\\bike\\pedal.x");
	CEffect3D::Create(D3DXVECTOR3(m_pos.x, m_pos.y + 20.0f, m_pos.z), D3DXVECTOR3(m_move.x, m_move.y + 10.0f, m_move.z), D3DXVECTOR3(fRot, fRot, fRot), 60, CEffect3D::TYPE_BIKE, "data\\MODEL\\bike\\tier.x");
	
}

//===========================================================
// 電子レンジ故障
//===========================================================
void CParticle::BreakDown(void)
{
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		int nLife = 0;
		float fRadius = 0;
		D3DXVECTOR3 move = {};
		D3DXCOLOR col = {};

		//移動量の設定
		move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * 0.09f;
		move.y = 20.0f * 0.05f;
		move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * 0.09f;

		//色の設定
		col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

		//半径の設定
		fRadius = 10.0f;

		//寿命の設定
		nLife = 500;

		CEffect::Create(m_pos, move, col, fRadius, nLife, CEffect::TYPE_SMOOK);
	}
}