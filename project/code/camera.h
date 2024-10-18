//===========================================================
//
// �J�������� [camera.h]
// Author : �匴�@�叫
//
//===========================================================
#ifndef _CAMERA_H_             //���̃}�N����`������ĂȂ�������
#define _CAMERA_H_             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �O���錾
//===========================================================
class CEnemy;
class CCameraState;

//===========================================================
// �J�����N���X�̒�`
//===========================================================
class CCamera
{
public:
	CCamera();
	~CCamera();

	struct Info
	{
		D3DXMATRIX mtxView;        // �r���[�}�g���b�N�X
		D3DXMATRIX mtxProjection;  // �v���W�F�N�V�����}�g���b�N�X
		D3DXVECTOR3 posV;          // ���_
		D3DXVECTOR3 OldposV;       // �O��̎��_�̈ʒu
		D3DXVECTOR3 posR;          // �����_
		D3DXVECTOR3 OldposR;       // �O��̒����_�̈ʒu
		D3DXVECTOR3 posU;          // ������x�N�g��
		D3DXVECTOR3 posVDest;      // �ړI�̎��_
		D3DXVECTOR3 posRDest;      // �ړI�̒����_
		D3DXVECTOR3 rot;           // ����
		D3DXVECTOR3 Oldrot;        // �O��̌��� 
		D3DXVECTOR3 move;          // �ړ�

		int nCounter;              // �J�������؂�ւ���Ă��鎞��
		float fLength;             // ����
		float fOldLength;          // �O��̋���
	};

	enum MODE
	{
		MODE_NONE = 0,
		MODE_TITLE,      // �^�C�g���̃J����
		MODE_TUTORIAL,   // �`���[�g���A��
		MODE_GAME,       // �Q�[���̃J����
		MODE_RESULT,     // ���U���g�̃J����
		MODE_HEAT,       // �q�[�g�A�N�V����
		MODE_RETURN,     // ���̈ʒu�ɖ߂�
		MODE_DEBUG,      // ����[����
		MODE_ONSTAGE,    // �o��
		MODE_TARGET,     // �G���^�[�Q�b�g����J����
		MODE_MAX 
	};

	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_HEAT,
		TYPE_MAX
	};

	void Init(void);          //�J�����̏���������    
	void Uninit(void);        //�J�����̏I������
	void Update(void);        //�J�����̍X�V����
	void CameraV(void);
	void Tutorial(void);
	void Edit(void);          //�G�f�B�b�g�̃J����
	void CameraR(void);
	void Title(void);
	void Scope(void);
	void SetCamera(void);
	void Reset(void);

	void Boss(void);

	//void GetInstance(void);

	// �ݒ�n
	void SetMode(MODE type);
	void SetPositionR(D3DXVECTOR3 pos);
	void SetPositionV(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 Rot);
	void SetDistnce(float fLen);
	void ChangeState(CCameraState* pBehaviour);
	void AdjustToTarget(D3DXVECTOR3 targetPosR, D3DXVECTOR3 targetPosV, float fCorrent);
	void Follow(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot);

	// �擾�n
	MODE GetMode(void);
	D3DXVECTOR3 GetRotation(void) { return m_Info.rot; }
	D3DXMATRIX GetView(void) { return m_Info.mtxView; }
	D3DXMATRIX GetProjection(void) { return m_Info.mtxProjection; }
	Info *GetInfo(void) { return &m_Info; }
	static CCamera* GetInstance(void) { return m_pCamera; }

private:
	void Mode(void);
	void Heat(void);
	void Return(void);
	void Debug(void);
	void OnStage(void);
	void Target(void);

	Info m_Info;
	MODE m_mode;

	bool m_bTarget = false;
	CCameraState*m_pState = nullptr;
	CEnemy* m_pEnemy;
	static CCamera* m_pCamera;

	// �f�o�b�O�p
	D3DXVECTOR3 m_DebugPosR = {};

};

