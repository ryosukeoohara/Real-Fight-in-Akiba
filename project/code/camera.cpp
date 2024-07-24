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
#include "InputMouse.h"
#include "InputJoyPad.h"
#include "title.h"
#include "game.h"
#include "player.h"
#include "enemymanager.h"
#include "appearanceUI.h"
#include "tutorial.h"
#include "enemy.h"
#include "utility.h"

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const D3DXVECTOR3 HEAT_CAMERAROT = { 0.0f, -D3DX_PI * 0.75f, D3DX_PI * -0.38f };   // 目標の角度
	const D3DXVECTOR3 ONSTAGE_POSV = { 0.0f, 150.0f, -100.0f };   // 目標の角度
	const D3DXVECTOR3 ONSTAGE_POSR = { 0.0f, 50.0f, 0.0f };       // 目標の角度
	const float CAMERA_DISTNCE = 200.0f;  // 距離
	const float CAMERA_MOVE = 2.0f;       // 移動量
	const float CAMERA_ROT_Y = 0.03f;     // Y軸の回転量
}

//================================================================
// コンストラクタ
//================================================================
CCamera::CCamera()
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mode = MODE_NONE;
	m_nCounter = 0;
	m_fLen = 0.0f;

	m_pEnemy = nullptr;
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
		m_posV = D3DXVECTOR3(0.0f, 200.0f, -500.0f);
		m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_posU = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 距離
	m_fLen = CAMERA_DISTNCE;
}

//================================================================
// 終了処理
//================================================================
void CCamera::Uninit(void)
{
	
}

//================================================================
// 更新処理
//================================================================
void CCamera::Update(void)
{
	Mode();
} 

//================================================================
// 設定処理
//================================================================
void CCamera::SetCamera(void)
{
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		100000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_posU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//================================================================
// 向きリセット
//================================================================
void CCamera::Reset(void)
{
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	m_posR = pos;
}

//================================================================
// 視点設定
//================================================================
void CCamera::SetPositionV(D3DXVECTOR3 pos)
{
	m_posV = pos;
}

//================================================================
// 向き設定
//================================================================
void CCamera::SetRotation(D3DXVECTOR3 Rot)
{
	m_rot = Rot;

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
}

//================================================================
// 距離設定
//================================================================
void CCamera::SetDistnce(float fLen)
{
	m_fLen = fLen;
}

//================================================================
// モード取得
//================================================================
CCamera::MODE CCamera::GetMode(void)
{
	return m_mode;
}

//================================================================
// モード
//================================================================
void CCamera::Mode(void)
{
	//シーンの情報を寿徳
	CScene *pScene = CManager::GetInstance()->GetScene();

	switch (m_mode)
	{
	case MODE_NONE:
		break;

	case MODE_TITLE:

		Title();
		break;

	case MODE_TUTORIAL:

		m_OldposR = m_posR;
		m_OldposV = m_posV;
		m_Oldrot = m_rot;
		m_fOldLen = m_fLen;
		CameraV();
		break;

	case MODE_GAME:

		m_OldposR = m_posR;
		m_OldposV = m_posV;
		m_Oldrot = m_rot;
		m_fOldLen = m_fLen;

		CameraV();
		break;

	case MODE_RESULT:

		break;

	case MODE_HEAT:

		Heat();
		break;

	case MODE_RETURN:
		
		Return();
		break;

	case MODE_DEBUG:

		Debug();
		break;

	case MODE_ONSTAGE:

		OnStage();
		break;

	case MODE_TARGET:

		m_OldposR = m_posR;
		m_OldposV = m_posV;
		m_Oldrot = m_rot;
		m_fOldLen = m_fLen;
		CameraV();
		break;

	case MODE_MAX:
		break;

	default:
		break;
	}

	//キーボードを取得
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//ゲームパッドを取得
	CInputJoyPad* pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();

	if ((CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME || CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TUTORIAL)
	 && InputKeyboard->GetTrigger(DIK_R) == true || pInputJoyPad->GetTrigger(CInputJoyPad::BUTTON_RB, 0) == true)
	{
		m_mode = MODE_TARGET;
		Target();
	}

	//向きを設定
	SetRotation(m_rot);

	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	if (pDebugProc == nullptr)
		return;

	pDebugProc->Print("\n---カメラの情報---");
	pDebugProc->Print("\nカメラの位置視点：X軸<%f>,Y軸<%f>,Z軸<%f>", m_posV.x, m_posV.y, m_posV.z);
	pDebugProc->Print("\nカメラの位置注視点：X軸<%f>,Y軸<%f>,Z軸<%f>", m_posR.x, m_posR.y, m_posR.z);
	pDebugProc->Print("\nカメラの向き ：X軸<%f>,Y軸<%f>,Z軸<%f>", m_rot.x, m_rot.y, m_rot.z);
}

//================================================================
// 激アツアクション
//================================================================
void CCamera::Heat(void)
{
	m_posV.x = m_posR.x - sinf(m_rot.y) * -m_fLen;
	m_posV.z = m_posR.z - cosf(m_rot.y) * -m_fLen;

	D3DXVECTOR3 pos = CPlayer::GetInstance()->GetPosition();

	m_posV = D3DXVECTOR3(0.0f + m_posV.x, 150.0f, 30.0f + m_posV.z);
	m_posR = D3DXVECTOR3(pos.x, 50.0f, pos.z + 10.0f);
	m_posU = D3DXVECTOR3(0.0f, 5.0f, 0.0f);

	//目標の注視点を設定
	m_posRDest.x = pos.x;
	m_posRDest.z = pos.z;

	//カメラの移動量
	m_move.x = m_posRDest.x - m_posR.x;
	m_move.z = m_posRDest.z - m_posR.z;

	//位置に移動量を保存
	m_posR.x += m_move.x;
	m_posR.z += m_move.z;
}

//================================================================
// 元の位置に戻ってくる
//================================================================
void CCamera::Return(void)
{
	if (m_nCounter <= 90)
	{
		// カメラを目標の向きまで回転させる
		D3DXVECTOR3 rotDest = m_Oldrot - m_rot;
		SetRotation(m_rot + rotDest * 0.05f);

		D3DXVECTOR3 posDestR = m_OldposR - m_posR;
		SetPositionR(m_posR + posDestR * 0.05f);

		D3DXVECTOR3 posDestV = m_OldposV - m_posV;
		SetPositionV(m_posV + posDestV * 0.05f);

		float fLenDest = m_fOldLen - m_fLen;
		SetDistnce(m_fLen + fLenDest * 0.05f);

		m_nCounter++;
	}
	else
	{
		// カウンターをリセット
		m_nCounter = 0;

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{
			// カメラモードをゲーム
			m_mode = MODE_GAME;

			CGame::GetPlayer()->SetMobile();

			CGame::GetEnemyManager()->SetMobility();
		}
		else if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_TUTORIAL)
		{
			// カメラモードをゲーム
			m_mode = MODE_TUTORIAL;

			CTutorial::GetPlayer()->SetMobile();
		}
	}
}

