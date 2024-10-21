//===========================================================
//
// �G�̏��� [enemy_weak.h]
// Author : �匴�@�叫
//
//===========================================================
#ifndef _ENEMY_WEAK_NEAR_H_
#define _ENEMY_WEAK_NEAR_H_

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
class CEnemyWeakNearState;
class CEnemy;

//===========================================================
// �U�R�G�N���X��`
//===========================================================
class CEnemyWeakNear : public CEnemy
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

	CEnemyWeakNear();                                  //�R���X�g���N�^
	CEnemyWeakNear(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //�R���X�g���N�^
	~CEnemyWeakNear();                                 //�f�X�g���N�^

	HRESULT Init(void) override;                        // ������
	void Uninit(void) override;                         // �I��
	void Update(void) override;                         // �X�V
	void Draw(void) override;                           // �`��

	static CEnemyWeakNear* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //����
	void ChangeState(CEnemyWeakNearState* pState);  // �X�e�C�g�̐؂�ւ�

	void Damege(void) override;
	void Grabbed(void) override;

	// �ݒ�n
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }

private:

	CGage3D* m_pLife3D;            // �Q�[�W�̃|�C���^
	CEnemyWeakNearState* m_pState;         // �X�e�C�g
	int m_nDamegeCounter;          // �_���[�W��Ԃł���J�E���g
	int m_nBiriBiriCount;
	int m_nAtcCounter;             // �U���̃C���^�[�o��
	int m_nIdx;
	bool m_bDamage;               // �U�����󂯂����ǂ���
};

//=================================================================
// �X�e�[�g
//=================================================================
class CEnemyWeakNearState
{
public:
	CEnemyWeakNearState() {};
	~CEnemyWeakNearState() {};

	virtual void Update(CEnemyWeakNear* pEnemyWeak) = 0;

private:

};

//===========================================================
// �ړ��ҋ@���
//===========================================================
class CEnemyWeakNearStateMoveWait : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateMoveWait();
	~CEnemyWeakNearStateMoveWait() {}

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// �ړ����
//===========================================================
class CEnemyWeakNearStateMove : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateMove();
	~CEnemyWeakNearStateMove() {};

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// �U���ҋ@���
//===========================================================
class CEnemyWeakNearStateAttackWait : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateAttackWait();
	~CEnemyWeakNearStateAttackWait() {};

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:
	int m_nAtcCounter = 0;  // �U������܂ł̃J�E���g

};

//===========================================================
// �U�����
//===========================================================
class CEnemyWeakNearStateAttack : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateAttack();
	~CEnemyWeakNearStateAttack() {};

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// �U��������������
//===========================================================
class CEnemyWeakNearStateDamege : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateDamege();
	~CEnemyWeakNearStateDamege() {};

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// �悢�U��������������
//===========================================================
class CEnemyWeakNearStateHeavyDamege : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateHeavyDamege();
	~CEnemyWeakNearStateHeavyDamege() {};

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// �N���オ����
//===========================================================
class CEnemyWeakNearStateGetUp : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateGetUp();
	~CEnemyWeakNearStateGetUp() {}

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// �߂܂�Ă�����
//===========================================================
class CEnemyWeakNearStateGrabbed : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateGrabbed();
	~CEnemyWeakNearStateGrabbed() {}

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

//===========================================================
// ���S���
//===========================================================
class CEnemyWeakNearStateDeath : public CEnemyWeakNearState
{
public:
	CEnemyWeakNearStateDeath();
	~CEnemyWeakNearStateDeath() {}

	void Update(CEnemyWeakNear* pEnemyWeak) override;

private:

};

#endif