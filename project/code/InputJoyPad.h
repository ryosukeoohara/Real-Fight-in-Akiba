//===========================================================
//
// ゲームパットの処理[InputJoyPad.h]
// Author 大原怜将
//
//===========================================================
#ifndef _INPUTJOYPAD_H_
#define _INPUTJOYPAD_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "input.h"

//===========================================================
// ゲームパットのクラス定義
//===========================================================
class CInputJoyPad : public CInput
{
public:
	
	// ボタンの種類
	enum PADBUTTON
	{
		BUTTON_UP = 0,  // 十字キー上
		BUTTON_DOWN,    // 十字キー下
		BOTTON_LEFT,    // 十字キー左
		BUTTON_RIGHT,   // 十字キー右
		BUTTON_START,   // スタート
		BUTTON_BACK,    // 戻る
		BUTTON_LTHUMB,  // 
		BUTTON_RTHUMB,  // 
		BUTTON_LB,      // 左
		BUTTON_RB,      // 右
		BUTTON_LT,      // 左トリガー
		BUTTON_RT,      // 右トリガー
		BUTTON_A,       // A
		BUTTON_B,       // B
		BUTTON_X,       // X
		BUTTON_Y,       // Y
		STICK_LX,       // 左スティックX軸
		STICK_LY,       // 左スティックY軸
		STICK_RX,       // 右スティックX軸
		STICK_RY,       // 右スティックY軸
		BUTTON_MAX
	};

	CInputJoyPad();   //コンストラクタ
	~CInputJoyPad();  //デストラクタ

	HRESULT Init(void);  //初期化処理
	void Uninit(void);  //終了処理
	void Update(void);  //更新処理

	bool GetPress(PADBUTTON nKey, int nCntPlayer);
	bool GetTrigger(PADBUTTON nKey, int nCntPlayer);
	bool GetRelease(PADBUTTON nKey, int nCntPlayer);

	bool GetLeftTirgger(PADBUTTON nKey, int nCntPlayer);
	bool GetRightTirgger(PADBUTTON nKey, int nCntPlayer);

	float GetLXStick(PADBUTTON nKey, int nCntPlayer);
	float GetLYStick(PADBUTTON nKey, int nCntPlayer);

	float GetRXStick(PADBUTTON nKey, int nCntPlayer);
	float GetRYStick(PADBUTTON nKey, int nCntPlayer);

	float GetLYTriggerStick(PADBUTTON nKey, int nCntPlayer);

private:
	XINPUT_STATE m_State;
	XINPUT_STATE m_StateTrigger;
	XINPUT_STATE m_StateRelease;
	XINPUT_STATE m_StateRepeat;
};

#endif