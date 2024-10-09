//===========================================================
//
// �}�b�v�̃��f��[map.cpp]
// Author �匴�叫
//
//===========================================================
#include "mapobject.h"
#include "objectX.h"
#include "debugproc.h"
#include "mapobject_can.h"
#include "mapobject_trashbox.h"
#include "mapobject_fance.h"
#include "mapobject_dramcan.h"
#include "mapobject_light.h"
#include "mapobject_sign.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CMapObject* CMapObject::m_pMap = nullptr;


//===========================================================
// �萔��`
//===========================================================
namespace
{
	const char* MAPTEXT = "data\\TEXT\\model_set.txt";  // �}�b�v�̃e�L�X�g�t�@�C��
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CMapObject::CMapObject()
{
	m_nNumModel = 0;
	m_nNumItem = 0;

	m_pMap = this;
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CMapObject::CMapObject(const char* aModelFliename, int nPriority) : CObjectX(aModelFliename, nPriority)
{
	m_nNumModel = 0;
	m_nNumItem = 0;

	m_pMap = this;
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CMapObject::~CMapObject()
{

}

//===========================================================
// ����������
//===========================================================
HRESULT CMapObject::Init(void)
{
	// �}�b�v�̃e�L�X�g�t�@�C���Ǎ�
	//TextLoad();

	CObjectX::Init();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject::Uninit(void)
{
	// ���g�����X�g�������
	//ListOut();

	CObjectX::Uninit();
}

//===========================================================
// �X�V����
//===========================================================
void CMapObject::Update(void)
{
	CObjectX::Update();
}

//===========================================================
// �`�揈��
//===========================================================
void CMapObject::Draw(void)
{
	CObjectX::Draw();
}

//===========================================================
// ��������
//===========================================================
CMapObject* CMapObject::Create(void)
{
	// �}�b�v�̃|�C���^
	CMapObject* pMap = nullptr;

	if (pMap == nullptr)
	{
		// ��������
		pMap = new CMapObject();

		// ����������
		pMap->Init();
	}

	return pMap;
}

////===========================================================
//// ���X�g���玩������������
////===========================================================
//void CMapObject::ListOut(void)
//{
//	if (m_pTop == this)
//	{// ���g���擪
//
//		if (m_pNext != nullptr)
//		{// �������݂��Ă���
//
//			m_pTop = m_pNext;	// ����擪�ɂ���
//			m_pNext->m_pPrev = nullptr;	// ���̑O�̃|�C���^���o���Ă��Ȃ��悤�ɂ���
//		}
//		else
//		{// ���݂��Ă��Ȃ�
//
//			m_pTop = nullptr;	// �擪���Ȃ���Ԃɂ���
//			m_pCur = nullptr;	// �Ō�����Ȃ���Ԃɂ���
//		}
//	}
//	else if (m_pCur == this)
//	{// ���g���Ō��
//
//		if (m_pPrev != nullptr)
//		{// �������݂��Ă���
//
//			m_pCur = m_pPrev;			// �O���Ō���ɂ���
//			m_pPrev->m_pNext = nullptr;	// �O�̎��̃|�C���^���o���Ă��Ȃ��悤�ɂ���
//		}
//		else
//		{// ���݂��Ă��Ȃ�
//
//			m_pTop = nullptr;	// �擪���Ȃ���Ԃɂ���
//			m_pCur = nullptr;	// �Ō�����Ȃ���Ԃɂ���
//		}
//	}
//	else
//	{
//		if (m_pNext != nullptr)
//		{
//			m_pNext->m_pPrev = m_pPrev;	// ���g�̎��ɑO�̃|�C���^���o��������
//		}
//		if (m_pPrev != nullptr)
//		{
//			m_pPrev->m_pNext = m_pNext;	// ���g�̑O�Ɏ��̃|�C���^���o��������
//		}
//	}
//}

//===========================================================
// �e�L�X�g�t�@�C���Ǎ�����
//===========================================================
void CMapObject::TextLoad(void)
{
	char aString[128] = {};    // �����ǂݎ��p
	char aComment[128] = {};   // �e�L�X�g�t�@�C�����̃R�����g�ǂݎ��p
	char aFileName[128] = {};  // �t�@�C���̖��O�ǂݎ��p
	int nCntItem = 0;
	int nCntFileName = 0;
	int nType = -1;
	D3DXVECTOR3 pos, rot;      // �ǂݎ��p

	FILE* pFile;   //�t�@�C���|�C���^��錾

	pFile = fopen(MAPTEXT, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		fscanf(pFile, "%s", &aString[0]);

		if (strcmp("SCRIPT", aString) == 0)
		{
			while (strcmp("END_SCRIPT", aString) != 0)
			{
				fscanf(pFile, "%s", &aString[0]);

				if (strcmp("NUM_ITEM", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);      //=
					fscanf(pFile, "%d", &m_nNumItem);  //���f���̑���

				}  // NUM_ITEM�̂�����

				if (strcmp("MODEL_FILENAME", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);       //=
					fscanf(pFile, "%s", &aFileName[0]);  //���f���̖��O

					strcpy(m_aTex[nCntFileName].aName, aFileName);
					nCntFileName++;

					m_appObjectX = new CObjectX * [MAX_MODEL];

				}  // MODEL_LILENAME�̂�����

				if (strcmp("MODELSET", aString) == 0)
				{
					while (strcmp("END_MODELSET", aString) != 0)
					{
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp("TYPE", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      //=
							fscanf(pFile, "%d", &nType);  //���f���̑���
						}

						if (strcmp("POS", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      //=
							fscanf(pFile, "%f", &pos.x);  //���f���̑���
							fscanf(pFile, "%f", &pos.y);  //���f���̑���
							fscanf(pFile, "%f", &pos.z);  //���f���̑���
						}

						if (strcmp("ROT", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);      //=
							fscanf(pFile, "%f", &rot.x);  //���f���̑���
							fscanf(pFile, "%f", &rot.y);  //���f���̑���
							fscanf(pFile, "%f", &rot.z);  //���f���̑���
						}
					}

					if (nType == 3)
					{
						m_appObjectX[nCntItem] = CObjectX::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
							m_appObjectX[nCntItem]->SetVtx();
						}
					}

					if (nType == 0)
					{
						m_appObjectX[nCntItem] = CMapObject_Fance::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}

					if (nType == 1)
					{
						m_appObjectX[nCntItem] = CMapObject_TrashBox::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}

					if (nType == 2)
					{
						m_appObjectX[nCntItem] = CMapObject_Can::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}

					if (nType == 4)
					{
						m_appObjectX[nCntItem] = CMapObject_Dramcan::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}

					if (nType == 5)
					{
						m_appObjectX[nCntItem] = CMapObject_Sign::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}

					if (nType == 6)
					{
						m_appObjectX[nCntItem] = CMapObject_Light::Create(m_aTex[nType].aName);

						if (m_appObjectX[nCntItem] != nullptr)
						{
							m_appObjectX[nCntItem]->SetPosition(pos);
							m_appObjectX[nCntItem]->SetRotition(rot);
							m_appObjectX[nCntItem]->SetIdxModel(nType);
							m_appObjectX[nCntItem]->SetbEnable(true);
						}
					}
					
					nCntItem++;

					m_nNumModel++;
				}
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{
		return;
	}
}