//===========================================================
//
// �}�b�v�ɔz�u��Ă���򏈗�[mapobject_fance.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAPOBJECT_FANCE_H__             //���̃}�N����`������ĂȂ�������
#define _MAPOBJECT_FANCE_H__             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// �O���錾
class CFanceState;

//===========================================================
// ��N���X��`
//===========================================================
class CMapObject_Fance : public CMapObject
{
public:
	CMapObject_Fance();
	CMapObject_Fance(const char* aModelFliename, int nPriority = 3);
	~CMapObject_Fance();

	HRESULT Init(void);           // ����������    
	void Uninit(void);            // �I������
	void Update(void);            // �X�V����
	void Draw(void);              // �`�揈��

	static CMapObject_Fance* Create(const char* aModelFliename, int nPriority = 3);  //����
	void ChangeState(CFanceState* pState);  // �X�e�[�g�̕ύX

	// �擾
	static CMapObject_Fance* GetTop(void) { return m_pTop; }  // ���X�g�̐擪

private:

	CFanceState* m_pState;  // �X�e�[�g�̃|�C���^

	static CMapObject_Fance* m_pTop;  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CMapObject_Fance* m_pCur;  // �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_Fance* m_pNext;        // ���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_Fance* m_pPrev;        // �O�̃I�u�W�F�N�g�ւ̃|�C���^

	bool m_bHit;  // �����������ǂ���
};

//===========================================================
// �X�e�[�g
//===========================================================
class CFanceState
{
public:
	CFanceState() {}
	~CFanceState() {}

	virtual void Update(CMapObject_Fance* pFance) = 0;

private:

};

// �������
class CFanceBlowAway : public CFanceState
{
public:
	CFanceBlowAway();
	~CFanceBlowAway() {}

	void Update(CMapObject_Fance* pFance) override;

private:

	float m_fFallDownSpeed = 0.0f;

};

// �o�E���h
class CFanceNeutral : public CFanceState
{
public:
	CFanceNeutral();
	~CFanceNeutral() {}

	void Update(CMapObject_Fance* pFance) override;

private:

};

#endif