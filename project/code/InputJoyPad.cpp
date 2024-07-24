//===========================================================
//
// ゲームパットの処理[InputJoyPad.cpp]
// Author 大原怜将
//
//===========================================================
#include "InputJoyPad.h"

//===========================================================
// コンストラクタ
//===========================================================
CInputJoyPad::CInputJoyPad()
{
	ZeroMemory(&m_State, sizeof(m_State));
	ZeroMemory(&m_StateRelease, sizeof(m_StateRelease));
	ZeroMemory(&m_StateTrigger, sizeof(m_StateTrigger));
	ZeroMemory(&m_StateRepeat, sizeof(m_StateRepeat));
}

//===========================================================
// デストラクタ
//===========================================================
CInputJoyPad::~CInputJoyPad()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CInputJoyPad::Init(void)
{
	XInputEnable(true);

	memset(&m_State, 0, sizeof(XINPUT_STATE));
	memset(&m_StateTrigger, 0, sizeof(XINPUT_STATE));
	memset(&m_StateRelease, 0, sizeof(XINPUT_STATE));
	memset(&m_StateRepeat, 0, sizeof(XINPUT_STATE));

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CInputJoyPad::Uninit(void)
{
	XInputEnable(false);
}

//===========================================================
// 更新処理
//===========================================================
void CInputJoyPad::Update(void)
{
	XINPUT_STATE aState;

	if (XInputGetState(0, &aState) == ERROR_SUCCESS)
	{
		// トリガー
		m_StateTrigger.Gamepad.wButtons =
			(m_State.Gamepad.wButtons ^ aState.Gamepad.wButtons)
			& aState.Gamepad.wButtons;

		// リリース
		m_StateRelease.Gamepad.wButtons =
			(m_State.Gamepad.wButtons | aState.Gamepad.wButtons)
			^ aState.Gamepad.wButtons;

		// プレス
		m_State = aState;
	}
	else
	{
		return;
	}

}

//===========================================================
// プレス取得処理
//===========================================================
bool CInputJoyPad::GetPress(PADBUTTON nKey, int nCntPlayer)
{
	return (m_State.Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//===========================================================
// トリガー取得処理
//===========================================================
bool CInputJoyPad::GetTrigger(PADBUTTON nKey, int nCntPlayer)
{
	return (m_StateTrigger.Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//===========================================================
// リリース取得処理
//===========================================================
bool CInputJoyPad::GetRelease(PADBUTTON nKey, int nCntPlayer)
{
	return (m_StateRelease.Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//===========================================================
// 左トリガーボタン取得処理
//===========================================================
bool CInputJoyPad::GetLeftTirgger(PADBUTTON nKey, int nCntPlayer)
{
	return (m_StateTrigger.Gamepad.bLeftTrigger & (0x01 << nKey)) ? true : false;
}

//===========================================================
// 右トリガーボタン取得処理
//===========================================================
bool CInputJoyPad::GetRightTirgger(PADBUTTON nKey, int nCntPlayer)
{
	return (m_StateTrigger.Gamepad.bRightTrigger & (0x01 << nKey)) ? true : false;
}

//===========================================================
// 左スティックのX軸取得処理
//===========================================================
float CInputJoyPad::GetLXStick(PADBUTTON nKey, int nCntPlayer)
{
	if (m_State.Gamepad.sThumbLX > (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 3))
	{
		return m_State.Gamepad.sThumbLX;
	}

	if (m_State.Gamepad.sThumbLX < -(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 3))
	{
		return m_State.Gamepad.sThumbLX;
	}

	return 0.0f;
}

//===========================================================
// 左スティックのY軸取得処理
//===========================================================
float CInputJoyPad::GetLYStick(PADBUTTON nKey, int nCntPlayer)
{
	if (m_State.Gamepad.sThumbLY > (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 3))
	{
		return m_State.Gamepad.sThumbLY;
	}

	if (m_State.Gamepad.sThumbLY < -(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 3))
	{
		return m_State.Gamepad.sThumbLY;
	}

	return 0.0f;
}

//===========================================================
// 右スティックのX軸取得処理
//===========================================================
float CInputJoyPad::GetRXStick(PADBUTTON nKey, int nCntPlayer)
{
	if (m_State.Gamepad.sThumbRX > (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 3))
	{
		return m_State.Gamepad.sThumbRX;
	}

	if (m_State.Gamepad.sThumbRX < -(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 3))
	{
		return m_State.Gamepad.sThumbRX;
	}

	return 0.0f;
}

//===========================================================
// 右スティックのY軸取得処理
//===========================================================
float CInputJoyPad::GetRYStick(PADBUTTON nKey, int nCntPlayer)
{
	if (m_State.Gamepad.sThumbRY > (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 3))
	{
		return m_State.Gamepad.sThumbRY;
	}

	if (m_State.Gamepad.sThumbRY < -(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 3))
	{
		return m_State.Gamepad.sThumbRY;
	}

	return 0.0f;
}