//===========================================================
// �J�����̃X�e�C�g
//===========================================================
class CCameraState
{
public:
	CCameraState();
	~CCameraState();

	virtual void Update(CCamera* pCamera) = 0;

private:

};

//===========================================================
// �Ǐ]����J����
//===========================================================
class FollowPlayerCamera : public CCameraState
{
public:
	FollowPlayerCamera();
	~FollowPlayerCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// �Q�[���X�^�[�g���ɓG��l�X�Ȋp�x���猩��J����
//===========================================================
class CameraEnemyOverview : public CCameraState
{
public:
	CameraEnemyOverview();
	~CameraEnemyOverview() {}

	void Update(CCamera* pCamera) override;

private:

	CEnemy* m_pEnemy = nullptr;

	int m_nFocusCounter = 0;
	int m_nLookCount = 0;

};

//===========================================================
// �G��Ǐ]����J����
//===========================================================
class FollowEnemyCamera : public CCameraState
{
public:
	FollowEnemyCamera();
	~FollowEnemyCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// �G��Ǐ]����J����
//===========================================================
class FollowEnemyOverviewCamera : public CCameraState
{
public:
	FollowEnemyOverviewCamera();
	~FollowEnemyOverviewCamera() {}

	void Update(CCamera* pCamera) override;

private:

	CEnemy* m_pEnemy = nullptr;
	
	int m_nFocusCounter = 0;
	int m_nLookCount = 0;
};

//===========================================================
// �����Ă����J����
//===========================================================
class CameraZoomOut : public CCameraState
{
public:
	CameraZoomOut();
	~CameraZoomOut() {}

	void Update(CCamera* pCamera) override;

private:

	D3DXVECTOR3 TargetPosV = {};
	int n = 0;
};

//===========================================================
// �ڕW�̈ʒu�������J����
//===========================================================
class CameraTargetFocus : public CCameraState
{
public:
	CameraTargetFocus();
	~CameraTargetFocus() {}

	void Update(CCamera* pCamera) override;

private:

	int n = 0;
	D3DXVECTOR3 TargetPosV = {};
};

//===========================================================
// �Œ�J����
//===========================================================
class FixedCamera : public CCameraState
{
public:
	FixedCamera();
	~FixedCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// �L�����N�^�[�o�ꉉ�o�p�J����
//===========================================================
class CutSceneCamera : public CCameraState
{
public:
	CutSceneCamera();
	~CutSceneCamera();

	void Update(CCamera* pCamera) override;

private:

	D3DXVECTOR3 TargetPosV = {};
};

//===========================================================
// �L�����N�^�[�o�ꉉ�o�p�J����
//===========================================================
class ReturnPlayerBehindCamera : public CCameraState
{
public:
	ReturnPlayerBehindCamera();
	~ReturnPlayerBehindCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// �{�X���j���o�p�J����
//===========================================================
class FinalBlowCamera : public CCameraState
{
public:
	FinalBlowCamera();
	~FinalBlowCamera() {}

	void Update(CCamera* pCamera) override;

private:

	CEnemy* m_pEnemy = nullptr;

	int m_nFocusCounter = 0;
	int m_nLookCount = 0;

	D3DXVECTOR3 m_ShakePosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ShakePosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float m_fShankeX = 0.0f;
	float m_fShankeZ = 0.0f;

	int m_nShakeTimeCounter = 0;
	int m_nFallDownCount = 0;
	float m_fShakeAngle = 0.0f;
};

//===========================================================
// �q�[�g�A�N�V�����J����
//===========================================================
class HeatActionCamera : public CCameraState
{
public:
	HeatActionCamera();
	~HeatActionCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// �G�f�B�b�g�J����
//===========================================================
class EditCamera : public CCameraState
{
public:
	EditCamera();
	~EditCamera();

	void Update(CCamera* pCamera) override;

private:

};

namespace camera
{
	CCamera *GetInstance(void);
}


#endif