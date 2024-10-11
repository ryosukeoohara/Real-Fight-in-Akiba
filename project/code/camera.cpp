//===========================================================
//
// カメラ処理 [camera.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"
#include "InputKeyBoard.h"
#include "InputMouse.h"
#include "InputJoyPad.h"
#include "title.h"
#include "game.h"
#include "player.h"
#include "enemymanager.h"
#include "appearanceUI.h"
#include "tutorial.h"
#include "enemy.h"
#include "motion.h"
#include "enemy_boss.h"
#include "utility.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CCamera* CCamera::m_pCamera = nullptr;

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const D3DXVECTOR3 HEAT_CAMERAROT = { 0.0f, -D3DX_PI * 0.75f, D3DX_PI * -0.38f };   // 目標の角度
	const D3DXVECTOR3 ONSTAGE_POSV = { 0.0f, 150.0f, -100.0f };   // キャラクターが登場演出用カメラの目標の視点の位置
	const D3DXVECTOR3 ONSTAGE_POSR = { 0.0f, 50.0f, 0.0f };       // キャラクターが登場演出用カメラの目標の注視点の位置
	const float CAMERA_DISTNCE = 200.0f;  // 距離
	const float CAMERA_MOVE = 2.0f;       // 移動量
	const float CAMERA_ROT_Y = 0.03f;     // Y軸の回転量
	const float CORRECT_TO_FACT = 0.05f;  // 補正する値
	const int TIME_TO_CORRECT = 90;       // 補正する時間
	const int FOCUS_ENEMY = 120;          // ゲームスタート時に敵を注視する時間
	const int FOCUS_COUNT = 3;            // ゲームスタート時にカメラの位置が変わる回数
	const D3DXVECTOR3 CAMERA_POSV[3] =
	{
		{ -500.0f, 300.0f, 1000.0f},
		{ 300.0f, 30.0f, 120.0f},
		{ 650.0f, 300.0f, -600.0f},
	};

	const D3DXVECTOR3 CAMERA_POS[3] =
	{
		{ -740.0f, 500.0f, 1000.0f},
		{ -53.0f, 30.0f, 120.0f},
		{ 650.0f, 300.0f, -1000.0f},
	};
}

//================================================================
// コンストラクタ
//================================================================
CCamera::CCamera()
{
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mode = MODE_NONE;
	m_Info.nCounter = 0;
	m_Info.fLength = 0.0f;

	m_pEnemy = nullptr;
	m_pCamera = this;
}

//================================================================
// デストラクタ
//================================================================
CCamera::~CCamera()
{

}

//================================================================
// 初期化処理
//================================================================
void CCamera::Init(void)
{
	CScene *pScene = CManager::GetInstance()->GetScene();

	if (pScene->GetMode() == CScene::MODE_TITLE)
	{
		m_Info.posV = D3DXVECTOR3(0.0f, 200.0f, -500.0f);
		m_Info.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Info.posU = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 距離
	m_Info.fLength = CAMERA_DISTNCE;
}

//================================================================
// 終了処理
//================================================================
void CCamera::Uninit(void)
{
	m_pCamera = nullptr;
}

//================================================================
// 更新処理
//================================================================
void CCamera::Update(void)
{
	// ステートの更新
	if (m_pState != nullptr)
		m_pState->Update(this);

	CManager::GetInstance()->GetDebugProc()->Print("\nカメラの注視点の位置：%f,%f,%f\n", m_Info.posR.x, m_Info.posR.y, m_Info.posR.z);
	CManager::GetInstance()->GetDebugProc()->Print("\nカメラの視点の位置  ：%f,%f,%f\n", m_Info.posV.x, m_Info.posV.y, m_Info.posV.z);
	CManager::GetInstance()->GetDebugProc()->Print("\nカメラの向き        ：%f,%f,%f\n", m_Info.rot.x, m_Info.rot.y, m_Info.rot.z);
} 

//================================================================
// 設定処理
//================================================================
void CCamera::SetCamera(void)
{
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixPerspectiveFovLH(&m_Info.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		100000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_Info.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_Info.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_Info.mtxView,
		&m_Info.posV,
		&m_Info.posR,
		&m_Info.posU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_Info.mtxView);
}

//================================================================
// 向きリセット
//================================================================
void CCamera::Reset(void)
{
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//================================================================
// モード設定
//================================================================
void CCamera::SetMode(MODE type)
{
	m_mode = type;
}

//================================================================
// 注視点設定
//================================================================
void CCamera::SetPositionR(D3DXVECTOR3 pos)
{
	m_Info.posR = pos;
}

//================================================================
// 視点設定
//================================================================
void CCamera::SetPositionV(D3DXVECTOR3 pos)
{
	m_Info.posV = pos;
}

//================================================================
// 向き設定
//================================================================
void CCamera::SetRotation(D3DXVECTOR3 Rot)
{
	m_Info.rot = Rot;

	m_Info.rot.y = utility::CorrectAngle(m_Info.rot.y);
}

//================================================================
// 距離設定
//================================================================
void CCamera::SetDistnce(float fLen)
{
	m_Info.fLength = fLen;
}

//================================================================
// ステイトの変更
//================================================================
void CCamera::ChangeState(CCameraState* pBehaviour)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}
		
	m_pState = pBehaviour;
}

