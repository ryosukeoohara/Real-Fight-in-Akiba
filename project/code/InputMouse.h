//===========================================================
//
// ゲームパットの処理[InputMouse.h]
// Author 大原怜将
//
//===========================================================
#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "input.h"

//===========================================================
// キーボードのクラス定義
//===========================================================
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();   // コンストラクタ
	~CInputKeyboard();  // デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);  // 初期化処理
	void Uninit(void);                             // 終了処理
	void Update(void);                             // 更新処理

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];
};

#endif