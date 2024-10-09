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
#include "mapobject.h"

// �O���錾
class CCanState;

//===========================================================
// �ʃN���X��`
//===========================================================
class CMapObject_Can : public CMapObject
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
	void ChangeState(CCanState* pState);
	static CMapObject_Can* GetInstance(void) { return m_pInstance; }  // �������g

	// �擾
	CMapObject_Can* GetTop(void) { return m_pTop; }  // ���X�g�̐擪

private:

	static CMapObject_Can* m_pInstance;
	CCanState* m_pState;

	static CMapObject_Can* m_pTop;  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CMapObject_Can* m_pCur;  // �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_Can* m_pNext;        // ���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_Can* m_pPrev;        // �O�̃I�u�W�F�N�g�ւ̃|�C���^

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

	D3DXVECTOR3 m_move;

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