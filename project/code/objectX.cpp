//===========================================================
//
// ���f������[objectX.cpp]
// Author : �匴�@�叫
//
//===========================================================
#include "main.h"
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "texture.h"
#include "player.h"

//===========================================================
// �R���X�g���N�^
//===========================================================
CObjectX::CObjectX()
{
	//�l���N���A
	m_Info.vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_Info.vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	m_Info.vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_Info.vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);

	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_Info.Fliename = "a";
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	m_bDraw = true;
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CObjectX::CObjectX(const char *aModelFliename, int nPriority) : CObject(nPriority)
{
	//�l���N���A
	m_Info.vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_Info.vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	m_Info.vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_Info.vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);

	m_Info.Fliename = aModelFliename;
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	m_bDraw = true;
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CObjectX::~CObjectX()
{

}

//===========================================================
// ��������
//===========================================================
CObjectX *CObjectX::Create(const char *aModelFliename, int nPriority)
{
	//�I�u�W�F�N�gX�̃|�C���^
	CObjectX *pObjectX = NULL;

	//���f�������擾
	CModel *pModel = CManager::GetInstance()->GetModel();

	if (pObjectX == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pObjectX = new CObjectX(aModelFliename, nPriority);

		//����������
		pObjectX->Init();
	}
	
	return pObjectX;
}

//===========================================================
// �ʂƃS�~�����R���Đ�����ԏ���
//===========================================================
void CObjectX::Shoot(void)
{
	// �v���C���[�ɏR���Ă���
	if (m_bShut)
	{
		D3DXVECTOR3 rot = CPlayer::GetInstance()->GetRotition();
		D3DXVECTOR3 move = CPlayer::GetInstance()->GetMove();

		m_Info.rot.x = 1.57f;
		m_Info.rot.y = rot.y;
		m_Info.rot.z = 1.57f;

		m_Info.move.x += (move.x * 1.2f);
		m_Info.move.y += 1.0f;
		m_Info.move.z += (move.z * 1.2f);
	}
}

//===========================================================
// �򂪓|��鏈��
//===========================================================
void CObjectX::FallDown(void)
{
	if (m_Info.rot.x >= 1.3f || m_Info.rot.x <= -1.3f)
	{
		m_bDown = false;

		m_fFallDownSpeed = 0.0f;
	}

	D3DXVECTOR3 move = {};

	// ��ɐU�ꂽ�u�Ԃ����ʂ�
	if (m_bDown)
	{
		move = CPlayer::GetInstance()->GetMove();

		m_fFallDownSpeed = (move.x + move.x) - (move.z + move.z);
	}

	if (m_fFallDownSpeed >= 8.0f || m_fFallDownSpeed <= -8.0f)
	{// �v���C���[�̑��x������葬��������

		move = CPlayer::GetInstance()->GetMove();
		m_Info.move.x += move.x;
		m_Info.move.z += move.z;
	}

	m_Info.rot.x -= (m_fFallDownSpeed * 0.01f);
}

//===========================================================
// �v���C���[�̍U�����q�b�g�����Ƃ��h�炷����
//===========================================================
void CObjectX::HitAttack(void)
{
	// �J������h�炷�������s�����ǂ����̃t���O�������Ă�����J������h�炷�������s��
	if (m_bHitAttack)
	{
		// sin ���g�p�����h�炵���W�̎Z�o
		m_Info.move.y = 0.0f;
			
		m_Info.move.x = sinf(ShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter /
			60)) * 2.0f;

		m_Info.move.z = sinf(ShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter /
			60)) * 2.0f;

		// �h�炵�����Ɏg�p���� sin �ɓn���p�x�̕ύX����
		ShakeAngle += 2.0f * 1.0f;

		// �h�炷���Ԃ��o�߂�����h�炵�������I������
		m_nShakeTimeCounter += 1;
		if (m_nShakeTimeCounter >= 60)
		{
			m_bHitAttack = false;
		}
	}
	else
	{
		// �h�炳��Ă��Ȃ��ꍇ�͗h�炵�����ɂ����Z���W���O�ɂ���
		//m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nShakeTimeCounter = 0;
	}
}

//===========================================================
// ����������
//===========================================================
HRESULT CObjectX::Init(void)
{
	int nNumVtx;      //���_��
	DWORD dwSizeFVF;  //���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;   //���_�o�b�t�@�ւ̃|�C���^

	//�f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//X�t�@�C���ǂݍ���
	D3DXLoadMeshFromX(m_Info.Fliename,
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

	//�e�N�X�`���̏��擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

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
		if (m_Info.vtxMini.x > vtx.x)
		{
			m_Info.vtxMini.x = vtx.x;
		}

		if (m_Info.vtxMini.y > vtx.y)
		{
			m_Info.vtxMini.y = vtx.y;
		}

		if (m_Info.vtxMini.z > vtx.z)
		{
			m_Info.vtxMini.z = vtx.z;
		}

		//�ő�l-------------------------------------------------
		if (m_Info.vtxMax.x < vtx.x)
		{
			m_Info.vtxMax.x = vtx.x;
		}

		if (m_Info.vtxMax.y < vtx.y)
		{
			m_Info.vtxMax.y = vtx.y;
		}

		if (m_Info.vtxMax.z < vtx.z)
		{
			m_Info.vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;    //���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CObjectX::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		delete m_pTexture;
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

	//�I�u�W�F�N�g�̔j��
	Release();
}

//===========================================================
// �X�V����
//===========================================================
void CObjectX::Update(void)
{
	if (m_bShut)
	{
		int i = 0l;
	}

	if (m_Info.pos.y - m_Info.vtxMax.x >= 0.0f)
	{
		m_Info.pos.y -= 1.0f;
		m_bShut = false;
	}

	if (m_Info.pos.y <= 0.0f)
	{
		m_Info.pos.y = 1.0f;
		m_bShut = false;
	}

	// �v���C���[�ɏR���Đ�����ԏ���
	Shoot();

	// �v���C���[����ɐG��ē|���
	FallDown();

	// 
	HitAttack();

	m_Info.pos += m_Info.move;

	// �ړ��ʂ��X�V(����������)
	m_Info.move.x += (0.0f - m_Info.move.x) * 0.1f;
	m_Info.move.y += (0.0f - m_Info.move.y) * 0.1f;
	m_Info.move.z += (0.0f - m_Info.move.z) * 0.1f;
}

//===========================================================
// �`�揈��
//===========================================================
void CObjectX::Draw(void)
{
	//�f�o�C�X�̎擾
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX m_mtxRot, m_mtxTrans;

	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL* pMat;

	// �`�������
	if (m_bDraw)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_Info.mtxWorld);

		if (m_pCurrent != nullptr)
		{
			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &m_mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&m_mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &m_mtxTrans);

			// �}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
			D3DXMatrixMultiply(&m_Info.mtxWorld,
				&m_Info.mtxWorld, m_pCurrent);
		}
		else
		{
			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &m_mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&m_mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &m_mtxTrans);
		}

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_Info.mtxWorld);

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
	}
}