//================================================================
// 目標の位置まで補正する処理
//================================================================
void CCamera::AdjustToTarget(D3DXVECTOR3 posR, D3DXVECTOR3 targetPosR, D3DXVECTOR3 posV, D3DXVECTOR3 targetPosV)
{
	D3DXVECTOR3 posDestR = targetPosR - posR;
	m_Info.posR = m_Info.posR + posDestR * 0.005f;

	D3DXVECTOR3 posDestV = targetPosV - posV;
	m_Info.posV = m_Info.posV + posDestV * 0.005f;

	//pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	//pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
}

//================================================================
// モード取得
//================================================================
CCamera::MODE CCamera::GetMode(void)
{
	return m_mode;
}

//=============================================================================
// カメラのステイト
//=============================================================================
//================================================================
// コンストラクタ
//================================================================
CCameraState::CCameraState()
{

}

//================================================================
// デストラクタ
//================================================================
CCameraState::~CCameraState()
{

}

//=============================================================================
// プレイヤーを追従するカメラ
//=============================================================================
//================================================================
// コンストラクタ
//================================================================
FollowPlayerCamera::FollowPlayerCamera()
{

}

//================================================================
// デストラクタ
//================================================================
FollowPlayerCamera::~FollowPlayerCamera()
{

}

//================================================================
// 更新
//================================================================
void FollowPlayerCamera::Update(CCamera* pCamera)
{
	if (pCamera == nullptr)
		return;

	// カメラの情報取得
	CCamera::Info *pCameraInfo = pCamera->GetInfo();

	//マウスの情報を取得
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	//マウスの位置を取得
	D3DXVECTOR2 MousePos = pInputMouse->GetMouseMove();

	//ゲームパッドを取得
	CInputJoyPad* pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	if (pInputJoyPad->GetRXStick(CInputJoyPad::STICK_RX, 0) > 0)
	{
		pCameraInfo->rot.y += 0.05f;
	}
	else if (pInputJoyPad->GetRXStick(CInputJoyPad::STICK_RX, 0) < 0)
	{
		pCameraInfo->rot.y -= 0.05f;
	}

	if (pPlayer->GetMobility() == CPlayer::Mobile)
		pCameraInfo->rot.y += MousePos.x * 0.005f;

	// 角度の値を修正する
	pCameraInfo->rot.y = utility::CorrectAngle(pCameraInfo->rot.y);

	pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

	D3DXVECTOR3 pos = pPlayer->GetPosition();

	pCameraInfo->posV = D3DXVECTOR3(0.0f + pCameraInfo->posV.x, 150.0f, 0.0f + pCameraInfo->posV.z);
	pCameraInfo->posR = D3DXVECTOR3(pos.x, 75.0f, pos.z);

	//目標の注視点を設定
	pCameraInfo->posRDest.x = pos.x;
	pCameraInfo->posRDest.z = pos.z;

	//カメラの移動量
	pCameraInfo->move.x = pCameraInfo->posRDest.x - pCameraInfo->posR.x;
	pCameraInfo->move.z = pCameraInfo->posRDest.z - pCameraInfo->posR.z;

	//位置に移動量を保存
	pCameraInfo->posR.x += pCameraInfo->move.x;
	pCameraInfo->posR.z += pCameraInfo->move.z;

	pCameraInfo->OldposR = pCameraInfo->posR;
	pCameraInfo->OldposV = pCameraInfo->posV;
	pCameraInfo->Oldrot = pCameraInfo->rot;
	pCameraInfo->fOldLength = pCameraInfo->fLength;
}

