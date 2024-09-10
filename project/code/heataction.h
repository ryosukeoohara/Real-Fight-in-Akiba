//===========================================================
//
// �q�[�g�A�N�V����[heataction.h]
// Author �匴�叫
//
//===========================================================
#ifndef _HIEATACTION_H_
#define _HIEATACTION_H_

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "player.h"

//===========================================================
// �O���錾
//===========================================================
class CEnemy;
class CItem;
class CHeatActionBehaviour;

//===========================================================
// �q�[�g�A�N�V�����N���X��`
//===========================================================
class CHeatAction
{
public:

	CHeatAction();
	~CHeatAction();
	void Init(void);
	void Uninit(void);
	void Updata(void);

	void BikeCrash(CPlayer* pPlayer, CEnemy* pEnemy);                         // ���]�Ԃ��g�p�����A�N�V����
	void MicroWave(CPlayer* pPlayer, CEnemy* pEnemy);                         // �d�q�����W���g�p�����A�N�V����
	void SetAction(CHeatActionBehaviour* Behaviour, CPlayer* pPlayer, CEnemy* pEnemy);  // �A�N�V������ݒ�

	// �擾�n
	CPlayer* GetPlayer(void) { return m_pPlayer; }  // �v���C���[
	CEnemy* GetEnemy(void) { return m_pEnemy; }     // �G�l�~�[

private:

	void ChangeBehaviour(CHeatActionBehaviour* Behaviour);  // �r�w�C�r�A�̐؂�ւ�
	CHeatActionBehaviour *m_pBehaviour;
	CPlayer::HEAT m_HeatAct;
	CPlayer* m_pPlayer;
	CEnemy* m_pEnemy;

	int m_nHeatActTime;

	bool m_bInMicroWave = false;;  // �q�[�g�A�N�V����:�d�q�����W�œG���d�q�����W�̒��ɓ��������ǂ���
};

//===========================================================
// �r�w�C�r�A
//===========================================================
class CHeatActionBehaviour
{
public:
	CHeatActionBehaviour() {}
	~CHeatActionBehaviour() {}

	virtual void Update(CHeatAction *pHeatAct) = 0;

private:

};

//===========================================================
// ���]�Ԃ��g�p�����A�N�V����
//===========================================================
class BikeCrash : public CHeatActionBehaviour
{
public:
	BikeCrash();
	~BikeCrash() {}

	void Update(CHeatAction* pHeatAct) override;

private:

};

//===========================================================
// �d�q�����W���g�p�����A�N�V����
//===========================================================
class MicroWave : public CHeatActionBehaviour
{
public:
	MicroWave();
	~MicroWave() {}

	void Update(CHeatAction* pHeatAct) override;

private:

	bool m_bInMicroWave = false;
	int m_nHeatActTime = 0;
};

#endif