//================================================================
// ちょーせい
//================================================================
void CCamera::Debug(void)
{
	
}

//================================================================
// 登場
//================================================================
void CCamera::OnStage(void)
{
	if (m_nCounter <= 80)
	{
		D3DXVECTOR3 posDestR = ONSTAGE_POSR - m_posR;
		SetPositionR(m_posR + posDestR * 0.05f);

		D3DXVECTOR3 posDestV = ONSTAGE_POSV - m_posV;
		SetPositionV(m_posV + posDestV * 0.05f);

		m_posV.x = m_posR.x - sinf(m_rot.y) * -m_fLen;
		m_posV.z = m_posR.z - cosf(m_rot.y) * -m_fLen;

		m_nCounter++;
	}
	/*else if (m_nCounter <= 80 && m_nCounter <= 140)
	{
		if (CGame::GetWave() == CGame::WAVE_00)
		{
			CAppearanceUI::Create(CAppearanceUI::TYPE_WEAKNAME);
		}
		else if (CGame::GetWave() == CGame::WAVE_01)
		{
			CAppearanceUI::Create(CAppearanceUI::TYPE_BOSSNAME);
		}

		m_nCounter++;
	}*/
	else
	{
		// カメラモードをゲーム
		m_mode = MODE_RETURN;

		// カウンターをリセット
		m_nCounter = 0;

		if (CGame::GetWave() == CGame::WAVE_00)
		{
			CAppearanceUI::Create(CAppearanceUI::TYPE_WEAKNAME);
		}
		else if (CGame::GetWave() == CGame::WAVE_01)
		{
			CAppearanceUI::Create(CAppearanceUI::TYPE_BOSSNAME);
		}
	}
}

//===========================================================
// 敵をターゲットするカメラ処理
//===========================================================
void CCamera::Target(void)
{
	float fLenght = 10000.0f;
	float fLenghtDiff = 0.0f;  // 比較用

	CEnemy* pEnemy = CEnemy::GetTop();

	// プレイヤーと最も近い敵を探索
	while (pEnemy != nullptr)
	{
		CEnemy* pEnemyNext = pEnemy->GetNext();

		CPlayer* pPlayer = CPlayer::GetInstance();
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

		if (CManager::GetInstance()->GetUtility() != nullptr)
			fLenghtDiff = utility::Distance(PlayerPos, pEnemy->GetPosition());

		// 距離を比較
		if (fLenght >= fLenghtDiff)
		{
			fLenght = fLenghtDiff;

			// 一番近くの敵を覚える
			m_pEnemy = pEnemy;
		}

		pEnemy = pEnemyNext;
	}

	m_posR = m_pEnemy->GetPosition();
	m_bTarget = true;
}

