//=============================================================================
//
// �ۉe���� [shadow.h]
// Author : �匴�@�叫
//
//=============================================================================
#ifndef _SHADOW_H_             //���̃}�N����`������ĂȂ�������
#define _SHADOW_H_             //2�d�C���N���[�h�h�~�̃}�N����`������

#include "object3D.h"

class CShadow : public CObject3D
{
public:
	CShadow();
	~CShadow();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:



};

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int type;
	bool bUse;
} SHADOW;

typedef enum
{
	SHADOWTYPE_PLAYER = 0,
	SHADOWTYPE_BULLET,
	SHADOWTYPE_MAX
}SHADOWTYPE;

//�v���g�^�C�v�錾
void InitShadow(void);          //�|���S���̏���������    
void UninitShadow(void);        //�|���S���̏I������
void UpdateShadow(void);        //�|���S���̍X�V����
void DrawShadow(void);          //�|���S���̕`�揈��
SHADOW *GetShadow(void);

int SetShadow(void);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
//void SetPositionShadow1(int nIdxShadow1, D3DXVECTOR3 pos);

#endif
