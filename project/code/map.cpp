//===========================================================
//
// �}�b�v�̃��f��[map.cpp]
// Author �匴�叫
//
//===========================================================
#include "renderer.h"
#include "manager.h"
#include "map.h"
#include "objectX.h"
#include "debugproc.h"
#include "object.h"
#include "mapobject.h"
#include "mapobject_can.h"
#include "mapobject_dramcan.h"
#include "mapobject_fance.h"
#include "mapobject_trashbox.h"
#include "mapobject_electricbox.h"
#include "mapobject_light.h"
#include "mapobject_sign.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CMap *CMap::m_pMap = nullptr;
CMapObject* CMap::m_pTop = nullptr;
CMapObject* CMap::m_pCur = nullptr;
CObjectX* CMap::m_pObjXTop = nullptr;
CObjectX* CMap::m_pObjXCur = nullptr;

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const char* MAPTEXT = "data\\TEXT\\set_map_model.txt";  // �}�b�v�̃e�L�X�g�t�@�C��
	const char* MAP_OBJECCT_TEXT = "data\\TEXT\\model_set.txt";  // �}�b�v�̃e�L�X�g�t�@�C��
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CMap::CMap()
{
	m_nNumModel = 0;
	m_nNumItem = 0;

	m_pMap = this;
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CMap::~CMap()
{

}

//===========================================================
// ����������
//===========================================================
HRESULT CMap::Init(void)
{
	// �}�b�v�̃e�L�X�g�t�@�C���Ǎ�
	TextLoad();

	// �}�b�v�̃e�L�X�g�t�@�C���Ǎ�
	MapObjectLoad();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMap::Uninit(void)
{
	/*if (m_appObjectX != nullptr)
	{
		for (int nCount = 0; nCount < m_nNumModel; nCount++)
		{
			m_appObjectX[nCount]->Uninit();
			m_appObjectX[nCount] = nullptr;
		}

		delete m_appObjectX;
		m_appObjectX = nullptr;
	}*/
}

//===========================================================
// �X�V����
//===========================================================
void CMap::Update(void)
{
	// �}�b�v�I�u�W�F�N�g�̐擪�A�h���X���擾
	CMapObject* pMapObject = GetTop();

	while (pMapObject != nullptr)
	{
		CMapObject* pNext = pMapObject->GetNext();

		//pMapObject->Update();

		pMapObject = pNext;
	}

}

//===========================================================
// �`�揈��
//===========================================================
void CMap::Draw(void)
{
	// �}�b�v�I�u�W�F�N�g�̐擪�A�h���X���擾
	CMapObject* pMapObject = GetTop();

	while (pMapObject != nullptr)
	{
		CMapObject* pNext = pMapObject->GetNext();

		//pMapObject->Draw();

		pMapObject = pNext;
	}
}

//===========================================================
// ��������
//===========================================================
CMap *CMap::Create(void)
{
	// �}�b�v�̃|�C���^
	CMap *pMap = nullptr;

	if (pMap == nullptr)
	{
		// ��������
		pMap = new CMap();

		// ����������
		pMap->Init();
	}

	return pMap;
}

//===========================================================
// �e�L�X�g�t�@�C���Ǎ�����
//===========================================================
void CMap::TextLoad(void)
{
	char aString[128] = {};    // �����ǂݎ��p
	char aComment[128] = {};   // �e�L�X�g�t�@�C�����̃R�����g�ǂݎ��p
	char aFileName[128] = {};  // �t�@�C���̖��O�ǂݎ��p
	int nCntItem = 0;
	int nCntFileName = 0;
	int nType = -1;
	D3DXVECTOR3 pos, rot;      // �ǂݎ��p

	FILE *pFile;   //�t�@�C���|�C���^��錾

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

					CObjectX* pObjectX = CObjectX::Create(m_aTex[nType].aName);
					pObjectX->SetPosition(pos);
					pObjectX->SetRotition(rot);
					pObjectX->SetIdxModel(nType);

					if (/*nType >= 43 && nType <= 51 || */nType == 56)
					{
						pObjectX->SetbEnable(false);
					}
					else
					{
						pObjectX->SetbEnable(true);
					}

					if (m_pObjXTop != nullptr)
					{// �擪�����݂��Ă���ꍇ

						m_pObjXCur->SetNext(pObjectX);
						m_pObjXTop->SetPrev(m_pObjXCur);
						m_pObjXCur = pObjectX;
					}
					else
					{// ���݂��Ȃ��ꍇ

						m_pObjXTop = pObjectX;
						m_pObjXCur = pObjectX;
					}

					//if (m_pObjXTop != nullptr)
					//{// �擪�����݂��Ă���ꍇ

					//	m_pCur[nPriority]->m_pNext = this;
					//	m_pPrev = m_pCur[nPriority];
					//	m_pCur[nPriority] = this;
					//}
					//else
					//{// ���݂��Ȃ��ꍇ

					//	m_pObjXTop = pObjectX;
					//	m_pObjXCur = pObjectX;
					//}

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

//===========================================================
// �e�L�X�g�t�@�C���Ǎ�����
//===========================================================
void CMap::MapObjectLoad(void)
{
	char aString[128] = {};    // �����ǂݎ��p
	char aComment[128] = {};   // �e�L�X�g�t�@�C�����̃R�����g�ǂݎ��p
	char aFileName[128] = {};  // �t�@�C���̖��O�ǂݎ��p
	int nCntItem = 0;
	int nCntFileName = 0;
	int nType = -1;
	D3DXVECTOR3 pos, rot;      // �ǂݎ��p

	FILE* pFile;   //�t�@�C���|�C���^��錾

	pFile = fopen(MAP_OBJECCT_TEXT, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		fscanf(pFile, "%s", &aString[0]);

		if (strcmp("SCRIPT", aString) == 0)
		{
			while (strcmp("END_SCRIPT", aString) != 0)
			{
				fscanf(pFile, "%s", &aString[0]);

				if (strcmp("NUM_MODEL", aString) == 0)
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

					CMapObject* pMapObject = nullptr;

					if (nType == 0)
					{
						pMapObject = CMapObject_Fance::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
							pMapObject->SetVtx();
						}
					}

					if (nType == 1)
					{
						pMapObject = CMapObject_TrashBox::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
							pMapObject->SetVtx();
						}
					}

					if (nType == 2)
					{
						pMapObject = CMapObject_Can::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
						}
					}

					if (nType == 3)
					{
						pMapObject = CMapObject_ElectricBox::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
							pMapObject->SetVtx();
						}
					}

					if (nType == 4)
					{
						pMapObject = CMapObject_Dramcan::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
						}
					}

					if (nType == 6)
					{
						pMapObject = CMapObject_Sign::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
						}
					}

					if (nType == 5)
					{
						pMapObject = CMapObject_Light::Create(m_aTex[nType].aName);

						if (pMapObject != nullptr)
						{
							pMapObject->SetPosition(pos);
							pMapObject->SetRotition(rot);
							pMapObject->SetIdxModel(nType);
							pMapObject->SetbEnable(true);
							pMapObject->SetVtx();
						}
					}

					if (m_pTop != nullptr)
					{// �擪�����݂��Ă���ꍇ

						m_pCur->SetNext(pMapObject);
						m_pTop->SetPrev(m_pCur);
						m_pCur = pMapObject;
					}
					else
					{// ���݂��Ȃ��ꍇ

						m_pTop = pMapObject;
						m_pCur = pMapObject;
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
