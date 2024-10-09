//===========================================================
//
// �}�b�v�ɔz�u��Ă���d�@�{�b�N�X����[mapobject_electricbox.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAPOBJECT_ELECTRICBOX_H__             //���̃}�N����`������ĂȂ�������
#define _MAPOBJECT_ELECTRICBOX_H__             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "mapobject.h"

// �O���錾
class CElectricBoxState;

//===========================================================
// �S�~���N���X��`
//===========================================================
class CMapObject_ElectricBox : public CMapObject
{
public:
	CMapObject_ElectricBox();
	CMapObject_ElectricBox(const char* aModelFliename, int nPriority = 3);
	~CMapObject_ElectricBox();

	HRESULT Init(void);           // ����������    
	void Uninit(void);            // �I������
	void Update(void);            // �X�V����
	void Draw(void);              // �`�揈��

	static CMapObject_ElectricBox* Create(const char* aModelFliename, int nPriority = 3);  //����
	void ChangeState(CElectricBoxState* pState);  // �X�e�C�g��ύX

	// �擾
	CMapObject_ElectricBox* GetTop(void) { return m_pTop; }  // ���X�g�̐擪
	int GetFallDownCount(void) { return m_nFallDownCount; }

	// �ݒ�
	void SetFallDown(int nNum) { m_nFallDownCount = nNum; }
	void IsFallDown(bool bValue) { m_bFallDown = bValue; }

private:

	CElectricBoxState* m_pState;  // �X�e�C�g�̃|�C���^

	static CMapObject_ElectricBox* m_pTop;  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CMapObject_ElectricBox* m_pCur;  // �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_ElectricBox* m_pNext;        // ���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_ElectricBox* m_pPrev;        // �O�̃I�u�W�F�N�g�ւ̃|�C���^

	int m_nFallDownCount = 0;
	bool m_bFallDown = false;
};

//===========================================================
// �X�e�C�g
//===========================================================
class CElectricBoxState
{
public:
	CElectricBoxState() {}
	~CElectricBoxState() {}

	virtual void Update(CMapObject_ElectricBox* pElectricBox) = 0;

private:

};

// �j���[�g����
class CElectricNeutral : public CElectricBoxState
{
public:
	CElectricNeutral();
	~CElectricNeutral() {}

	void Update(CMapObject_ElectricBox* pElectricBox) override;

private:

};

// �|���
class CElectricFallDown : public CElectricBoxState
{
public:
	CElectricFallDown();
	~CElectricFallDown() {}

	void Update(CMapObject_ElectricBox* pElectricBox) override;

private:

	D3DXVECTOR3 PlayerPos;

};

// ����ꂽ
class CElectrictBeaten : public CElectricBoxState
{
public:
	CElectrictBeaten();
	~CElectrictBeaten() {}

	void Update(CMapObject_ElectricBox* pElectricBox) override;

private:

	int m_nShakeTimeCounter;
	float m_fShakeAngle;
};

#endif