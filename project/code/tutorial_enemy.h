//===========================================================
//
// �G�̏��� [tutorial_enemy.h]
// Author : �匴�@�叫
//
//===========================================================
#ifndef _TUTORIAL_ENEMY_H_
#define _TUTORIAL_ENEMY_H_

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
class CTutorialEnemyState;
class CEnemy;

//===========================================================
// �U�R�G�N���X��`
//===========================================================
class CTutorial_Enemy : public CEnemy
{
public:

	// ���[�V����
	enum MOTIONTYPE
	{
		MOTION_NEUTRAL = 0,          // �ҋ@
		MOTION_DASH,                 // �ړ�
		MOTION_ATTACK,               // �U��
		MOTION_DAMAGE,               // �U���󂯂�
		MOTION_GRAP,                 // �������҂�
		MOTION_FALLDOWN,             // �]��

		MOTION_HEATACTELECTROWAIT,   // �q�[�g�A�N�V����:�d�q�����W�ҋ@
		MOTION_HEATACTELECTRO,       // �q�[�g�A�N�V����:�d�q�����W�т�т�
		MOTION_HEATACTFAINTING,      // �q�[�g�A�N�V����:�d�q�����W�C��

		MOTION_GETUP,                // �N���オ��
		MOTION_DEATH,                // ���S
		MOTION_DENIAL,               // �q�[�g�A�N�V�����F���]�Ԏ󂯑҂�

		MOTION_PAINFULDAMAGE,        // �ɂ��U�����󂯂�
		MOTION_HEATDAMAGE,           // �U���󂯂�
		MOTION_MAX
	};

	CTutorial_Enemy();                                  //�R���X�g���N�^
	CTutorial_Enemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //�R���X�g���N�^
	~CTutorial_Enemy();                                 //�f�X�g���N�^

	HRESULT Init(void) override;                        //������
	void Uninit(void) override;                         //�I��
	void Update(void) override;                         //�X�V
	void Draw(void) override;                           //�`��

	static CTutorial_Enemy* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //����
	void ChangeState(CTutorialEnemyState* pState);  // �X�e�C�g�̐؂�ւ�

	void Damage(void) override;
	void Grabbed(void) override;
	void Denial(void) override;
	bool GetbDeathFlag(void) override;
	bool GetbHeatDamageFlag(void) override { return m_bHeatDamage; }

	// �ݒ�n
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }

	void RestHeatDamageFrag(void) { m_bHeatDamage = false; }

private:

	CGage3D* m_pLife3D;            // �Q�[�W�̃|�C���^
	CTutorialEnemyState* m_pState;         // �X�e�C�g
	int m_nDamageCounter;          // �_���[�W��Ԃł���J�E���g
	int m_nBiriBiriCount;
	int m_nAtcCounter;             // �U���̃C���^�[�o��
	int m_nIdx;
	bool m_bDamage;               // �U�����󂯂����ǂ���
	bool m_bHeatDamage;           // �q�[�g�A�N�V����������������ǂ���
};

//=================================================================
// �X�e�[�g
//=================================================================
class CTutorialEnemyState
{
public:
	CTutorialEnemyState() {};
	~CTutorialEnemyState() {};

	virtual void Update(CTutorial_Enemy* pEnemy) = 0;

private:

};

//===========================================================
// �j���[�g�������
//===========================================================
class CTutorialEnemyNeutral : public CTutorialEnemyState
{
public:
	CTutorialEnemyNeutral();
	~CTutorialEnemyNeutral() {}

	void Update(CTutorial_Enemy* pEnemy) override;

private:

};

//===========================================================
// �U��������������
//===========================================================
class CTutorialEnemyStateDamage : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateDamage();
	~CTutorialEnemyStateDamage() {};

	void Update(CTutorial_Enemy* pEnemy) override;

private:

	int m_nRecoverDamageTime = 0;  // �_���[�W��Ԃ��畜�A���鎞��

};

//===========================================================
// �悢�U��������������
//===========================================================
class CTutorialEnemyStateHeavyDamage : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateHeavyDamage();
	~CTutorialEnemyStateHeavyDamage() {};

	void Update(CTutorial_Enemy* pEnemy) override;

private:

};

//===========================================================
// �N���オ����
//===========================================================
class CTutorialEnemyStateGetUp : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateGetUp();
	~CTutorialEnemyStateGetUp() {}

	void Update(CTutorial_Enemy* pEnemy) override;

private:

};

//===========================================================
// �߂܂�Ă�����
//===========================================================
class CTutorialEnemyStateGrabbed : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateGrabbed();
	~CTutorialEnemyStateGrabbed() {}

	void Update(CTutorial_Enemy* pEnemy) override;

private:

};

//===========================================================
// ���ۏ��
//===========================================================
class CTutorialEnemyStateDenial : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateDenial();
	~CTutorialEnemyStateDenial() {}

	void Update(CTutorial_Enemy* pEnemy) override;

private:

};

//===========================================================
// ���S���
//===========================================================
class CTutorialEnemyStateDeath : public CTutorialEnemyState
{
public:
	CTutorialEnemyStateDeath();
	~CTutorialEnemyStateDeath() {};

	void Update(CTutorial_Enemy* pEnemy) override;

private:


};

#endif