//================================================================
// 視点の移動
//================================================================
void CCamera::CameraV(void)
{
	//キーボードの情報を取得
	CInputKeyboard *InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//マウスの情報を取得
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

	//マウスの位置を取得
	D3DXVECTOR2 MousePos = pInputMouse->GetMouseMove();

	//ゲームパッドを取得
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pInputJoyPad->GetRXStick(CInputJoyPad::STICK_RX, 0) > 0)
	{
		m_rot.y += 0.05f;
	}
	else if (pInputJoyPad->GetRXStick(CInputJoyPad::STICK_RX, 0) < 0)
	{
		m_rot.y -= 0.05f;
	}

	m_rot.y += MousePos.x * 0.005f;

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	m_posV.x = m_posR.x - sinf(m_rot.y) * -m_fLen;
	m_posV.z = m_posR.z - cosf(m_rot.y) * -m_fLen;

	if (m_bTarget == true)
	{
		m_posV = D3DXVECTOR3(0.0f + m_posV.x, 150.0f, 0.0f + m_posV.z);
		m_posU = D3DXVECTOR3(0.0f, 5.0f, 0.0f);

		//カメラの移動量
		m_move.x = m_posRDest.x - m_posR.x;
		m_move.z = m_posRDest.z - m_posR.z;
	}
	else
	{
		D3DXVECTOR3 pos = pPlayer->GetPosition();

		m_posV = D3DXVECTOR3(0.0f + m_posV.x, 150.0f, 0.0f + m_posV.z);
		m_posR = D3DXVECTOR3(pos.x, 75.0f + m_DebugPosR.y, pos.z + m_DebugPosR.z);
		m_posU = D3DXVECTOR3(0.0f, 5.0f, 0.0f);

		//目標の注視点を設定
		m_posRDest.x = pos.x;
		m_posRDest.z = pos.z;

		//カメラの移動量
		m_move.x = m_posRDest.x - m_posR.x;
		m_move.z = m_posRDest.z - m_posR.z;

		//位置に移動量を保存
		m_posR.x += m_move.x;
		m_posR.z += m_move.z;
	}

	if (InputKeyboard->GetPress(DIK_I) == true)
		m_DebugPosR.y += 0.05f;

	if (InputKeyboard->GetPress(DIK_K) == true)
		m_DebugPosR.y -= 0.05f;

	if (InputKeyboard->GetPress(DIK_J) == true)
		m_DebugPosR.z += 0.05f;

	if (InputKeyboard->GetPress(DIK_L) == true)
		m_DebugPosR.z -= 0.05f;
	
}

//================================================================
// チュートリアル
//================================================================
void CCamera::Tutorial(void)
{
	//キーボードの情報を取得
	CInputKeyboard *InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//マウスの情報を取得
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();

	//マウスの位置を取得
	D3DXVECTOR2 MousePos = pInputMouse->GetMouseMove();

	//ゲームパッドを取得
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();

	CPlayer *pPlayer = CTutorial::GetPlayer();

	if (pInputJoyPad->GetRXStick(CInputJoyPad::STICK_RX, 0) > 0)
	{
		m_rot.y += 0.05f;
	}
	else if (pInputJoyPad->GetRXStick(CInputJoyPad::STICK_RX, 0) < 0)
	{
		m_rot.y -= 0.05f;
	}

	m_rot.y += MousePos.x * 0.005f;

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	m_posV.x = m_posR.x - sinf(m_rot.y) * -m_fLen;
	m_posV.z = m_posR.z - cosf(m_rot.y) * -m_fLen;

	D3DXVECTOR3 pos = pPlayer->GetPosition();

	m_posV = D3DXVECTOR3(0.0f + m_posV.x, 150.0f, 30.0f + m_posV.z);
	m_posR = D3DXVECTOR3(pos.x, 50.0f, pos.z + 10.0f);
	m_posU = D3DXVECTOR3(0.0f, 5.0f, 0.0f);

	//目標の注視点を設定
	m_posRDest.x = pos.x;
	m_posRDest.z = pos.z;

	//カメラの移動量
	m_move.x = m_posRDest.x - m_posR.x;
	m_move.z = m_posRDest.z - m_posR.z;

	//位置に移動量を保存
	m_posR.x += m_move.x;
	m_posR.z += m_move.z;
}

//================================================================
// 視点の移動
//================================================================
void CCamera::CameraR(void)
{
	
}

//================================================================
// 視点の移動
//================================================================
void CCamera::Title(void)
{
	/*m_rot.y -= 0.002f;

	m_posV.x = m_posR.x - sinf(m_rot.y) * -300.0f;
	m_posV.z = m_posR.z - cosf(m_rot.y) * -300.0f;

	m_posV = D3DXVECTOR3(m_posV.x, 50.0f, 30.0f + m_posV.z);
	m_posR = D3DXVECTOR3(0.0f, 50.0f, 500.0f);*/

	m_posV.x = m_posR.x - sinf(m_rot.y) * -m_fLen;
	m_posV.z = m_posR.z - cosf(m_rot.y) * -m_fLen;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 pos = pPlayer->GetPosition();

		m_posV = D3DXVECTOR3(100.0f, 50.0f, 100.0f);
		m_posR = D3DXVECTOR3(50.0f, 50.0f, 10.0f);
		m_posU = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
	}
}