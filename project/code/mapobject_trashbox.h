//===========================================================
//
// �}�b�v�ɔz�u��Ă���S�~������[mapobject_trashbox.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAPOBJECT_TRANSHBOX_H__             //���̃}�N����`������ĂȂ�������
#define _MAPOBJECT_TRANSHBOX_H__             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// �O���錾
class CTrashBoxState;

//===========================================================
// �S�~���N���X��`
//===========================================================
class CMapObject_TrashBox : public CMapObject
{
public:
	CMapObject_TrashBox();
	CMapObject_TrashBox(const char* aModelFliename, int nPriority = 3);
	~CMapObject_TrashBox();

	HRESULT Init(void);           // ����������    
	void Uninit(void);            // �I������
	void Update(void);            // �X�V����
	void Draw(void);              // �`�揈��

	static CMapObject_TrashBox* Create(const char* aModelFliename, int nPriority = 3);  //����
	void ChangeState(CTrashBoxState* pState);  // �X�e�[�g��ύX

	// �擾
	CMapObject_TrashBox* GetTop(void) { return m_pTop; }  // ���X�g�̐擪

private:

	CTrashBoxState* m_pState;  // �X�e�[�g�̃|�C���^

	static CMapObject_TrashBox* m_pTop;  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CMapObject_TrashBox* m_pCur;  // �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_TrashBox* m_pNext;        // ���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_TrashBox* m_pPrev;        // �O�̃I�u�W�F�N�g�ւ̃|�C���^

};

//===========================================================
// �X�e�C�g
//===========================================================
class CTrashBoxState
{
public:
	CTrashBoxState() {}
	~CTrashBoxState() {}

	virtual void Update(CMapObject_TrashBox* pTrashBox) = 0;

private:

};

// �j���[�g����
class CTrashBoxNeutral : public CTrashBoxState
{
public:
	CTrashBoxNeutral();
	~CTrashBoxNeutral() {}

	void Update(CMapObject_TrashBox* pTrashBox) override;

private:

};

// �������
class CTrashBoxBlowAway : public CTrashBoxState
{
public:
	CTrashBoxBlowAway();
	~CTrashBoxBlowAway() {}

	void Update(CMapObject_TrashBox* pTrashBox) override;

private:

};

// �o�E���h
class CTrashBoxBound : public CTrashBoxState
{
public:
	CTrashBoxBound();
	~CTrashBoxBound() {}

	void Update(CMapObject_TrashBox* pTrashBox) override;

private:

};

#endif