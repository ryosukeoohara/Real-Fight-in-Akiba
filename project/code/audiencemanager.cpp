//===========================================================
//
// 周りで見ている人の管理[audiencemanager.cpp]
// Author 大原怜将
//
//===========================================================
#include "audiencemanager.h"
#include "audience.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const int FRONT_AUDIENCE = 50;  // 正面の見ている人の数
	const int BACK_AUDIENCE = 45;   // 背面の見ている人の数
	const int LIFT_AUDIENCE = 20;   // 左の見ている人の数

	const char* MODELNAME[CAudience::TYPE_MAX] =
	{
		"data\\MODEL\\audience\\crowd_02.x",
		"data\\MODEL\\audience\\crowd_02.x",
		"data\\MODEL\\audience\\crowd_03.x",
		"data\\MODEL\\audience\\crowd_04.x",
	};  // モデルのファイルネーム
}

//================================================================
// コンストラクタ
//================================================================
CAudienceManager::CAudienceManager()
{

}

//================================================================
// デストラクタ
//================================================================
CAudienceManager::~CAudienceManager()
{

}

//================================================================
// 初期化処理
//================================================================
HRESULT CAudienceManager::Init(void)
{
	// 正面
	for (int i = 0; i < FRONT_AUDIENCE; i++)
	{
		int nModelFileIdx = rand() % TYPE_MAX;
		float fPosX = rand() % 21;
		float fPosZ = rand() % 101;
		
		CAudience* pAudience = CAudience::Create(D3DXVECTOR3((800.0f - i * 35.0f) + fPosX, 0.0f, -800.0f + fPosZ), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), MODELNAME[nModelFileIdx]);
	}

	// 背面
	for (int i = 0; i < BACK_AUDIENCE; i++)
	{
		int nModelFileIdx = rand() % TYPE_MAX;
		float fPosX = rand() % 21;
		float fPosZ = rand() % 101;

		CAudience* pAudience = CAudience::Create(D3DXVECTOR3((-800.0f + i * 35.0f) + fPosX, 0.0f, 1050.0f + fPosZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODELNAME[nModelFileIdx]);
	}

	// 左
	for (int i = 0; i < LIFT_AUDIENCE; i++)
	{
		int nModelFileIdx = rand() % TYPE_MAX;
		float fPosX = rand() % 21;
		float fPosZ = rand() % 101;

		CAudience* pAudience = CAudience::Create(D3DXVECTOR3(850.0f + fPosX, 0.0f, (900.0f - i * 35.0f) + fPosZ), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), MODELNAME[nModelFileIdx]);
	}

	return S_OK;
}

//================================================================
// 終了処理
//================================================================
void CAudienceManager::Uninit(void)
{

}

//================================================================
// 更新処理
//================================================================
void CAudienceManager::Update(void)
{

}

//================================================================
// 生成処理
//================================================================
CAudienceManager* CAudienceManager::Create(void)
{
	CAudienceManager* pAudienceManager = nullptr;
	pAudienceManager = new CAudienceManager;

	if (pAudienceManager != nullptr)
	{
		pAudienceManager->Init();
	}

	return pAudienceManager;
}
