//===========================================================
//
// �G�̏��� [enemy.h]
// Author : �匴�@�叫
//
//===========================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "object.h"
#include "player.h"

//===========================================================
// �O���錾
//===========================================================
class CMotion;
class CCharacter;
class CEnemyState;

//===========================================================
// �N���X��`
//===========================================================
class CEnemy : public CObject
{
public:

	// �G�̎��
	enum TYPE
	{
		WEAK = 0,  // �U�R
		BOSS,      // �{�X
		MAX
	};

	// �ǔ�
	enum CHASE
	{
		CHASE_OFF = 0,
		CHASE_ON,
		CHASE_MAX
	};

	enum MOBILITY
	{
		Mobile = 0,  // ������
		Immobile,    // �����Ȃ�
		MOBILITY_MAX
	};

	MOBILITY m_Mobility;

	CHASE m_Chase;

public:

	// ���[�V����
	enum MOTIONTYPE
	{
		MOTION_NEUTRAL = 0,          // �ҋ@
		MOTION_DASH,                 // �ړ�
		MOTION_ATTACK,               // �U��
		MOTION_DAMEGE,               // �U���󂯂�
		MOTION_GRAP,                 // �������҂�
		MOTION_FALLDOWN,             // �]��
		
		MOTION_HEATACTELECTROWAIT,   // �q�[�g�A�N�V����:�d�q�����W�ҋ@
		MOTION_HEATACTELECTRO,       // �q�[�g�A�N�V����:�d�q�����W�т�т�
		MOTION_HEATACTFAINTING,      // �q�[�g�A�N�V����:�d�q�����W�C��

		MOTION_GETUP,                // �N���オ��
		MOTION_DEATH,                // ���S

		MOTION_PAINFULDAMAGE,        // �ɂ��U�����󂯂�
		MOTION_HEATDAMEGE,           // �U���󂯂�
		//MOTION_ONSTEGE,
		MOTION_MAX
	};

	// ���
	enum STATE
	{
		STATE_NONE = 0,             // �Ȃ���Ȃ�
		STATE_NEUTRAL,              // �ҋ@
		STATE_DASH,                 // �ړ�
		STATE_ATTACK,               // �U��
		STATE_DAMEGE,               // �U���󂯂�
		STATE_GRAP,                 // �������҂�
		STATE_PAINFULDAMAGE,        // �ɂ��U�����󂯂�
		STATE_HEATDAMEGE,           // �U���󂯂�
		STATE_GETUP,                // �N���オ��
		STATE_HEATACTELECTROWAIT,   // �q�[�g�A�N�V����:�d�q�����W�ҋ@
		STATE_HEATACTELECTRO,       // �q�[�g�A�N�V����:�d�q�����W�т�т�
		STATE_HEATACTFAINTING,      // �q�[�g�A�N�V����:�d�q�����W�C��
		STATE_FALLDOWN,             // �]��
		STATE_DEATH,                // ���S
		STATE_MAX
	};

	// ���
	struct INFO
	{							    
		D3DXVECTOR3 pos;            // �ʒu
		D3DXVECTOR3 posOld;         // �O��̈ʒu
		D3DXVECTOR3 rot;            // ����
		D3DXVECTOR3 move;           // �ړ���
		D3DXMATRIX mtxWorld;        // ���[���h�}�g���b�N�X
		STATE state;                // ���
		int nIdxID;                 // �C���f�b�N�X�ԍ�
		int nLife;                  // �̗�
		bool bDraw;                 // �`�悷�邩�ǂ���
	};

	CEnemy();                                  //�R���X�g���N�^
	CEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //�R���X�g���N�^
	~CEnemy();                                 //�f�X�g���N�^
										       
	virtual HRESULT Init(void);                        //������
	virtual void Uninit(void);                         //�I��
	virtual void Update(void);                         //�X�V
	virtual void Draw(void);                           //�`��

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //����
	void ReadText(const char *fliename);             //�e�L�X�g�t�@�C���ǂݍ���

