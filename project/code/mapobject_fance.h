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


// �O���錾
class CFanceState;

//===========================================================
// ��N���X��`
//===========================================================
class CMapObject_Fance : public CObjectX
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

private:

};

//===========================================================
// �X�e�C�g
//===========================================================
class CFanceState
{
public:
	CFanceState() {}
	~CFanceState() {}

	virtual void Update(CFanceState* pFance) = 0;

private:

};

// �������
class CFanceBlowAway : public CFanceState
{
public:
	CFanceBlowAway();
	~CFanceBlowAway() {}

	void Update(CFanceState* pFance) override;

private:

};

// �o�E���h
class CFanceFallDown : public CFanceState
{
public:
	CFanceFallDown();
	~CFanceFallDown() {}

	void Update(CFanceState* pFance) override;

private:

};

#endif