//===========================================================
//
// �G�̏��� [enemy.h]
// Author : �匴�@�叫
//
//===========================================================
#ifndef _ENEMY_BOSS_H_
#define _ENEMY_BOSS_H_

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "enemy.h"
#include "player.h"

//===========================================================
// �O���錾
//===========================================================
class CMotion;
class CCharacter;
class CGage2D;
class CEnemyBossState;
class CEnemy;
class CAttackType;

//===========================================================
// �{�X�G�N���X��`
//===========================================================
class CEnemyBoss : public CEnemy
{
public:

	// ���
	enum STATE
	{
		STATE_NONE = 0,             // �Ȃ���Ȃ�
		STATE_NEUTRAL,              // �ҋ@
		STATE_DASH,                 // �ړ�
		STATE_WALK,                 // �ړ�
		STATE_ATTACK,               // �U��
		STATE_GRAP,                 // �������҂�
		STATE_DAMEGE,               // �U���󂯂�
		STATE_PAINFULDAMAGE,        // �ɂ��U�����󂯂�
		STATE_HEATDAMEGE,           // �U���󂯂�
		STATE_GETUP,                // �N���オ��
		STATE_HEATACTELECTROWAIT,   // �q�[�g�A�N�V����:�d�q�����W�ҋ@
		STATE_HEATACTELECTRO,       // �q�[�g�A�N�V����:�d�q�����W�т�т�
		STATE_HEATACTFAINTING,      // �q�[�g�A�N�V����:�d�q�����W�C��
		STATE_FALLDOWN,             // �]��
		STATE_ONSTEGE,
		STATE_DEATH,                // ���S
		STATE_MAX
	};

	// ���[�V����
	enum MOTIONTYPE
	{
		MOTION_NEUTRAL = 0,          // �ҋ@
		MOTION_WALK,                 // �ړ�
		MOTION_ATTACK,               // �U��
		MOTION_DAMEGE,               // �U���󂯂�
		MOTION_GRAP,                 // �������҂�
		MOTION_HEATDAMEGE,           // �U���󂯂�
		MOTION_HEATACTELECTROWAIT,   // �q�[�g�A�N�V����:�d�q�����W�ҋ@
		MOTION_HEATACTELECTRO,       // �q�[�g�A�N�V����:�d�q�����W�т�т�
		MOTION_HEATACTFAINTING,      // �q�[�g�A�N�V����:�d�q�����W�C��MOTIONE_PAINFULDAMAGE,        // �ɂ��U�����󂯂�
		MOTION_GETUP,                // �N���オ��
		MOTION_DEATH,                // ���S
		MOTION_FALLDOWN,             // �]��
		MOTION_GURUGURU,              // ���邮��p���`
		MOTION_PUNCH,                 // �p���`
		MOTION_SUTAN,                 // �X�^��
		MOTION_SUTANRECOVER,          // �X�^������̕��A
		MOTION_ONSTEGE,
		MOTION_KUGIKIRI,
		MOTION_MAX
	};

	

public:

	CEnemyBoss();                                  //�R���X�g���N�^
	CEnemyBoss(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //�R���X�g���N�^
	~CEnemyBoss();                                 //�f�X�g���N�^

	HRESULT Init(void) override;                        //������
	void Uninit(void) override;                         //�I��
	void Update(void) override;                         //�X�V
	void Draw(void) override;                           //�`��

	static CEnemyBoss *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //����
	void ChangeState(CEnemyBossState* pState);  // �X�e�C�g�̐؂�ւ�
	void RecoverFromDamage(void) override;

	// �ݒ�n
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }

	void Grabbed(void) override;
	void RollingPunch(void);
	void NormalPunch(void);
	void Fly(void);

private:

	// �G�̓�������
	//void Controll(void) override;
	
	void Damege(void) override;
	void HardDamege(void) override;
	
	void HitDetection(D3DXVECTOR3 MyPos, float attackrange, float targetradius);
	//void MicroWave(void) override;

	CGage2D *m_pLife2D;           // �Q�[�W�̃|�C���^
	CEnemyBossState* m_pState;         // �X�e�C�g
	int m_nDamegeCounter;         // �_���[�W��Ԃł���J�E���g
	int m_nBiriBiriCount;         // �d�q�����W�т�т�J�E���g
	int m_nAtcCounter;            // �U���̃C���^�[�o��
	int m_nReceivedAttack;        // �U�����󂯂���
	
	int m_nIdx;
	bool m_bDamage;               // �U�����󂯂����ǂ���
	

};

//===========================================================
// �{�X�̍s���X�e�[�g
//===========================================================
class CEnemyBossState
{
public:
	CEnemyBossState();
	~CEnemyBossState() {}

	virtual void Update(CEnemyBoss* pEnemyBoss) = 0;

private:
	
};

