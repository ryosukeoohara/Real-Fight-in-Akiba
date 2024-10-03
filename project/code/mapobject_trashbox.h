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

// �O���錾
class CTrashBoxState;

//===========================================================
// �S�~���N���X��`
//===========================================================
class CMapObject_TrashBox : public CObjectX
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

private:

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