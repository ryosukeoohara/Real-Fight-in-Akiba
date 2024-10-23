//===========================================================
//
// �}�b�v�̃��f��[mapobject.cpp]
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
#include "mapobject_electricbox.h"

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
	CObjectX::Init();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject::Uninit(void)
{
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