//=============================================================================
// 敵を追従するカメラ
//=============================================================================
//================================================================
// コンストラクタ
//================================================================
FollowEnemyCamera::FollowEnemyCamera()
{
	m_move = { 5.0f, 0.0f, 5.0f };
}

//================================================================
// デストラクタ
//================================================================
FollowEnemyCamera::~FollowEnemyCamera()
{

}

//================================================================
// 更新
//================================================================
void FollowEnemyCamera::Update(CCamera* pCamera)
{
	CEnemy* pEnemy = CEnemy::GetTop();

	// カメラの情報取得
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	if (pEnemy != nullptr)
	{
		D3DXVECTOR3 rot = pEnemy->GetRotition();

		pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
		pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

		D3DXVECTOR3 pos = pEnemy->GetPosition();

		pCameraInfo->posV = D3DXVECTOR3(0.0f + pCameraInfo->posV.x, 150.0f, 0.0f + pCameraInfo->posV.z);
		pCameraInfo->posR = D3DXVECTOR3(pos.x, 75.0f, pos.z);

		//目標の注視点を設定
		pCameraInfo->posRDest.x = pos.x;
		pCameraInfo->posRDest.z = pos.z;

		//カメラの移動量
		pCameraInfo->move.x = pCameraInfo->posRDest.x - pCameraInfo->posR.x;
		pCameraInfo->move.z = pCameraInfo->posRDest.z - pCameraInfo->posR.z;

		//位置に移動量を保存
		pCameraInfo->posR.x += pCameraInfo->move.x;
		pCameraInfo->posR.z += pCameraInfo->move.z;

		pCameraInfo->OldposR = pCameraInfo->posR;
		pCameraInfo->OldposV = pCameraInfo->posV;
		pCameraInfo->Oldrot = pCameraInfo->rot;
		pCameraInfo->fOldLength = pCameraInfo->fLength;

		pos += m_move;

		pEnemy->SetPosition(pos);

		if (pos.z > 0.0f)
		{
			pEnemy->GetMotion()->Set(CEnemyBoss::MOTION_KUGIKIRI);
			pCamera->ChangeState(new CutSceneCamera);
		}
	}
}

//=============================================================================
// ゲームスタート時に敵を様々な角度から見るカメラ
//=============================================================================
//================================================================
// コンストラクタ
//================================================================
CameraEnemyOverview::CameraEnemyOverview()
{
	// 敵の先頭を取得
	m_pEnemy = CEnemy::GetTop();

	// カメラのインスタンスを取得
	CCamera* pCamera = CCamera::GetInstance();

	// カメラの情報取得
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	// 敵の位置を取得
	D3DXVECTOR3 posEnemy = m_pEnemy->GetPosition();

	pCameraInfo->posR = D3DXVECTOR3(posEnemy.x, posEnemy.y + 50.0f, posEnemy.z);
}

//================================================================
// 更新処理
//================================================================
void CameraEnemyOverview::Update(CCamera* pCamera)
{
	// どちらかが使用されていないとき処理を抜ける
	if (pCamera == nullptr || m_pEnemy == nullptr)
		return;

	// カメラの情報取得
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	pCamera->AdjustToTarget(pCameraInfo->posR, pCameraInfo->posR, pCameraInfo->posV, CAMERA_POSV[m_nLookCount]);
	//// 注視点と視点を設定
	//pCameraInfo->posR = D3DXVECTOR3(posEnemy.x, posEnemy.y + 50.0f, posEnemy.z);
	//pCameraInfo->posV = CAMERA_POS[m_nLookCount];

	// カウントアップ
	m_nFocusCounter++;

	// 規定の時間がすぎた
	if (m_nFocusCounter >= FOCUS_ENEMY)
	{
		m_pEnemy = m_pEnemy->GetNext();

		//m_pEnemy = pNext;
		m_nFocusCounter = 0;
		m_nLookCount++;

		if (m_pEnemy != nullptr)
		{
			// 敵の位置を取得
			D3DXVECTOR3 posEnemy = m_pEnemy->GetPosition();

			pCameraInfo->posR = D3DXVECTOR3(posEnemy.x, posEnemy.y + 50.0f, posEnemy.z);
			pCameraInfo->posV = CAMERA_POS[m_nLookCount];
		}
	}

	// 3回敵を注視したら、カメラのステートを変更する
	if (m_nLookCount >= FOCUS_COUNT)
		pCamera->ChangeState(new CutSceneCamera);

}

