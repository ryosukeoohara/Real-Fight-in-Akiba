//===========================================================
//
// �}�b�v�ɔz�u���郂�f��[map.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAPOBJECT_H_             //���̃}�N����`������ĂȂ�������
#define _MAPOBJECT_H_             //2�d�C���N���[�h�h�~�̃}�N����`������

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
class CMapObject
{
public:

	CMapObject();   // �R���X�g���N�^
	~CMapObject();  // �f�X�g���N�^

	HRESULT Init(void);   // ����������    
	void Uninit(void);    // �I������
	void Update(void);    // �X�V����

	static CMapObject* Create(void); // ��������


	// �ݒ�n

	// �擾�n
	CObjectX** GetObjectX(void) { return m_appObjectX; }
	int GetNum(void) { return m_nNumModel; }
	static CMapObject* GetInstance(void) { return m_pMap; }

private:

	struct TEX
	{
		char aName[MAX_NAME];
	};


	void TextLoad(void);

	static CMapObject* m_pMap;
	CObjectX** m_appObjectX;
	int m_nNumModel;
	int m_nNumItem;

	TEX m_aTex[MAX_MODEL];
};

#endif