//===========================================================
//
// �}�b�v�ɔz�u���郂�f��[map.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAPOBJECT_H_             //���̃}�N����`������ĂȂ�������
#define _MAPOBJECT_H_             //2�d�C���N���[�h�h�~�̃}�N����`������

#include "objectX.h"

//===========================================================
// �O���錾
//===========================================================
class CObjectX;

// �}�N����`
#define MAX_MODEL (512)  // ���f���̍ő吔
#define MAX_NAME  (128)  // �e�N�X�`���̍ő啶����

//===========================================================
// �}�b�v�N���X��`
//===========================================================
class CMapObject : public CObjectX
{
public:

	CMapObject();   // �R���X�g���N�^
	CMapObject(const char* aModelFliename, int nPriority = 3);
	~CMapObject();  // �f�X�g���N�^

	HRESULT Init(void);   // ����������    
	void Uninit(void);    // �I������
	void Update(void);    // �X�V����
	void Draw(void);      // �`�揈��

	static CMapObject* Create(void); // ��������

	// �ݒ�n
	void SetNext(CMapObject* pNext) { m_pNext = pNext; }
	void SetPrev(CMapObject* pPrev) { m_pPrev = pPrev; }

	// �擾�n
	int GetNum(void) { return m_nNumModel; }
	static CMapObject* GetInstance(void) { return m_pMap; }
	CMapObject* GetNext(void) { return m_pNext; }
	
private:

	static CMapObject* m_pMap;
	int m_nNumModel;
	int m_nNumItem;
	CMapObject* m_pNext;        // ���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject* m_pPrev;        // �O�̃I�u�W�F�N�g�ւ̃|�C���^
	
	bool m_bDeath;              // ���S�t���O

	bool m_bShut = false;
	bool m_bDown = false;
};

#endif
