//===========================================================
//
// マウスの処理[InputMouse.cpp]
// Author 大原怜将
//
//===========================================================
#include "InputMouse.h"
#include "debugproc.h"
#include "manager.h"

//===========================================================
// コンストラクタ
//===========================================================
CInputMouse::CInputMouse()
{
	
}

//===========================================================
// デストラクタ
//===========================================================
CInputMouse::~CInputMouse()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	if (m_pInput != NULL)
	{
		// 入力デバイス（マウス）の生成
		if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
		{
			return E_FAIL;
		}

		// データフォーマット
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
		{
			return E_FAIL;
		}

		// 調教モードを設定
		if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			return E_FAIL;
		}
	}

	// アクセス権を獲得
	m_pDevice->Acquire();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CInputMouse::Uninit(void)
{
	// 入力デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//===========================================================
// 更新処理
//===========================================================
void CInputMouse::Update(void)
{
	//デバッグプロックの情報を取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	//マウス座標を取得する
	POINT pos;
	GetCursorPos(&pos);

	//スクリーン座標をクライアント座標に変換する
	ScreenToClient(FindWindowA(CLASS_NAME, nullptr), &pos);

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState)))
	{

	}
	else
	{
		m_pDevice->Acquire();     //マウスアクセス権を獲得
	}

	if (pDebugProc == nullptr)
		return;
	
	pDebugProc->Print("\n---マウスの情報---");
	pDebugProc->Print("\n<<位置 : X:%d Y:%d>>", pos, pos);
	pDebugProc->Print("\n<<[X] : %f, [Y] : %f>>", (float)m_CurrentMouseState.lX, (float)m_CurrentMouseState.lY);
	pDebugProc->Print("\n<<ホイールの回転量 : %f>>", (float)m_CurrentMouseState.lZ);
}

//===========================================================
// 右クリックの情報取得
//===========================================================
bool CInputMouse::GetLButton(void)
{
	return (m_CurrentMouseState.rgbButtons[0] & 0x80) ? true : false;
}

//===========================================================
// 左クリックの情報取得
//===========================================================
bool CInputMouse::GetRButton(void)
{
	return (m_CurrentMouseState.rgbButtons[1] & 0x80) ? true : false;
}

//===========================================================
// ホイール取得
//===========================================================
bool CInputMouse::GetWheel(void)
{
	return (m_CurrentMouseState.rgbButtons[2] & 0x80) ? true : false;
}

//===========================================================
// 移動量の取得
//===========================================================
D3DXVECTOR2 CInputMouse::GetMouseMove(void)
{
	return D3DXVECTOR2((float)m_CurrentMouseState.lX, (float)m_CurrentMouseState.lY);
}

//===========================================================
// ホイールの回転量取得
//===========================================================
float CInputMouse::GetMouseWheel(void)
{
	return (float)m_CurrentMouseState.lZ;
}