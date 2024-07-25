//===========================================================
//
// エフェクシア[MyEffekseer.cpp]
// Author 大原怜将
//
//===========================================================
#include <stdio.h>
#include <string>
#include <windows.h>

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "MyEffekseer.h"
#include "camera.h"

// エフェクトの名前
const char* CMyEffekseer::m_apEfkName[CMyEffekseer::TYPE_MAX] =
{
	"",                                                // なんもない
	"data\\EFFEKSEER\\Effect\\impact.efkefc",          // 衝撃波
	"data\\EFFEKSEER\\Effect\\hit.efkefc",             // ヒット
	"data\\EFFEKSEER\\Effect\\attack_impact.efkefc",   // 攻撃の予兆
};

//===========================================================
// コンストラクタ
//===========================================================
CMyEffekseer::CMyEffekseer()
{
	// 値を初期化
	for (int i = 0; i < MAX_EFK; i++)
	{
		m_Info[i].pos = {};
		m_Info[i].rot = {};
		m_Info[i].effect = nullptr;
		m_Info[i].efkHandle = NULL;
		m_Info[i].EfkName = {};
		m_Info[i].time = 0;
	}

	m_nNum = 0;
}

//===========================================================
// デストラクタ
//===========================================================
CMyEffekseer::~CMyEffekseer()
{

}

//===========================================================
// 初期化処理
//===========================================================
void CMyEffekseer::Init(void)
{
	// エフェクトのマネージャーの作成
	m_EfkManager = ::Effekseer::Manager::Create(8000);

	// Specify a position of view
	// 視点位置を確定
	m_ViewerPosition = ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f);

	// 座標系を設定する。アプリケーションと一致させる必要がある。
	m_EfkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// Setup effekseer modules
	// Effekseerのモジュールをセットアップする
	SetupEffekseerModules(m_EfkManager);

	// Specify a projection matrix
	// 投影行列を設定
	m_ProjectionMatrix.PerspectiveFovLH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// Specify a camera matrix
		// カメラ行列を設定
	m_CameraMatrix.LookAtLH(m_ViewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
}

//===========================================================
// 終了処理
//===========================================================
void CMyEffekseer::Uninit(void)
{
	for (int i = 0; i < MAX_EFK; i++)
	{
		m_Info[i].pos = {};
		m_Info[i].rot = {};
		m_Info[i].effect = nullptr;
		m_Info[i].efkHandle = NULL;
		m_Info[i].EfkName = {};
		m_Info[i].time = 0;
	}

	m_nNum = 0;
}

//===========================================================
// 更新処理
//===========================================================
void CMyEffekseer::Update(void)
{
	for (int i = 0; i < MAX_EFK; i++)
	{
		if (m_Info[i].effect != nullptr)
		{
			m_Info[i].pos = m_EfkManager->GetLocation(m_Info[i].efkHandle);

			// Move the effect
			// エフェクトの移動
			m_EfkManager->AddLocation(m_Info[i].efkHandle, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f));

			// Set layer parameters
			// レイヤーパラメータの設定
			Effekseer::Manager::LayerParameter layerParameter;
			layerParameter.ViewerPosition = m_ViewerPosition;
			m_EfkManager->SetLayerParameter(0, layerParameter);

			// Update the manager
			// マネージャーの更新
			Effekseer::Manager::UpdateParameter updateParameter;
			m_EfkManager->Update(updateParameter);

			// Update a time
			// 時間を更新する
			m_EfkRenderer->SetTime(m_time / 60.0f);
			//m_Info[i].time++;
			
			// 毎フレーム、エフェクトが再生終了しているか確認する
			if (m_EfkManager->Exists(m_Info[i].efkHandle) == false)
			{
				// 新たにエフェクトを再生し直す。座標はエフェクトを表示したい場所を設定する
				// (位置や回転、拡大縮小も設定しなおす必要がある)
				m_EfkManager->StopEffect(m_Info[i].efkHandle);
				Release(i);
			}
		}
	}

	m_time++;
}

