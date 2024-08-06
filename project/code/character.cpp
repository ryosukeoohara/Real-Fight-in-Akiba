//=============================================================================
//
// �L�����N�^�[ [character.cpp]
// Author : �匴�@�叫
//
//=============================================================================
#include "main.h"
#include "character.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "object.h"
#include "texture.h"

//================================================================
//�R���X�g���N�^
//================================================================
CCharacter::CCharacter()
{
	//�l���N���A
	m_pTexture = NULL;  //�e�N�X�`���ւ̃|�C���^
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_pTexture = NULL;
	m_pCurrent = nullptr;
	m_dwNumMat = 0;
	m_nNumModel = 0;
	m_vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	m_vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	//m_nIdxTexture = -1;
}

//================================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//================================================================
CCharacter::CCharacter(const char *aModelFliename)
{
	//�l���N���A
	m_pTexture = NULL;  //�e�N�X�`���ւ̃|�C���^
	m_pParent = NULL;
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_pTexture = NULL;
	m_pCurrent = nullptr;
	m_dwNumMat = 0;
	m_nNumModel = 0;
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	m_vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	m_Fliename = aModelFliename;
	//m_nIdxTexture = -1;
}

//================================================================
//�f�X�g���N�^
//================================================================
CCharacter::~CCharacter()
{

}

//================================================================
//��������
//================================================================
CCharacter *CCharacter::Create(const char *aModelFliename)
{
	//�I�u�W�F�N�g2D�̃|�C���^
	CCharacter *pCharacter = nullptr;

	if (pCharacter == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pCharacter = new CCharacter(aModelFliename);

		//����������
		pCharacter->Init();
	}
	
	return pCharacter;
}

//================================================================
//�|���S���̏���������
//================================================================
HRESULT CCharacter::Init(void)
{
	int nNumVtx;      //���_��
	DWORD dwSizeFVF;  //���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;   //���_�o�b�t�@�ւ̃|�C���^

	//�f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�e�N�X�`���̏��擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//X�t�@�C���ǂݍ���
	D3DXLoadMeshFromX(m_Fliename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	//���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//�}�e���A���ւ̃|�C���^
	D3DXMATERIAL *pMat;

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	m_nIdxTexture = new int[(int)m_dwNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`�������݂���

			m_nIdxTexture[nCntMat] = pTexture->Regist(pMat[nCntMat].pTextureFilename);
		}
		else
		{
			m_nIdxTexture[nCntMat] = -1;
		}
	}

	//���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;     //���_���W�̑��

		//�ŏ��l-------------------------------------------------
		if (m_vtxMini.x > vtx.x)
		{
			m_vtxMini.x = vtx.x;
		}

		if (m_vtxMini.y > vtx.y)
		{
			m_vtxMini.y = vtx.y;
		}

		if (m_vtxMini.z > vtx.z)
		{
			m_vtxMini.z = vtx.z;
		}

		//�ő�l-------------------------------------------------
		if (m_vtxMax.x < vtx.x)
		{
			m_vtxMax.x = vtx.x;
		}

		if (m_vtxMax.y < vtx.y)
		{
			m_vtxMax.y = vtx.y;
		}

		if (m_vtxMax.z < vtx.z)
		{
			m_vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;    //���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//================================================================
//�|���S���̏I������
//================================================================
void CCharacter::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		//delete m_pTexture;
		m_pTexture = NULL;
	}

	//�e�N�X�`���̏���
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//================================================================
//�|���S���̍X�V����
//================================================================
void CCharacter::Update(void)
{
	
}

//================================================================
//�|���S���̕`�揈��
//================================================================
void CCharacter::Draw(void)
{
	//�f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�e�N�X�`���̏��擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX m_mtxRot, m_mtxTrans;

	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;

	//�e�̃}�g���b�N�X
	D3DXMATRIX mtxParent;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	if (m_pCurrent != nullptr)
	{
		//�ʒu�𔽉f
		D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);

		// �}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld, m_pCurrent);
	}
	else
	{
		//�ʒu�𔽉f
		D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);
	}
	

	//�p�[�c�̐e�̃}�g���b�N�X��ݒ�
	if (m_pParent != NULL)
	{//�e���f��������ꍇ

		mtxParent = *m_pParent->GetMtxWorld();
	}
	else
	{//�e���f�����Ȃ��ꍇ

		//���݂̃}�g���b�N�X���擾����
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture[nCntMat]));

		//���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	D3DXMATRIX mtxShadow;
	D3DLIGHT9 light;
	D3DXVECTOR4 posLight;
	D3DXVECTOR3 pos, normal;
	D3DXPLANE plane;

	// ���C�g�̈ʒu��ݒ�
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	// ���ʏ���ݒ�
	pos = D3DXVECTOR3(m_mtxWorld._41, 0.5f, m_mtxWorld._43);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// �V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	// �V���h�E�}�g���b�N�X�̍쐬
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		m_ShadowMat.Emissive.r = 0.0f;
		m_ShadowMat.Emissive.g = 0.0f;
		m_ShadowMat.Emissive.b = 0.0f;
		m_ShadowMat.Emissive.a = 1.0f;
		m_ShadowMat.Diffuse.r = 0.0f;
		m_ShadowMat.Diffuse.g = 0.0f;
		m_ShadowMat.Diffuse.b = 0.0f;
		m_ShadowMat.Diffuse.a = 1.0f;

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_ShadowMat);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, nullptr);

		//���f��(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//================================================================
//�e�N�X�`���擾
//================================================================
void CCharacter::BindTexture(LPDIRECT3DTEXTURE9 *m_Texture)
{
	m_pTexture = m_Texture;
}