//=============================================================================
// 引いてくカメラ
//=============================================================================
//================================================================
// コンストラクタ
//================================================================
CameraZoomOut::CameraZoomOut()
{

}

//================================================================
// 更新
//================================================================
void FixedCamera::Update(CCamera* pCamera)
{
	// カメラの情報取得
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

	pCameraInfo->posV = D3DXVECTOR3(100.0f, 50.0f, 100.0f);

}

//=============================================================================
// 位置が固定のカメラ
//=============================================================================
//================================================================
// コンストラクタ
//================================================================
FixedCamera::FixedCamera()
{

}

//================================================================
// デストラクタ
//================================================================
FixedCamera::~FixedCamera()
{

}

//================================================================
// 更新
//================================================================
void FixedCamera::Update(CCamera* pCamera)
{
	// カメラの情報取得
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

	pCameraInfo->posV = D3DXVECTOR3(100.0f, 50.0f, 100.0f);
	pCameraInfo->posR = D3DXVECTOR3(50.0f, 50.0f, 10.0f);
}

//=============================================================================
// キャラクターが登場するときのカメラ
//=============================================================================
//================================================================
// コンストラクタ
//================================================================
CutSceneCamera::CutSceneCamera()
{

}

//================================================================
// デストラクタ
//================================================================
CutSceneCamera::~CutSceneCamera()
{

}

//================================================================
// 更新
//================================================================
void CutSceneCamera::Update(CCamera* pCamera)
{
	// カメラの情報取得
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	if (pCameraInfo->nCounter <= TIME_TO_CORRECT)
	{
		D3DXVECTOR3 posDestR = ONSTAGE_POSR - pCameraInfo->posR;
		pCamera->SetPositionR(pCameraInfo->posR + posDestR * CORRECT_TO_FACT);

		D3DXVECTOR3 posDestV = ONSTAGE_POSV - pCameraInfo->posV;
		pCamera->SetPositionV(pCameraInfo->posV + posDestV * CORRECT_TO_FACT);

		pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
		pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

		pCameraInfo->nCounter++;
	}
	else
	{
		// カウンターをリセット
		pCameraInfo->nCounter = 0;

		if (CGame::GetWave() == CGame::WAVE_00)
		{
			CAppearanceUI::Create(CAppearanceUI::TYPE_WEAKNAME);
		}
		else if (CGame::GetWave() == CGame::WAVE_01)
		{
			CAppearanceUI::Create(CAppearanceUI::TYPE_BOSSNAME);
		}

		// カメラモードを変更
		pCamera->ChangeState(new ReturnPlayerBehindCamera);
	}
}

//=============================================================================
// プレイヤーの背後に戻ってくるカメラ
//=============================================================================
//================================================================
// コンストラクタ
//================================================================
ReturnPlayerBehindCamera::ReturnPlayerBehindCamera()
{

}

//================================================================
// デストラクタ
//================================================================
ReturnPlayerBehindCamera::~ReturnPlayerBehindCamera()
{

}

