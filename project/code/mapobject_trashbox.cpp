//===========================================================
//
// �}�b�v�ɔz�u��Ă���S�~������[mapobject_trashbox.cpp]
// Author �匴�叫
//
//===========================================================
#include "mapobject_trashbox.h"

//===========================================================
// �R���X�g���N�^
//===========================================================
CMapObject_TrashBox::CMapObject_TrashBox()
{

}

//===========================================================
// �R���X�g���N�^
//===========================================================
CMapObject_TrashBox::CMapObject_TrashBox(const char* aModelFliename, int nPriority) : CObjectX(aModelFliename, nPriority)
{

}

//===========================================================
// �f�X�g���N�^
//===========================================================
CMapObject_TrashBox::~CMapObject_TrashBox()
{

}

//===========================================================
// ����������
//===========================================================
HRESULT CMapObject_TrashBox::Init(void)
{
	CObjectX::Init();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CMapObject_TrashBox::Uninit(void)
{
	CObjectX::Uninit();
}

//===========================================================
// �X�V����
//===========================================================
void CMapObject_TrashBox::Update(void)
{
}

//===========================================================
// �`�揈��
//===========================================================
void CMapObject_TrashBox::Draw(void)
{
	CObjectX::Draw();
}

//===========================================================
// ��������
//===========================================================
CMapObject_TrashBox* CMapObject_TrashBox::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_TrashBox* pTrashBox = new CMapObject_TrashBox(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}