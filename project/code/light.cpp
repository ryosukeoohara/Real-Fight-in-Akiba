//===========================================================
//
// ライト処理 [light.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "main.h"
#include "light.h"
#include "renderer.h"
#include "manager.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const int NUMLIGHT = 3;  // ライトの総数
}

//===========================================================
// コンストラクタ
//===========================================================
CLight::CLight()
{
	
}

//===========================================================
// デストラクタ
//===========================================================
CLight::~CLight()
{

}

//===========================================================
// 初期化処理
//===========================================================
void CLight::Init(void)
{
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (pRenderer == nullptr)
		return;

	if (pDevice == nullptr)
		return;

	// ライトの情報をクリアする
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	for (int nCntLight = 0; nCntLight < NUMLIGHT; nCntLight++)
	{
		// ライトの種類を設定
		m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{
		case 0:
			// ライトの拡散校を設定
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向を設定
			m_vecDir[nCntLight] = D3DXVECTOR3(-0.05f, -0.87f, 0.05f);
			break;

		case 1:
			// ライトの拡散校を設定
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向を設定
			m_vecDir[nCntLight] = D3DXVECTOR3(0.46f, 0.88f, -0.44f);
			break;

		case 2:
			// ライトの拡散校を設定
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向を設定
			m_vecDir[nCntLight] = D3DXVECTOR3(-0.60f, -0.82f, 0.42f);
			break;
		}

		//ベクトルを正規化する
		D3DXVec3Normalize(&m_vecDir[nCntLight], &m_vecDir[nCntLight]);

		m_light[nCntLight].Direction = m_vecDir[nCntLight];

		//ライトを設定する
		pDevice->SetLight(nCntLight, &m_light[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//===========================================================
// 終了処理
//===========================================================
void CLight::Uninit(void)
{

}

//===========================================================
// 更新処理
//===========================================================
void CLight::Update(void)
{

}