//===========================================================
// 描画処理
//===========================================================
void CMyEffekseer::Draw(void)
{
	if (m_EfkRenderer != nullptr)
	{
		// Specify a projection matrix
		// 投影行列を設定
		m_EfkRenderer->SetProjectionMatrix(m_ProjectionMatrix);

		CCamera* pCamera = CManager::GetInstance()->GetCamera();

		auto ViewMatrix = pCamera->GetView();
		auto Projection = pCamera->GetProjection();

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				m_ProjectionMatrix.Values[i][j] = Projection.m[i][j];
				m_CameraMatrix.Values[i][j] = ViewMatrix.m[i][j];
			}
		}

		// Specify a camera matrix
		// カメラ行列を設定
		m_EfkRenderer->SetCameraMatrix(m_CameraMatrix);

		// Begin to rendering effects
		// エフェクトの描画開始処理を行う。
		m_EfkRenderer->BeginRendering();

		// Render effects
		// エフェクトの描画を行う。
		Effekseer::Manager::DrawParameter drawParameter;
		drawParameter.ZNear = 0.0f;
		drawParameter.ZFar = 1.0f;
		drawParameter.ViewProjectionMatrix = m_EfkRenderer->GetCameraProjectionMatrix();
		m_EfkManager->Draw(drawParameter);

		// Finish to rendering effects
		// エフェクトの描画終了処理を行う。
		m_EfkRenderer->EndRendering();
	}
}

//===========================================================
// 設定
//===========================================================
void CMyEffekseer::Set(TYPE type, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale)
{
	for (int i = 0; i < MAX_EFK; i++)
	{
		if (m_Info[i].effect == nullptr)
		{
			// char16_tに変換
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
			std::u16string string16t = converter.from_bytes(m_apEfkName[type]);

			m_Info[i].pos = pos;
			m_Info[i].rot = rot;
			m_Info[i].scale = scale;
			m_Info[i].time = 0;

			// std::char_16
			// Load an effect
			// エフェクトの読込
			m_Info[i].effect = Effekseer::Effect::Create(m_EfkManager, string16t.c_str());

			// Play an effect
			// エフェクトの再生
			m_Info[i].efkHandle = m_EfkManager->Play(m_Info[i].effect, 0, 0, 0);

			m_Info[i].EfkName = m_apEfkName[type];

			// 位置、向き、大きさ設定
			m_EfkManager->SetLocation(m_Info[i].efkHandle, m_Info[i].pos);
			m_EfkManager->SetRotation(m_Info[i].efkHandle, {0.0f, 1.0f, 0.0f}, m_Info[i].rot.Y);
			m_EfkManager->SetScale(m_Info[i].efkHandle, m_Info[i].scale.X, m_Info[i].scale.Y, m_Info[i].scale.Z);

			break;
		}
	}
}

//===========================================================
// モジュール設定
//===========================================================
void CMyEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
	// Create a  graphics device
	// 描画デバイスの作成
	::Effekseer::Backend::GraphicsDeviceRef graphicsDevice;
	graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(CManager::GetInstance()->GetRenderer()->GetDevice());

	// Create a renderer of effects
	// エフェクトのレンダラーの作成
	m_EfkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// Sprcify rendering modules
	// 描画モジュールの設定
	efkManager->SetSpriteRenderer(m_EfkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(m_EfkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(m_EfkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(m_EfkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(m_EfkRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	efkManager->SetTextureLoader(m_EfkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(m_EfkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(m_EfkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

//===========================================================
// 指定した奴消す
//===========================================================
void CMyEffekseer::Release(int idx)
{
	if (m_Info[idx].effect != nullptr)
	{
		// 値を初期化
		m_Info[idx].pos = {};
		m_Info[idx].rot = {};
		m_Info[idx].scale = {};
		m_Info[idx].effect = nullptr;
		m_Info[idx].efkHandle = NULL;
		m_Info[idx].EfkName = {};
		m_Info[idx].time = 0;
	}
}