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
class CInputMouse : public CInput
{
public:
	CInputMouse();   //コンストラクタ
	~CInputMouse();  //デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);  //初期化処理
	void Uninit(void);                             //終了処理
	void Update(void);                             //更新処理

	bool GetLButton(void);  //左クリック
	bool GetRButton(void);  //右クリック
	bool GetWheel(void);    //ホイール

	D3DXVECTOR2 GetMouseMove(void);  //移動量取得
	float GetMouseWheel(void);       //マウスホイールの回転量取得

private:
	DIMOUSESTATE  m_CurrentMouseState;
	//BYTE rgbButtons[4];
};

#endif