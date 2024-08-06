//===========================================================
//
// ���[�V��������[motion.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MOTION_H_
#define _MOTION_H_

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "objectX.h"

//�}�N����`
#define MAX_INFO   (64)

//===========================================================
// �O���錾
//===========================================================
class CCharacter;

//===========================================================
// ���[�V�����N���X��`
//===========================================================
class CMotion
{
private:

	//���f��---------------------------------------------------------------
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;

	} KEY;

	//���f��---------------------------------------------------------------
	typedef struct
	{
		KEY aKey[MAX_INFO];  //���݂̃L�[
		int nFrame;   //���[�V�����̃t���[����

	} KEYINFO;

	//���[�V����---------------------------------------------------------------
	typedef struct
	{
		int nNumKey;    // �L�[�̑���
		int nLoop;      // ���[�v���邩�ǂ���
		int nNumFrame;  // �t���[���̑���
		int nAttackOccurs;  // �U�����蔭���t���[��
		int nAttackEnd;  // �U������I���t���[��
		int nAttackDamege;  // �U����
		int nInvincibleStart;  // ���S���G��ԊJ�n
		int nInvincibleEnd;    // ���S���G�I��
		int nCancellFrame;   // �L�����Z���\�t���[��
		int nCancellFrameEnd;  // �L�����Z���\�t���[��
		float nKnockBack;      // �������
		bool bLoop;     // ���[�v���邩�ǂ���
		bool bFinish;   // �I���������ǂ���

		KEYINFO KeySet[MAX_INFO];

	} INFO;

public:
	CMotion();
	~CMotion();

	void Init(void);
	void Uninit(void);
	void Update(void);

	bool IsFinish(void);
	
	// �ǂݍ��݌n
	void ReadText(const char *TextFilename);            // �O���t�@�C���ǂݍ���
	
	// �ݒ�n
	void Set(int nType, bool value = true);            // ���[�V����
	void SetInfoBlendON(void);                                 // ���
	void SetInfoBlendOFF(KEY key);
	void SetModel(CCharacter **ppModel, int nNumModel); // �L�����N�^�[�̃��f��
	void InitPose(int type);

	// �擾�n
	int GetType(void);                                  // ���
	int GetNumFrame(void) { return m_nNowFrame; }       // ���[�V�����̑��t���[��
	int GetNowFrame(void);
	int GetAttackOccurs(void);
	int GetAttackEnd(void);
	int GetAttackDamege(void);
	int GetInvincibleStrat(void);
	int GetInvincibleEnd(void);
	int GetCancellFrame(void);
	int GetCancellFrameEnd(void);
	float GetKnockBack(void);
	INFO GetInfo(void);

private:

	void RotCorrect(float rot);
	void BlendON(void);
	void BlendOFF(void);

	INFO m_aInfo[MAX_INFO];
	KEY m_aOldInfo[MAX_INFO];
	int  m_nNumAll;              //���[�V�����̑���
	int  m_nType;                //���[�V�����̎��
	int m_nTypeold;              //�O��̃��[�V�����̎��
	bool m_bLoop;                //���[�v���邩�ǂ���
	int  m_nNumKey;              //�L�[�̑���
	int  m_nKey;                 //���݂̃L�[
	int  m_nCounter;             //�t���[�����ɒB������
	int  m_nCntkeySet;
	int m_nNumFrame;             //���[�V�����̃t���[����
	int m_nNowFrame;
	bool m_bFinish;              //�I���������ǂ���
	bool m_bBlend;               // ���[�V�����u�����h���邩�ǂ���
	
	CCharacter **m_ppModel;  //���f���ւ̃_�u���|�C���^
	int m_nNumModel;         //���f���̑���
	static int m_nObj;       //�N�̃��[�V������
};

#endif