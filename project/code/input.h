//===========================================================
//
// ���͏���[input.h]
// Author �匴�叫
//
//===========================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "main.h"
#pragma comment(lib, "xinput.lib")

//===========================================================
// �}�N����`
//===========================================================
#define NUM_KEY_MAX     (256)     //�L�[�̍ő吔

//===========================================================
// �C���v�b�g�N���X��`
//===========================================================
class CInput
{
public:
	CInput();           // �R���X�g���N�^
	virtual ~CInput();  // �f�X�g���N�^

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);  // ����������
	virtual void Uninit(void);                             // �I������
	virtual void Update(void) = 0;                         // �X�V����
	
protected:
	static LPDIRECTINPUT8 m_pInput;     // directInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 m_pDevice;     // ���̓f�o�C�X�i�L�[�{�[�h�j�ւ̃|�C���^
};
 
#endif