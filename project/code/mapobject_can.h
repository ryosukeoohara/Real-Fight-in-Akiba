//===========================================================
//
// �}�b�v�ɔz�u��Ă���ʏ���[mapobject_can.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAPOBJECT_CAN_H__             //���̃}�N����`������ĂȂ�������
#define _MAPOBJECT_CAN_H__             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "objectX.h"

// �O���錾
class CCanState;

//===========================================================
// �ʃN���X��`
//===========================================================
class CMapObject_Can : public CObjectX
{
public:
	CMapObject_Can();
	CMapObject_Can(const char* aModelFliename, int nPriority = 3);
	~CMapObject_Can();

	HRESULT Init(void);           // ����������    
	void Uninit(void);            // �I������
	void Update(void);            // �X�V����
	void Draw(void);              // �`�揈��

	static CMapObject_Can* Create(const char* aModelFliename, int nPriority = 3);  //����
	
private:

};

//===========================================================
// �X�e�C�g
//===========================================================
class CCanState
{
public:
	CCanState() {}
	~CCanState() {}

	virtual void Update(CMapObject_Can* pCan) = 0;

private:

};

// �������
class CCanBlowAway : public CCanState
{
public:
	CCanBlowAway();
	~CCanBlowAway() {}

	void Update(CMapObject_Can* pCan) override;

private:

};

// �o�E���h
class CCanBound : public CCanState
{
public:
	CCanBound();
	~CCanBound() {}

	void Update(CMapObject_Can* pCan) override;

private:

};

#endif