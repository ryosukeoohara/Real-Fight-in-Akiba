//===========================================================
//
// ���f������[objectX.h]
// Author �匴�叫
//
//===========================================================
#ifndef _OBJECTX_H__             //���̃}�N����`������ĂȂ�������
#define _OBJECTX_H__             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "object.h"

//===========================================================
// �O���錾
//===========================================================
class CModel;

//===========================================================
//�@�I�u�W�F�N�gX(X�t�@�C��)�N���X��`
//===========================================================
class CObjectX : public CObject
{
private:
	struct INFO
	{
		D3DXVECTOR3 pos;              // �ʒu
		D3DXVECTOR3 rot;              // ����
		D3DXVECTOR3 move;             // �ړ�
		D3DXCOLOR col;                // �F
		D3DXVECTOR3 vtxMini;          // �ŏ��l
		D3DXVECTOR3 vtxMax;           // �ő�l
		D3DXMATRIX mtxWorld;          // ���[���h�}�g���b�N�X
		int nIdxModel;	              // ���f���̃C���f�b�N�X�ԍ�
		const char *Fliename;         // ���f���̖��O
	};

	INFO m_Info;                // ���

public:
	CObjectX();
	CObjectX(const char *aModelFliename, int nPriority = 3);
	~CObjectX();

	HRESULT Init(void);           // ����������    
	void Uninit(void);            // �I������
	void Update(void);            // �X�V����
	void Draw(void);              // �`�揈��
						          
	void ReadText(void);          //�e�L�X�g�t�@�C���ǂݍ���

	static CObjectX *Create(const char *aModelFliename, int nPriority = 3);  //����

	void BindModel(INFO pObjX);

	void SetVtx(void);

	// �ݒ�n
	void SetPosition(D3DXVECTOR3 pos) { m_Info.pos = pos; }                     // �ʒu
	void SetPositionOri(D3DXVECTOR3 posOrigin) { m_posOrigin = posOrigin; }     // ���_�̈ʒu
	void SetRotition(D3DXVECTOR3 rot) { m_Info.rot = rot; }                     // ����
	void SetRotOrigin(D3DXVECTOR3 rotOrigin) { m_rotOrigin = rotOrigin; }       // ����
	void SetColor(D3DXCOLOR col) { m_Info.col = col; }                          // �F
	void SetIdxModel(int nIdx) { m_Info.nIdxModel = nIdx; }                     // ���f���̃C���f�b�N�X�ԍ�
	void SetCurrent(D3DXMATRIX *Current) { m_pCurrent = Current; }              // �e�̃}�g���b�N�X
	void SetbEnable(bool bValue = true) { m_bEnable = bValue; }
	void SetbDraw(bool bValue) { m_bDraw = bValue; }
	void SetbShut(bool bValue) { m_bShut = bValue; }
	void SetbDown(bool bValue) { m_bDown = bValue; }
	void SetbHitAttack(bool bValue) { m_bHitAttack = bValue; }
	void SetbFall(bool bValue) { m_bFall = bValue; }
	void SetFallCounter(int n) { m_nFallDownCounter = n; }

	//�@�擾�n
	D3DXVECTOR3 GetPosition(void) { return  m_Info.pos; }        // �ʒu
	D3DXVECTOR3 GetPositionOri(void) { return m_posOrigin; }     // ���_�̈ʒu  
	D3DXVECTOR3 GetRotition(void) { return  m_Info.rot; }        // ����
	D3DXVECTOR3 GetRotOrigin(void) { return m_rotOrigin; }       // ���_�̌���
	D3DXCOLOR GetColor(void) { return  m_Info.col; }             // �F
	D3DXMATRIX *GetMtxWorld(void) { return &m_Info.mtxWorld; }   // �}�g���b�N�X�擾
	D3DXVECTOR3 GetVtxMin(void) { return m_Info.vtxMini; }       // ���f���̍ŏ��l
	D3DXVECTOR3 GetVtxMax(void) { return m_Info.vtxMax; }        // ���f���̍ő�l
	int GetIdxModel(void) { return m_Info.nIdxModel; }           // ���f���̃C���f�b�N�X�ԍ�
	const char *GetFileName(void) { return m_Info.Fliename; }    // ���f���̃t�@�C���l�[��
	bool IsEnable(void) { return m_bEnable; }
	int GetFallCounter(void) { return m_nFallDownCounter; }
	bool GetFallDown(void) { return m_bFallDown; }

private:
	
	void Shoot(void);
	void FallDown(void);
	void HitAttack(void);
	void GraduallyFallDown(void);

	D3DXVECTOR3 m_posOrigin;
	D3DXVECTOR3 m_rotOrigin;
	LPD3DXMESH m_pMesh;       //�e�N�X�`���ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;  //���_�o�b�t�@�ւ̃|�C���^
	DWORD m_dwNumMat;         //�}�e���A���̐�
	LPDIRECT3DTEXTURE9 *m_pTexture;     //�e�N�X�`���ւ̃|�C���^
	D3DXMATRIX *m_pCurrent;                     // �e�̃}�g���b�N�X
	D3DMATERIAL9 m_ShadowMat;
	float m_fFallDownSpeed = 0.0f;
	int m_nShakeTimeCounter = 0;
	int m_nShake = 0;
	int m_nFallDownCounter = 0;
	float ShakeAngle = 0.0f;
	float m_fAngle = 1.0f;

	int *m_nIdxTexture;
	bool m_bEnable;
	bool m_bDraw;
	bool m_bShut = false; 
	bool m_bDown = false;
	bool m_bHitAttack = false;
	bool m_bFall = false;
	bool m_bFallDown = false;
};

#endif