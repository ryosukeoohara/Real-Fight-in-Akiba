//===========================================================
//
// �}�b�v�ɔz�u��Ă���h�����ʏ���[mapobject_dramcan.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAPOBJECT_DRAMCAN_H__             //���̃}�N����`������ĂȂ�������
#define _MAPOBJECT_DRAMCAN_H__             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// �O���錾
class CDramcanState;
class CObject3D;

//===========================================================
// �h�����ʃN���X��`
//===========================================================
class CMapObject_Dramcan : public CMapObject
{
public:
	CMapObject_Dramcan();
	CMapObject_Dramcan(const char* aModelFliename, int nPriority = 3);
	~CMapObject_Dramcan();

	HRESULT Init(void);           // ����������    
	void Uninit(void);            // �I������
	void Update(void);            // �X�V����
	void Draw(void);              // �`�揈��

	static CMapObject_Dramcan* Create(const char* aModelFliename, int nPriority = 3);  //����
	void ChangeState(CDramcanState* pState);  // �X�e�C�g��ύX

	// �擾
	//static CMapObject_Dramcan* GetInstance(void) { return m_pDramcan; }
	CMapObject_Dramcan* GetTop(void) { return m_pTop; }  // ���X�g�̐擪

private:

	//static CMapObject_Dramcan* m_pDramcan;
	CDramcanState* m_pState;  // �X�e�C�g�̃|�C���^

	static CMapObject_Dramcan* m_pTop;  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CMapObject_Dramcan* m_pCur;  // �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_Dramcan* m_pNext;        // ���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_Dramcan* m_pPrev;        // �O�̃I�u�W�F�N�g�ւ̃|�C���^
};

//===========================================================
// �X�e�C�g
//===========================================================
class CDramcanState
{
public:
	CDramcanState() {}
	~CDramcanState() {}

	virtual void Update(CMapObject_Dramcan* pDramcan) = 0;

private:

};

// �j���[�g����
class CDramcanNeutral : public CDramcanState
{
public:
	CDramcanNeutral();
	~CDramcanNeutral() {}

	void Update(CMapObject_Dramcan* pDramcan) override;

private:

};

// �������
class CDramcanBlowAway : public CDramcanState
{
public:
	CDramcanBlowAway();
	~CDramcanBlowAway() {}

	void Update(CMapObject_Dramcan* pDramcan) override;

private:

};

// �h�����ʂ��琅�������
class CDramcanWaterLeak : public CDramcanState
{
public:
	CDramcanWaterLeak();
	~CDramcanWaterLeak() {}

	void Update(CMapObject_Dramcan* pDramcan) override;

private:

	CObject3D* m_pPuddle = nullptr;

};

#endif