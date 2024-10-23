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
		STATE_DAMAGE,               // �U���󂯂�
		STATE_PAINFULDAMAGE,        // �ɂ��U�����󂯂�
		STATE_HEATDAMAGE,           // �U���󂯂�
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
		MOTION_DAMAGE,               // �U���󂯂�
		MOTION_GRAP,                 // �������҂�
		MOTION_HEATDAMAGE,           // �U���󂯂�
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
		MOTION_ONSTEGE,               // �o�ꎞ���s
		MOTION_KUGIKIRI,              // �������^��
		MOTION_DENIAL,                // �q�[�g�A�N�V�����F���]�Ԏ󂯑҂�
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
	void DashEffect(void);         // �����Ă���Ƃ��ɏo���G�t�F�N�g
	void Damage(void) override;
	

	// �ݒ�n
	void SetChase(CHASE cha) { m_Chase = cha; }
	void SetbDamage(void) { m_bDamage = false; }
	void SetbHeatDamage(bool bDamage) { m_bHeatDamage = bDamage; }
	void SetbAttack(bool battack) { m_bAttack = battack; }
	void SetbAttackReady(bool battack) { m_IsAttackReady = battack; }
	void SetbStagger(bool bstagger) { m_bStagger = bstagger; }

	// �擾�n
	bool GetbAttack(void) { return m_bAttack; }
	bool GetbAttackReady(void) { return m_IsAttackReady; }

	void Grabbed(void) override;
	void Denial(void) override;
	bool GetbDeathFlag(void) override { return m_bDeath; }
	bool GetbHeatDamageFlag(void) override { return m_bHeatDamage; }
	bool GetbStaggerFlag(void) override { return m_bStagger; }

	void RestHeatDamageFrag(void) { m_bHeatDamage = false; }
	void RollingPunch(void);
	void NormalPunch(void);
	void Fly(void);

private:

	// �G�̓�������
	//void Controll(void) override;
	
	void HitDetection(D3DXVECTOR3 MyPos, float attackrange, float targetradius);
	//void MicroWave(void) override;

	CGage2D *m_pLife2D;           // �Q�[�W�̃|�C���^
	CEnemyBossState* m_pState;         // �X�e�C�g
	int m_nDamageCounter;         // �_���[�W��Ԃł���J�E���g
	int m_nBiriBiriCount;         // �d�q�����W�т�т�J�E���g
	int m_nAtcCounter;            // �U���̃C���^�[�o��
	int m_nReceivedAttack;        // �U�����󂯂���
	int m_nIdleTime;              // �U�����Ă��Ȃ�����
	int m_nIdx;
	bool m_bDamage;               // �U�����󂯂����ǂ���
	bool m_bAttack;               // �U�����Ă��邩�ǂ���
	bool m_bHeatDamage;           // �q�[�g�A�N�V����������������ǂ���
	bool m_IsAttackReady;         // ���U���ł��邩�ǂ���
	bool m_bDeath;                // ���S�t���O
	bool m_bStagger;              // ��낯�t���O

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
class CEnemyBossStateDamage : public CEnemyBossState
{
public:
	CEnemyBossStateDamage();
	~CEnemyBossStateDamage() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

	int m_nRecoverDamageTime = 0;  // �_���[�W��Ԃ��畜�A���鎞��

};

//===========================================================
// �悢�U��������������
//===========================================================
class CEnemyBossStateHeavyDamage : public CEnemyBossState
{
public:
	CEnemyBossStateHeavyDamage();
	~CEnemyBossStateHeavyDamage() {};

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
// ���ۏ��
//===========================================================
class CEnemyBossStateDenial : public CEnemyBossState
{
public:
	CEnemyBossStateDenial();
	~CEnemyBossStateDenial() {}

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

	bool m_bDeath = false;

};

//===========================================================
// �d�q�����W�Ŏ��S���
//===========================================================
class CEnemyBossStateDeathByMicrowave : public CEnemyBossState
{
public:
	CEnemyBossStateDeathByMicrowave();
	~CEnemyBossStateDeathByMicrowave() {};

	void Update(CEnemyBoss* pEnemyBoss) override;

private:

	bool m_bDeath = false;
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

#endif // !_ENEMY_H_