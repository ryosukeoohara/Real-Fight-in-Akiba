//===========================================================
//
// �G�̏��� [enemy_weak.h]
// Author : �匴�@�叫
//
//===========================================================
#ifndef _ENEMY_WEAK_H_
#define _ENEMY_WEAK_H_

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "enemy.h"

//===========================================================
// �O���錾
//===========================================================
class CMotion;
class CCharacter;
class CGage3D;
class CEnemyWeakState;
class CEnemy;

//===========================================================
// �U�R�G�N���X��`
//===========================================================
class CEnemyWeak : public CEnemy
{
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
		MOTION_MAX
	};

	CEnemyWeak();                                  //�R���X�g���N�^
	CEnemyWeak(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //�R���X�g���N�^
	~CEnemyWeak();                                 //�f�X�g���N�^

	HRESULT Init(void) override;                        //������
	void Uninit(void) override;                         //�I��
	void Update(void) override;                         //�X�V
	void Draw(void) override;                           //�`��

	static CEnemyWeak *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //����
	void ChangeState(CEnemyWeakState* pState);  // �X�e�C�g�̐؂�ւ�
	void RecoverFromDamage(void) override;  // �_���[�W��Ԃ���̕��A

	void Damege(void) override;
	void HardDamege(void) override;
	void Grabbed(void) override;

	 // �ݒ�n
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }
	
private:

	CGage3D *m_pLife3D;            // �Q�[�W�̃|�C���^
	CEnemyWeakState* m_pState;         // �X�e�C�g
	int m_nDamegeCounter;          // �_���[�W��Ԃł���J�E���g
	int m_nBiriBiriCount;
	int m_nAtcCounter;             // �U���̃C���^�[�o��
	int m_nIdx;
	bool m_bDamage;               // �U�����󂯂����ǂ���
};

//=================================================================
// �X�e�[�g
//=================================================================
class CEnemyWeakState
{
public:
	CEnemyWeakState() {};
	~CEnemyWeakState() {};

	virtual void Update(CEnemyWeak* pEnemyWeak) = 0;

private:

};

//===========================================================
// �ړ��ҋ@���
//===========================================================
class CEnemyWeakStateMoveWait : public CEnemyWeakState
{
public:
	CEnemyWeakStateMoveWait();
	~CEnemyWeakStateMoveWait() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// �ړ����
//===========================================================
class CEnemyWeakStateMove : public CEnemyWeakState
{
public:
	CEnemyWeakStateMove();
	~CEnemyWeakStateMove() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// �U���ҋ@���
//===========================================================
class CEnemyWeakStateAttackWait : public CEnemyWeakState
{
public:
	CEnemyWeakStateAttackWait();
	~CEnemyWeakStateAttackWait() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:
	int m_nAtcCounter = 0;  // �U������܂ł̃J�E���g

};

//===========================================================
// �U�����
//===========================================================
class CEnemyWeakStateAttack : public CEnemyWeakState
{
public:
	CEnemyWeakStateAttack();
	~CEnemyWeakStateAttack() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// �U��������������
//===========================================================
class CEnemyWeakStateDamege : public CEnemyWeakState
{
public:
	CEnemyWeakStateDamege();
	~CEnemyWeakStateDamege() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// �悢�U��������������
//===========================================================
class CEnemyWeakStateHeavyDamege : public CEnemyWeakState
{
public:
	CEnemyWeakStateHeavyDamege();
	~CEnemyWeakStateHeavyDamege() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// �N���オ����
//===========================================================
class CEnemyWeakStateGetUp : public CEnemyWeakState
{
public:
	CEnemyWeakStateGetUp();
	~CEnemyWeakStateGetUp() {}

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// �߂܂�Ă�����
//===========================================================
class CEnemyWeakStateGrabbed : public CEnemyWeakState
{
public:
	CEnemyWeakStateGrabbed();
	~CEnemyWeakStateGrabbed() {}

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

//===========================================================
// ���S���
//===========================================================
class CEnemyWeakStateDeath : public CEnemyWeakState
{
public:
	CEnemyWeakStateDeath();
	~CEnemyWeakStateDeath() {};

	void Update(CEnemyWeak* pEnemyWeak) override;

private:

};

#endif