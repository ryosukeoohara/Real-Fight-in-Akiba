//===========================================================
//
// �}�b�v�ɔz�u���郂�f��[map.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAP_H_             //���̃}�N����`������ĂȂ�������
#define _MAP_H_             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �O���錾
//===========================================================
class CObjectX;
class CMapObject;

//===========================================================
// �萔��`
//===========================================================
namespace MODEL_FILE
{
	const int MAX_MODEL = 512;  // ���f���̍ő吔
	const int MAX_NAME = 124;   // �t�@�C���p�X�̍ő啶����
}

//===========================================================
// �}�b�v�N���X��`
//===========================================================
class CMap
{
public:

	CMap();   // �R���X�g���N�^
	~CMap();  // �f�X�g���N�^

	HRESULT Init(void);   // ����������    
	void Uninit(void);    // �I������
	void Update(void);    // �X�V����
	void Draw(void);

	static CMap *Create(void); // ��������
	

	// �ݒ�n

	// �擾�n
	CObjectX **GetObjectX(void) { return m_appObjectX; }
	int GetNum(void) { return m_nNumModel; }
	static CMap *GetInstance(void) { return m_pMap; }
	static CMapObject* GetTop(void) { return m_pTop; }  // ���X�g�̐擪
	static CObjectX* GetTopObjX(void) { return m_pObjXTop; }

private:

	struct TEX
	{
		char aName[MODEL_FILE::MAX_NAME];
	} ;
	

	void TextLoad(void);
	void MapObjectLoad(void);

	static CMap* m_pMap;

	static CMapObject* m_pTop;  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CMapObject* m_pCur;  // �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	static CObjectX* m_pObjXTop;  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObjectX* m_pObjXCur;  // �Ō���̃I�u�W�F�N�g�ւ̃|�C���^

	CObjectX **m_appObjectX;
	int m_nNumModel;
	int m_nNumItem;

	TEX m_aTex[MODEL_FILE::MAX_MODEL];
};



#endif