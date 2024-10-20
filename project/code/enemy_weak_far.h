//===========================================================
//
// �G�̏��� [enemy_weak.h]
// Author : �匴�@�叫
//
//===========================================================
#ifndef _ENEMY_WEAK_FAR_H_
#define _ENEMY_WEAK_FAR_H_

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
class CEnemyWeakFarState;
class CEnemy;

//===========================================================
// �U�R�G�N���X��`
//===========================================================
class CEnemyWeakFar : public CEnemy
{
public:

	// ���[�V����
	enum MOTIONTYPE
	{
		MOTION_NEUTRAL = 0,          // �ҋ@
		MOTION_DASH,                 // �ړ�
		MOTION_ATTACK,               // �U��
		MOTION_BATTLE_NEUTRAL,       // �擪�j���[�g����
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

	CEnemyWeakFar();                                  //�R���X�g���N�^
	CEnemyWeakFar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //�R���X�g���N�^
	~CEnemyWeakFar();                                 //�f�X�g���N�^

	HRESULT Init(void) override;                        // ������
	void Uninit(void) override;                         // �I��
	void Update(void) override;                         // �X�V
	void Draw(void) override;                           // �`��

	static CEnemyWeakFar* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //����
	void ChangeState(CEnemyWeakFarState* pState);  // �X�e�C�g�̐؂�ւ�
	void RecoverFromDamage(void) override;  // �_���[�W��Ԃ���̕��A

	void Damege(void) override;
	void HardDamege(void) override;
	void Grabbed(void) override;

	// �ݒ�n
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }

private:

	CGage3D* m_pLife3D;            // �Q�[�W�̃|�C���^
	CEnemyWeakFarState* m_pState;         // �X�e�C�g
	int m_nDamegeCounter;          // �_���[�W��Ԃł���J�E���g
	int m_nBiriBiriCount;
	int m_nAtcCounter;             // �U���̃C���^�[�o��
	int m_nIdx;
	bool m_bDamage;               // �U�����󂯂����ǂ���
};

//=================================================================
// �X�e�[�g
//=================================================================
class CEnemyWeakFarState
{
public:
	CEnemyWeakFarState() {};
	~CEnemyWeakFarState() {};

	virtual void Update(CEnemyWeakFar* pEnemyWeak) = 0;

private:

};

//===========================================================
// �ړ��ҋ@���
//===========================================================
class CEnemyWeakFarStateMoveWait : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateMoveWait();
	~CEnemyWeakFarStateMoveWait() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// �ړ����
//===========================================================
class CEnemyWeakFarStateMove : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateMove();
	~CEnemyWeakFarStateMove() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// �U���ҋ@���
//===========================================================
class CEnemyWeakFarStateAttackWait : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateAttackWait();
	~CEnemyWeakFarStateAttackWait() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:
	int m_nAtcCounter = 0;  // �U������܂ł̃J�E���g

};

//===========================================================
// �U�����
//===========================================================
class CEnemyWeakFarStateAttack : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateAttack();
	~CEnemyWeakFarStateAttack() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// �U��������������
//===========================================================
class CEnemyWeakFarStateDamege : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateDamege();
	~CEnemyWeakFarStateDamege() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// �悢�U��������������
//===========================================================
class CEnemyWeakFarStateHeavyDamege : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateHeavyDamege();
	~CEnemyWeakFarStateHeavyDamege() {};

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// �N���オ����
//===========================================================
class CEnemyWeakFarStateGetUp : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateGetUp();
	~CEnemyWeakFarStateGetUp() {}

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// �߂܂�Ă�����
//===========================================================
class CEnemyWeakFarStateGrabbed : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateGrabbed();
	~CEnemyWeakFarStateGrabbed() {}

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:

};

//===========================================================
// ���S���
//===========================================================
class CEnemyWeakFarStateDeath : public CEnemyWeakFarState
{
public:
	CEnemyWeakFarStateDeath();
	~CEnemyWeakFarStateDeath() {}

	void Update(CEnemyWeakFar* pEnemyWeak) override;

private:


};

#endif