//===========================================================
// �X�e�[�W�o����
//===========================================================
class CEnemyBossOnStege : public CEnemyBossState
{
public:
	CEnemyBossOnStege();
	~CEnemyBossOnStege() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// �o�����Ă���ړ��J�n����܂ł̏��
//===========================================================
class CEnemyBossAppear : public CEnemyBossState
{
public:
	CEnemyBossAppear();
	~CEnemyBossAppear() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// �o�����Ă���ړ��J�n����܂ł̏��
//===========================================================
class CEnemyBossAppearMove : public CEnemyBossState
{
public:
	CEnemyBossAppearMove();
	~CEnemyBossAppearMove() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// �U�����Ă�����
//===========================================================
class CEnemyBossStateAttack : public CEnemyBossState
{
public:
	CEnemyBossStateAttack();
	~CEnemyBossStateAttack() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

	//void ChangeState(CAttackType* pAttackType);

private:

	CAttackType* m_pAttackType = nullptr;

};

//===========================================================
// �ړ��ҋ@���
//===========================================================
class CEnemyBossStateMoveWait : public CEnemyBossState
{
public:
	CEnemyBossStateMoveWait();
	~CEnemyBossStateMoveWait() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// �ړ����
//===========================================================
class CEnemyBossStateMove : public CEnemyBossState
{
public:
	CEnemyBossStateMove();
	~CEnemyBossStateMove() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// �U���ҋ@���
//===========================================================
class CEnemyBossStateAttackWait : public CEnemyBossState
{
public:
	CEnemyBossStateAttackWait();
	~CEnemyBossStateAttackWait() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

	enum ATTACKTYPE
	{
		ATTACKTYPE_GURUGURU = 0,
		ATTACKTYPE_PUNCH,
		ATTACKTYPE_FLY,
		ATTACKTYPE_MAX
	};

private:
	int m_nAtcCounter = 0;  // �U������܂ł̃J�E���g
	int m_nAttackType = 0;
	bool m_bAttack = false;
};

//===========================================================
// �U��������������
//===========================================================
class CEnemyBossStateDamege : public CEnemyBossState
{
public:
	CEnemyBossStateDamege();
	~CEnemyBossStateDamege() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// �悢�U��������������
//===========================================================
class CEnemyBossStateHeavyDamege : public CEnemyBossState
{
public:
	CEnemyBossStateHeavyDamege();
	~CEnemyBossStateHeavyDamege() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// �߂܂�Ă�����
//===========================================================
class CEnemyBossStateGrabbed : public CEnemyBossState
{
public:
	CEnemyBossStateGrabbed();
	~CEnemyBossStateGrabbed() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// ���S���
//===========================================================
class CEnemyBossStateDeath : public CEnemyBossState
{
public:
	CEnemyBossStateDeath();
	~CEnemyBossStateDeath() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// ���߂����
//===========================================================
class CEnemyBossStateStagger : public CEnemyBossState
{
public:
	CEnemyBossStateStagger();
	~CEnemyBossStateStagger() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

	int m_nStaggerTime = 0;  // ���߂���Ԃł��鎞��
};

//===========================================================
// ���߂���Ԃ���̕��A
//===========================================================
class CEnemyBossStateStaggerReCover : public CEnemyBossState
{
public:
	CEnemyBossStateStaggerReCover();
	~CEnemyBossStateStaggerReCover() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

	int m_nStaggerTime = 0;  // ���߂���Ԃł��鎞��
};

//===========================================================
// �]�ԏ��
//===========================================================
class CEnemyBossStateKnockDown : public CEnemyBossState
{
public:
	CEnemyBossStateKnockDown();
	~CEnemyBossStateKnockDown() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// �N���オ����
//===========================================================
class CEnemyBossStateGetUp : public CEnemyBossState
{
public:
	CEnemyBossStateGetUp();
	~CEnemyBossStateGetUp() {}

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

};

//===========================================================
// �{�X�̍s���X�e�C�g
//===========================================================
class CAttackType
{
public:
	CAttackType();
	~CAttackType() {};

	virtual void Update(CEnemy* pEnemy) = 0;

private:

};

class CAttackTypeNormalPunch : public CAttackType
{
public:
	CAttackTypeNormalPunch();
	~CAttackTypeNormalPunch() {};

	void Update(CEnemy* pEnemy) override;

private:

};

class CAttackTypeRollingPunch : public CAttackType
{
public:
	CAttackTypeRollingPunch();
	~CAttackTypeRollingPunch() {};

	void Update(CEnemy* pEnemy) override;

private:

};

class CAttackTypeBodyPress : public CAttackType
{
public:
	CAttackTypeBodyPress();
	~CAttackTypeBodyPress() {};

	void Update(CEnemy* pEnemy) override;

private:

};

#endif // !_ENEMY_H_