//================================================================
// 更新
//================================================================
void ReturnPlayerBehindCamera::Update(CCamera* pCamera)
{
	// カメラの情報取得
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	if (pCameraInfo->nCounter <= TIME_TO_CORRECT)
	{// カウンターが一定になるまで

		// カメラを目標の向きまで補正する
		D3DXVECTOR3 rotDest = pCameraInfo->Oldrot - pCameraInfo->rot;
		pCamera->SetRotation(pCameraInfo->rot + rotDest * CORRECT_TO_FACT);

		// カメラを目標の位置まで補正する
		D3DXVECTOR3 posDestR = pCameraInfo->OldposR - pCameraInfo->posR;
		pCamera->SetPositionR(pCameraInfo->posR + posDestR * CORRECT_TO_FACT);

		D3DXVECTOR3 posDestV = pCameraInfo->OldposV - pCameraInfo->posV;
		pCamera->SetPositionV(pCameraInfo->posV + posDestV * CORRECT_TO_FACT);

		// カメラを目標の距離まで補正する
		float fLenDest = pCameraInfo->fOldLength - pCameraInfo->fLength;
		pCamera->SetDistnce(pCameraInfo->fLength + fLenDest * CORRECT_TO_FACT);

		// カウンター加算
		pCameraInfo->nCounter++;
	}
	else
	{
		// カウンターをリセット
		pCameraInfo->nCounter = 0;

		// カメラモードを変更
		pCamera->ChangeState(new FollowPlayerCamera);

		// プレイヤーを行動可能にする
		CPlayer::GetInstance()->SetMobile();

		if (CEnemyManager::GetInstance() == nullptr)
			return;

		// ターゲット以外の敵の描画を再開
		CEnemyManager::GetInstance()->RestartDrawing();

		// 敵を行動可能にする
		CEnemyManager::GetInstance()->SetMobility();
	}
}

//=============================================================================
// ヒートアクションのカメラ
//=============================================================================
//================================================================
// コンストラクタ
//================================================================
HeatActionCamera::HeatActionCamera()
{

}

//================================================================
// デストラクタ
//================================================================
HeatActionCamera::~HeatActionCamera()
{

}

//================================================================
// 更新
//================================================================
void HeatActionCamera::Update(CCamera* pCamera)
{
	// カメラの情報取得
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

	if (CPlayer::GetInstance() == nullptr)
		return;

	D3DXVECTOR3 pos = CPlayer::GetInstance()->GetPosition();

	pCameraInfo->posV = D3DXVECTOR3(pCameraInfo->posV.x, 150.0f, 30.0f + pCameraInfo->posV.z);
	pCameraInfo->posR = D3DXVECTOR3(pos.x, 50.0f, pos.z + 10.0f);

	//目標の注視点を設定
	pCameraInfo->posRDest.x = pos.x;
	pCameraInfo->posRDest.z = pos.z;

	//カメラの移動量
	pCameraInfo->move.x = pCameraInfo->posRDest.x - pCameraInfo->posR.x;
	pCameraInfo->move.z = pCameraInfo->posRDest.z - pCameraInfo->posR.z;

	//位置に移動量を保存
	pCameraInfo->posR.x += pCameraInfo->move.x;
	pCameraInfo->posR.z += pCameraInfo->move.z;
}

//=============================================================================
// エディット用のカメラ
//=============================================================================
//================================================================
// コンストラクタ
//================================================================
EditCamera::EditCamera()
{

}

//================================================================
// デストラクタ
//================================================================
EditCamera::~EditCamera()
{

}

//================================================================
// 更新
//================================================================
void EditCamera::Update(CCamera* pCamera)
{
	CInputKeyboard *pInput = CManager::GetInstance()->GetKeyBoard();

	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	D3DXVECTOR2 MousePos = pInputMouse->GetMouseMove();

	// カメラの情報取得
	CCamera::Info* pCameraInfo = pCamera->GetInfo();

	if (pInputMouse->GetWheel() == true)
	{
		pCameraInfo->posV.x += MousePos.x;
		pCameraInfo->posR.x += MousePos.x;
		pCameraInfo->posV.z -= MousePos.y;
		pCameraInfo->posR.z -= MousePos.y;
	}

	if (pInputMouse->GetLButton() == true)
	{
		pCameraInfo->rot.x += MousePos.y * 0.005f;
		pCameraInfo->rot.y += MousePos.x * 0.005f;
		//pCameraInfo->rot.y += MousePos.x * 0.005f;
	}
		

	if (pInput->GetPress(DIK_J) == true)
	{
		pCameraInfo->posV.y += 1.0f;
		pCameraInfo->posR.y += 1.0f;
	}

	if (pInput->GetPress(DIK_K) == true)
	{
		pCameraInfo->posV.y -= 1.0f;
		pCameraInfo->posR.y -= 1.0f;
	}

	pCameraInfo->posV.x = pCameraInfo->posR.x - sinf(pCameraInfo->rot.y) * -pCameraInfo->fLength;
	pCameraInfo->posV.z = pCameraInfo->posR.z - cosf(pCameraInfo->rot.y) * -pCameraInfo->fLength;

}