	//�@�ݒ�n
	void SetPosition(D3DXVECTOR3 pos) { m_Info.pos = pos; }         // �ʒu
	void SetRotition(D3DXVECTOR3 rot) { m_Info.rot = rot; }         // ����
	void SetMove(D3DXVECTOR3 move) { m_Info.move = move; }          // �ړ���
	void SetState(STATE state) { m_Info.state = state; }            // ���
	void SetLife(int nLife) { m_Info.nLife = nLife; }               // �̗�
	void SetCurrent(D3DXMATRIX *Current) { m_pCurrent = Current; }  // �e�̃}�g���b�N�X
	void SetIdx(int idx) { m_Info.nIdxID = idx; }                   // ID
	void SetType(TYPE type) { m_Type = type; }                      // ���
	void SetNumAll(int nNum) { m_nNumAll = nNum; }
	virtual void SetChase(CHASE cha);
	void SetDraw(bool value) { m_Info.bDraw = value; }
	void SetMobile(void) { m_Mobility = Mobile; }                    // ������悤�ɂ���
	void SetImmobile(void) { m_Mobility = Immobile; }                // �����Ȃ��悤�ɂ���

	// �擾�n
	D3DXVECTOR3 GetPosition(void) { return m_Info.pos; }       // �ʒu
	D3DXVECTOR3 GetRotition(void) { return m_Info.rot; }       // ����
	D3DXVECTOR3 GetMove(void) { return m_Info.move; }          // �ړ���
	STATE GetState(void) { return m_Info.state; }              // ���
	int GetLife(void) { return m_Info.nLife; }                 // �̗�
	int GetIdxID(void) { return m_Info.nIdxID; }
	TYPE GetType(void) { return m_Type; }
	CCharacter **GetCharcter(void) { return m_apModel; }
	static int GetNumAll(void) { return m_nNumAll; }
 	CMotion *GetMotion(void) { return m_pMotion; }
	MOBILITY GetMobility(void) { return m_Mobility; }
	static CEnemy *GetTop(void) { return m_pTop; }
	CEnemy *GetNext(void) { return m_pNext; }
	CEnemy::INFO *GetInfo(void) { return &m_Info; }

	virtual void Damege(void) = 0;

	//virtual void ChangeState(CEnemyState *pEnemy) = 0;
	virtual void RecoverFromDamage(void) = 0;

	void HitDetection(D3DXVECTOR3 MyPos, float attackrange, float targetradius);

protected:

	// ���䏈��
	void Controll(void);
	TYPE m_Type;

private:

	void ListOut(void);
	
	INFO m_Info;                   // ���
	CMotion *m_pMotion;            // ���[�V�����ւ̃|�C���^
	CCharacter **m_apModel;        // ���f��(�p�[�c)�ւ̃|�C���^
	CGage3D *m_pLife3D;            // �Q�[�W�̃|�C���^
	CGage2D *m_pLife2D;            // �Q�[�W�̃|�C���^
	static int m_nNumAll;          // �G�̑���
	int m_nDamegeCounter;          // �_���[�W��Ԃł���J�E���g
	static int m_nIdx;             
	D3DXMATRIX *m_pCurrent;        // �e�̃}�g���b�N�X
	static CEnemy *m_pTop;         //�擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CEnemy *m_pCur;         //�Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CEnemy *m_pNext;
	CEnemy *m_pPrev;               //�O�̃I�u�W�F�N�g�ւ̃|�C���^
	bool m_bDeath;                 //���S�t���O
	
	//*=============================================================================
	// �O���t�@�C���ǂݍ��ݗp
	//*=============================================================================
	D3DXVECTOR3 m_Readpos;                     // �ʒu�ǂݍ��ݗp
	D3DXVECTOR3 m_Readrot;                     // �����ǂݍ��ݗp
	int m_nidx;                                // �C���f�b�N�X�ԍ�
	int m_nParent;                             // �e�̃C���f�b�N�X�ԍ�
	int m_nNumModel;                           // ���f��(�p�[�c)�̑���
	char m_filename[128] = {};                 // �����ǂݍ��ݗp

};

////===========================================================
//// �X�e�C�g
////===========================================================
//class CEnemyState
//{
//public:
//	CEnemyState() {};
//	~CEnemyState() {};
//
//	virtual void Update(CEnemy* pEnemy) = 0;
//
//private:
//
//};



#endif // !_ENEMY_H_