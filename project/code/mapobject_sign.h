//===========================================================
//
// �}�b�v�ɔz�u��Ă���W������[mapobject_sign.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAPOBJECT_SIGN_H__             //���̃}�N����`������ĂȂ�������
#define _MAPOBJECT_SIGN_H__             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// �O���錾
class CSignState;

//===========================================================
// �W���N���X��`
//===========================================================
class CMapObject_Sign : public CMapObject
{
public:
	CMapObject_Sign();
	CMapObject_Sign(const char* aModelFliename, int nPriority = 3);
	~CMapObject_Sign();

	HRESULT Init(void);           // ����������    
	void Uninit(void);            // �I������
	void Update(void);            // �X�V����
	void Draw(void);              // �`�揈��

	static CMapObject_Sign* Create(const char* aModelFliename, int nPriority = 3);  //����
	void ChangeState(CSignState* pState);  // �X�e�[�g��ύX

	// �擾
	CMapObject_Sign* GetTop(void) { return m_pTop; }  // ���X�g�̐擪

private:

	CSignState* m_pState;  // �X�e�[�g�̃|�C���^

	static CMapObject_Sign* m_pTop;  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CMapObject_Sign* m_pCur;  // �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_Sign* m_pNext;        // ���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_Sign* m_pPrev;        // �O�̃I�u�W�F�N�g�ւ̃|�C���^

	bool m_bShake = false;

};

//===========================================================
// �X�e�C�g
//===========================================================
class CSignState
{
public:
	CSignState() {}
	~CSignState() {}

	virtual void Update(CMapObject_Sign* pTrashBox) = 0;

private:

};

// �j���[�g����
class CSignNeutral : public CSignState
{
public:
	CSignNeutral();
	~CSignNeutral() {}

	void Update(CMapObject_Sign* pTrashBox) override;

private:

};

// ����ꂽ
class CSignBeaten : public CSignState
{
public:
	CSignBeaten();
	~CSignBeaten() {}

	void Update(CMapObject_Sign* pElectricBox) override;

private:

	int m_nShakeTimeCounter;
	int m_nFallDownCount;
	float m_fShakeAngle;
};

// �������
class CSignBlowAway : public CSignState
{
public:
	CSignBlowAway();
	~CSignBlowAway() {}

	void Update(CMapObject_Sign* pTrashBox) override;

private:

};

// �o�E���h
class CSignBound : public CSignState
{
public:
	CSignBound();
	~CSignBound() {}

	void Update(CMapObject_Sign* pTrashBox) override;

private:

};

#endif
