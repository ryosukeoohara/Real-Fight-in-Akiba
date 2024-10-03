//=============================================================================
//
// �V���h�E���� [shadow.cpp]
// Author : �匴�@�叫
//
//=============================================================================
//
//#include "main.h"
//#include "shadow.h"
//#include "input.h"
//#include "player.h"
//
////�}�N����`
//#define MAX_SHADOW      (128)
//
////�O���[�o���ϐ�
//LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;       //�e�N�X�`���ւ̃|�C���^
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;        //���_�o�b�t�@�ւ̃|�C���^
//D3DXMATRIX g_mtxWorldShadow;    //���[���h�}�g���b�N�X
//SHADOW g_Shadow;
//
//
////================================================================
////�V���h�E�̏���������
////================================================================
//void InitShadow(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	PLAYER *pPlayer = GetPlayer();
//
//	////�e�N�X�`���̓ǂݍ���1
//	D3DXCreateTextureFromFile(pDevice,
//		"data\\TEXTURE\\shadow000.jpg",
//		&g_pTextureShadow
//	);
//
//	//���_�o�b�t�@�𐶐�
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_3D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffShadow,
//		NULL);
//
//	//�ʒu�A������������
//	
//		g_Shadow.pos = D3DXVECTOR3(0.0f, 0.01f, 0.0f);
//		g_Shadow.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	
//	
//
//	VERTEX_3D * pVtx;     //���_���ւ̃|�C���^
//
//	//���_�o�b�t�@�����b�N���A���_���փ|�C���^���擾
//	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
//
//	
//		//���_���W�̐ݒ�
//		pVtx[0].pos = D3DXVECTOR3(-5.0f, 0.0f, 5.0f);
//		pVtx[1].pos = D3DXVECTOR3(5.0f, 0.0f, 5.0f);
//		pVtx[2].pos = D3DXVECTOR3(-5.0f, 0.0f, -5.0f);
//		pVtx[3].pos = D3DXVECTOR3(5.0f, 0.0f, -5.0f);
//
//		//�@���x�N�g���̐ݒ�
//		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//		//���_�J���[
//		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//
//		//�e�N�X�`�����W�̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//		//���_�f�[�^�̃|�C���^��4���i�߂�
//		pVtx += 4;
//	
//
//	//���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffShadow->Unlock();
//}
//
////================================================================
////�V���h�E�̏I������
////================================================================
//void UninitShadow(void)
//{
//	//�e�N�X�`���̏���
//	if (g_pTextureShadow != NULL)
//	{
//		g_pTextureShadow->Release();
//		g_pTextureShadow = NULL;
//	}
//
//	//���_�o�b�t�@�̔j��
//	if (g_pVtxBuffShadow != NULL)
//	{
//		g_pVtxBuffShadow->Release();
//		g_pVtxBuffShadow = NULL;
//	}
//}
//
////================================================================
////�V���h�E�̍X�V����
////================================================================
//void UpdateShadow(void)
//{
//
//}
//
////================================================================
////�V���h�E�̕`�揈��
////================================================================
//void DrawShadow(void)
//{
//	//�f�o�C�X�̎擾
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	//�v�Z�p�}�g���b�N�X
//	D3DXMATRIX mtxRot, mtxTrans;
//
//	//���Z�����̐ݒ�
//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//
//	//���[���h�}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&g_mtxWorldShadow);
//
//	//�����𔽉f
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Shadow.rot.y, g_Shadow.rot.x, g_Shadow.rot.z);
//
//	D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);
//
//	//�ʒu�𔽉f
//	D3DXMatrixTranslation(&mtxTrans, g_Shadow.pos.x, g_Shadow.pos.y, g_Shadow.pos.z);
//
//	D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);
//
//	//���[���h�}�g���b�N�X�̐ݒ�
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);
//
//	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//	pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));
//
//	//���_�t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_3D);
//
//	//�e�N�X�`���̐ݒ�
//	pDevice->SetTexture(0, g_pTextureShadow);
//
//	//�|���S���̕`��
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,    //�v���~�e�B�u�̎��
//		0,                      //�`�悷��ŏ��̒��_�C���f�b�N�X
//		2                       //�`�悷��v���~�e�B�u��
//	);
//
//	//�ʏ�̍����ɖ߂�
//	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//}
//
//int SetShadow(void)
//{
//	PLAYER *pPlayer = GetPlayer();
//	SHADOW *pShadow = GetShadow();
//	Bullet *pBullet = GetBullet();
//
//	int nCntShadow = -1;
//
//	for (int nCount = 0; nCount < MAX_SHADOW; nCount++)
//	{
//		pShadow->bUse = true;
//
// 		if (nCntShadow == -1)
//		{
//			pShadow->pos = pPlayer->pos;
//		}
//
//		else if (nCntShadow == 0)
//		{
//			pShadow->pos = pBullet->pos;
//		}
//
//		nCntShadow = nCount;
//
//	}
//
//	return nCntShadow;
//}
//
//void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
//{
//	PLAYER *pPlayer = GetPlayer();
//	SHADOW *pShadow = GetShadow();
//	Bullet *pBullet = GetBullet();
//
//	if (pShadow->type == SHADOWTYPE_PLAYER)
//	{
//		pShadow->pos = pPlayer->pos;
//	}
//
//	else if (pShadow->type == SHADOWTYPE_BULLET)
//	{
//		pShadow->pos = pBullet->pos;
//	}
//	
//
//	//pShadow->pos = pBullet->pos;
//}
//
//
//SHADOW *GetShadow(void)
//{
//	return &g_Shadow;
//}
//
//CShadow::CShadow()
//{
//}
//
//CShadow::~CShadow()
//{
//}
//
//HRESULT CShadow::Init(void)
//{
//	return E_NOTIMPL;
//}
//
//void CShadow::Uninit(void)
//{
//}
