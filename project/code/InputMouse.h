//===========================================================
//
// �Q�[���p�b�g�̏���[InputMouse.h]
// Author �匴�叫
//
//===========================================================
#ifndef _INPUTMOUSE_H_
#define _INPUTMOUSE_H_

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "input.h"

//===========================================================
// �L�[�{�[�h�̃N���X��`
//===========================================================
class CInputMouse : public CInput
{
public:
	CInputMouse();   //�R���X�g���N�^
	~CInputMouse();  //�f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);  //����������
	void Uninit(void);                             //�I������
	void Update(void);                             //�X�V����

	bool GetLButton(void);  //���N���b�N
	bool GetRButton(void);  //�E�N���b�N
	bool GetWheel(void);    //�z�C�[��

	D3DXVECTOR2 GetMouseMove(void);  //�ړ��ʎ擾
	float GetMouseWheel(void);       //�}�E�X�z�C�[���̉�]�ʎ擾

private:
	DIMOUSESTATE  m_CurrentMouseState;
	//BYTE rgbButtons[4];
};

#endif