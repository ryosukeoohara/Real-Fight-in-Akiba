//===========================================================
//
//�G�f�B�b�g[edit.cpp]
//Author �匴�叫
//
//===========================================================
#include "edit.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "player.h"
#include "InputKeyBoard.h"

#include <stdio.h>
#include<string.h>

//�}�N����`
#define MODEL_SET_TEXT  ("data\\TEXT\\model_set.txt")
#define MODEL  ("data\\TEXT\\model.txt")
#define MOVE   (4.0f)  //�ړ���

//===========================================================
//�R���X�g���N�^
//===========================================================
CEdit::CEdit()
{
	//m_pos = { 0.0f, 0.0f, 0.0f };
	m_Model[0] = {};
	m_nIdx = 0;
	m_nNumAll = 0;
	m_SetModel = 0;
	m_nSelectModel = 0;
	m_mode = MODE_CREATE;
}

//===========================================================
//�f�X�g���N�^
//===========================================================
CEdit::~CEdit()
{

}

//===========================================================
//����������
//===========================================================
HRESULT CEdit::Init(void)
{
	for (int nCount = 0; nCount < MAX_MODELSET; nCount++)
	{
		m_apModel[nCount] = nullptr;
	}

	//�e�L�X�g�t�@�C���ǂݍ���
	ReadText();

	//X�t�@�C���ǂݍ���
	XLoad();

	return S_OK;
}

//===========================================================
//�I������
//===========================================================
void CEdit::Uninit(void)
{
	for (int nCount = 0; nCount < m_nNumAll; nCount++)
	{
		if (m_Model[nCount].m_pTexture != NULL)
		{
			m_Model[nCount].m_pTexture = NULL;
		}
	}

	for (int nCount = 0; nCount < m_nNumAll; nCount++)
	{
		//�e�N�X�`���̏���
		if (m_Model[nCount].m_pMesh != NULL)
		{
			m_Model[nCount].m_pMesh->Release();
			m_Model[nCount].m_pMesh = NULL;
		}
	}

	for (int nCount = 0; nCount < m_nNumAll; nCount++)
	{
		//���_�o�b�t�@�̔j��
		if (m_Model[nCount].m_pBuffMat != NULL)
		{
			m_Model[nCount].m_pBuffMat->Release();
			m_Model[nCount].m_pBuffMat = NULL;
		}
	}
}

//===========================================================
//�X�V����
//===========================================================
void CEdit::Update(void)
{
	//�L�[�{�[�h�̏����擾
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	for (int nCount = 0; nCount < MAX_MODELSET; nCount++)
	{
		// �X�V����
		if (m_apModel[nCount] != NULL)
			m_apModel[nCount]->Update();
	}

	if (InputKeyboard == nullptr)
		return;

	if (InputKeyboard->GetTrigger(DIK_F2) == true)
	{//F2�L�[�������ꂽ

		if (m_mode == MODE_CREATE)
		{//���݂̃��[�h���N���G�C�g���[�h��������

			m_mode = MODE_DELETE;  //���[�h���f���[�g�ɂ���
		}
		else if (m_mode == MODE_DELETE)
		{//���݂̃��[�h���f���[�g���[�h��������

			m_mode = MODE_CREATE;  //���[�h���N���G�C�g�ɂ���
		}
	}

	switch (m_mode)
	{
	case MODE_CREATE:

		//���䏈��
		Control();

		break;

	case MODE_DELETE:

		//�Z�b�g�������f������������
		SetModelDelete();

		break;
	}

	// �f�o�b�N���
	Debug();
}

//===========================================================
//�`�揈��
//===========================================================
void CEdit::Draw(void)
{
	m_aModel[m_nIdx]->Draw();


	//�f�o�C�X�̎擾
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX m_mtxRot, m_mtxTrans;

	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;

	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL* pMat;

	if (m_mode == MODE_CREATE)
	{//���݂̃��[�h���N���G�C�g���[�h��������

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_Model[m_nIdx].m_mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_Model[m_nIdx].m_rot.y, m_Model[m_nIdx].m_rot.x, m_Model[m_nIdx].m_rot.z);

		D3DXMatrixMultiply(&m_Model[m_nIdx].m_mtxWorld, &m_Model[m_nIdx].m_mtxWorld, &m_mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&m_mtxTrans, m_Model[m_nIdx].m_pos.x, m_Model[m_nIdx].m_pos.y, m_Model[m_nIdx].m_pos.z);

		D3DXMatrixMultiply(&m_Model[m_nIdx].m_mtxWorld, &m_Model[m_nIdx].m_mtxWorld, &m_mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_Model[m_nIdx].m_mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_Model[m_nIdx].m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIdx].m_dwNumMat; nCntMat++)
		{
			pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, nullptr);

			//���f��(�p�[�c)�̕`��
			m_Model[m_nIdx].m_pMesh->DrawSubset(nCntMat);
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//===========================================================
//�Z�[�u����
//===========================================================
void CEdit::Save(void)
{
	//�f�o�b�O�v���b�N�̏����擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	FILE* pFile;

	pFile = fopen(MODEL, "w");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		fprintf(pFile, "SCRIPT\n");

		fprintf(pFile, "\n");

		for (int nCount = 0; nCount < m_SetModel; nCount++)
		{
			D3DXVECTOR3 pos = m_apModel[nCount]->GetPosition();  //�ʒu�擾
			D3DXVECTOR3 rot = m_apModel[nCount]->GetRotition();       //�����擾
			int n = m_apModel[nCount]->GetIdxModel();

			fprintf(pFile, "MODELSET\n");
			fprintf(pFile, "    TYPE = %d\n", n);  //���
			fprintf(pFile, "    POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);      //�ʒu
			fprintf(pFile, "    ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);      //����
			fprintf(pFile, "END_MODELSET\n");
			fprintf(pFile, "\n");
		}

		fprintf(pFile, "END_SCRIPT\n");

		//�t�@�C�������
		fclose(pFile);
	}

	pDebugProc->Print("\n�ۑ�������`");
}

//===========================================================
//X�t�@�C�����[�h����
//===========================================================
void CEdit::XLoad(void)
{
	for (int nCount = 0; nCount < m_nNumAll; nCount++)
	{
		int nNumVtx;      //���_��
		DWORD dwSizeFVF;  //���_�t�H�[�}�b�g�̃T�C�Y
		BYTE* pVtxBuff;   //���_�o�b�t�@�ւ̃|�C���^

		//�f�o�C�X�̎擾
		CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		//X�t�@�C���ǂݍ���
		D3DXLoadMeshFromX(m_Model[nCount].m_filename,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_Model[nCount].m_pBuffMat,
			NULL,
			&m_Model[nCount].m_dwNumMat,
			&m_Model[nCount].m_pMesh);

		//���_�����擾
		nNumVtx = m_Model[nCount].m_pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(m_Model[nCount].m_pMesh->GetFVF());

		if (m_Model[nCount].m_pTexture == NULL)
		{
			m_Model[nCount].m_pTexture = new LPDIRECT3DTEXTURE9[(int)m_Model[nCount].m_dwNumMat];
		}

		//�}�e���A���ւ̃|�C���^
		D3DXMATERIAL* pMat;

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)m_Model[nCount].m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_Model[nCount].m_dwNumMat; nCntMat++)
		{
			//if (pMat[nCntMat].pTextureFilename != NULL)
			//{//�e�N�X�`�������݂���

			//	D3DXCreateTextureFromFile(pDevice,
			//		pMat[nCntMat].pTextureFilename,
			//		&m_Model[nCount].m_pTexture[nCntMat]);
			//}
			//else
			//{
			//	m_Model[nCount].m_pTexture[nCntMat] = NULL;
			//}

			//if (pMat[nCntMat].pTextureFilename != NULL)
			//{//�e�N�X�`�������݂���

			//	m_nIdxTexture[nCntMat] = pTexture->Regist(pMat[nCntMat].pTextureFilename);
			//}
			//else
			//{
			//	m_nIdxTexture[nCntMat] = -1;
			//}
		}

		//���_�o�b�t�@�����b�N
		m_Model[nCount].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;     //���_���W�̑��

			//�ŏ��l-------------------------------------------------
			if (m_Model[nCount].m_vtxMini.x > vtx.x)
			{
				m_Model[nCount].m_vtxMini.x = vtx.x;
			}

			if (m_Model[nCount].m_vtxMini.y > vtx.y)
			{
				m_Model[nCount].m_vtxMini.y = vtx.y;
			}

			if (m_Model[nCount].m_vtxMini.z > vtx.z)
			{
				m_Model[nCount].m_vtxMini.z = vtx.z;
			}

			//�ő�l-------------------------------------------------
			if (m_Model[nCount].m_vtxMax.x < vtx.x)
			{
				m_Model[nCount].m_vtxMax.x = vtx.x;
			}

			if (m_Model[nCount].m_vtxMax.y < vtx.y)
			{
				m_Model[nCount].m_vtxMax.y = vtx.y;
			}

			if (m_Model[nCount].m_vtxMax.z < vtx.z)
			{
				m_Model[nCount].m_vtxMax.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;    //���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		}

		//���_�o�b�t�@���A�����b�N
		m_Model[nCount].m_pMesh->UnlockVertexBuffer();
	}
}

//===========================================================
//�}�b�v�t�@�C�����[�h����
//===========================================================
void CEdit::TEXTLoad(void)
{
	char aString[128] = {};
	char aComment[128] = {};
	int nCntModel = 0;
	int ntype = 0;

	FILE* pFile;   //�t�@�C���|�C���^��錾

	pFile = fopen(MODEL, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		fscanf(pFile, "%s", &aString[0]);

		if (strcmp("SCRIPT", aString) == 0)
		{
			while (strcmp("END_SCRIPT", aString) != 0)
			{
				fscanf(pFile, "%s", &aString[0]);

				if (strcmp("MODELSET", aString) == 0)
				{
					while (strcmp("END_MODELSET", aString) != 0)
					{
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp("TYPE", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);   //=
							fscanf(pFile, "%d", &ntype);     //���
						}

						if (strcmp("POS", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);          //=
							fscanf(pFile, "%f", &m_Model[ntype].m_pos.x);  //�ʒu
							fscanf(pFile, "%f", &m_Model[ntype].m_pos.y);  //�ʒu
							fscanf(pFile, "%f", &m_Model[ntype].m_pos.z);  //�ʒu
						}

						if (strcmp("ROT", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);          //=
							fscanf(pFile, "%f", &m_Model[ntype].m_rot.x);  //�ʒu
							fscanf(pFile, "%f", &m_Model[ntype].m_rot.y);  //�ʒu
							fscanf(pFile, "%f", &m_Model[ntype].m_rot.z);  //�ʒu

							//�I�u�W�F�N�gX�𐶐�
							m_apModel[m_SetModel] = CObjectX::Create(m_Model[ntype].m_filename);
							m_apModel[m_SetModel]->SetPosition(m_Model[ntype].m_pos);
							m_apModel[m_SetModel]->SetRotition(m_Model[ntype].m_rot);
							m_apModel[m_SetModel]->SetIdxModel(m_nIdx);

							//��ސݒ�
							//m_apModel[m_SetModel]->SetType(ntype);

							m_SetModel++;  //�J�E���g�A�b�v
						}
					}
				}  //NUM_MODEL�̂�����
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//===========================================================
//�e�L�X�g�t�@�C���ǂݍ���
//===========================================================
void CEdit::ReadText(void)
{
	char aString[128] = {};
	char aComment[128] = {};
	int nCntModel = 0;

	FILE* pFile;   //�t�@�C���|�C���^��錾

	pFile = fopen(MODEL_SET_TEXT, "r");

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
					fscanf(pFile, "%s", &aString);          //=
					fscanf(pFile, "%d", &m_nNumAll);  //���f���̑���

					m_aModel = new CObjectX*[m_nNumAll];

				}  //NUM_MODEL�̂�����

				if (strcmp("MODEL_FILENAME", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);          //=
					fscanf(pFile, "%s", &m_Model[nCntModel].m_filename[0]);  //���f���̖��O

					m_aModel[nCntModel] = CObjectX::Create(m_Model[nCntModel].m_filename);
					m_aModel[nCntModel]->SetIdxModel(nCntModel);
					m_Model[nCntModel].m_nType = nCntModel;
					m_Model[nCntModel].m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					m_Model[nCntModel].m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					m_Model[nCntModel].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					nCntModel++;

				}  //MODEL_LILENAME�̂�����
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//===========================================================
//���䏈��
//===========================================================
void CEdit::Control(void)
{
	//�L�[�{�[�h�̏����擾
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//�J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXVECTOR3 CameraRot = {};

	//�J�����̌����擾
	if (pCamera != nullptr)
		CameraRot = pCamera->GetRotation();

	if (InputKeyboard == nullptr)
		return;

	if (InputKeyboard->GetTrigger(DIK_1) == true)
	{
		//�C���f�b�N�X�ԍ���-1���傫���Ƃ��A�C���f�b�N�X�ԍ�����߂�
		if (m_nIdx > -1)
			m_nIdx -= 1;

		//�C���f�b�N�X�ԍ���0��菬�����Ƃ��A�C���f�b�N�X�ԍ��𑍐�-1�ɂ���
		if (m_nIdx < 0)
			m_nIdx = m_nNumAll - 1;
	}

	if (InputKeyboard->GetTrigger(DIK_2) == true)
	{
		//�C���f�b�N�X�ԍ���������菬�����Ƃ��A�C���f�b�N�X�ԍ�����i�߂�
		if (m_nIdx < m_nNumAll)
			m_nIdx += 1;

		//�C���f�b�N�X�ԍ��������ȏ�ɂȂ����Ƃ��A�C���f�b�N�X�ԍ���0�ɂ���
		if (m_nIdx >= m_nNumAll)
			m_nIdx = 0;
	}

	// �z�u����I�u�W�F�N�g����
	if (InputKeyboard->GetTrigger(DIK_RETURN) == true)
		CreateMapModel();

	// �z�u�����I�u�W�F�N�g�S����
	if (InputKeyboard->GetTrigger(DIK_F7) == true)
		AllDelete();

	// �O���t�@�C�������o��
	if (InputKeyboard->GetTrigger(DIK_F8) == true)
		Save();

	// �}�b�v�t�@�C���ǂݍ���
	if (InputKeyboard->GetTrigger(DIK_F9) == true)
		TEXTLoad();

	// �ړ�����
	Move();

	// ��]����
	Rotation();
}

//===========================================================
//�Z�b�g�������f����I�����ď������鏈��
//===========================================================
void CEdit::SetModelDelete(void)
{
	//�L�[�{�[�h�̏����擾
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//�f�o�b�O�v���b�N�̏����擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	if (InputKeyboard->GetTrigger(DIK_1) == true)
	{
		//�C���f�b�N�X�ԍ���-1���傫��
		if (m_nSelectModel > -1)
			m_nSelectModel -= 1;
	}

	//�C���f�b�N�X�ԍ���0��菬�����Ƃ��A�C���f�b�N�X�ԍ��𑍐�-1�ɂ���
	if (m_nSelectModel < 0)
		m_nSelectModel = m_SetModel - 1;

	if (InputKeyboard->GetTrigger(DIK_2) == true)
	{
		//�C���f�b�N�X�ԍ���������菬�����Ƃ�
		if (m_nSelectModel < m_SetModel)
			m_nSelectModel += 1;
	}

	//�C���f�b�N�X�ԍ��������ȏ�ɂȂ����Ƃ��A�C���f�b�N�X�ԍ���0�ɂ���
	if (m_nSelectModel >= m_SetModel)
		m_nSelectModel = 0;

	if (InputKeyboard->GetTrigger(DIK_DELETE) == true)
	{
		if (m_apModel[m_nSelectModel] != nullptr)
		{//�g�p����Ă�����

			//����������
			m_apModel[m_nSelectModel]->Uninit();

			//�g�p���Ă��Ȃ���Ԃɂ���
			m_apModel[m_nSelectModel] = nullptr;

			//�J�E���g�_�E��
			m_SetModel--;
		}

		for (int nCount = 0; nCount < m_SetModel; nCount++)
		{
			if (m_apModel[nCount] == nullptr && m_apModel[nCount + 1] != nullptr)
			{//���݂̃J�E���g���g�p����Ă��Ȃ����A���̃J�E���g���g�p����Ă���Ƃ�

				//���
				m_apModel[nCount] = m_apModel[nCount + 1];

				//���̃J�E���g���g�p���Ă��Ȃ���Ԃɂ���
				m_apModel[nCount + 1] = nullptr;
			}
		}
	}

	if (m_SetModel > 0)
	{
		pDebugProc->Print("\n<< �폜����I�u�W�F�N�g�̔ԍ��F%d/%d >>", m_nSelectModel, m_SetModel);
	}

	if (m_SetModel <= 0)
	{
		pDebugProc->Print("\n<< �폜�ł���I�u�W�F�N�g�͂���܂��� >>");
	}
}

//===========================================================
// �z�u����I�u�W�F�N�g��������
//===========================================================
void CEdit::CreateMapModel(void)
{
	if (m_apModel[m_SetModel] == nullptr)
	{//�g�p����Ă��Ȃ��Ƃ�

		//�I�u�W�F�N�gX�𐶐�
		m_apModel[m_SetModel] = CObjectX::Create(m_aModel[m_nIdx]->GetFileName());

		m_apModel[m_SetModel]->SetPosition(m_aModel[m_nIdx]->GetPosition());

		m_apModel[m_SetModel]->SetRotition(m_aModel[m_nIdx]->GetRotition());
		m_apModel[m_SetModel]->SetIdxModel(m_nIdx);

		//��ސݒ�
		//m_apModel[m_SetModel]->SetType(m_nIdx);

		//�������J�E���g�A�b�v
		m_SetModel++;
	}
}

//===========================================================
//�ړ�����
//===========================================================
void CEdit::Move(void)
{
	//�L�[�{�[�h�̏����擾
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//�J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXVECTOR3 CameraRot = {};

	//�J�����̌����擾
	if (pCamera != nullptr)
		CameraRot = pCamera->GetRotation();

	if (InputKeyboard == nullptr)
		return;

	D3DXVECTOR3 pos = m_aModel[m_nIdx]->GetPosition();

	//��Ɉړ�----------------------------------------------
	if (InputKeyboard->GetPress(DIK_W) == true)
	{//W�L�[�������ꂽ

		if (InputKeyboard->GetPress(DIK_D) == true)
		{//D�L�[�������� �E��Ɉړ�

		 //�ړ���
			pos.x += sinf(CameraRot.y + (D3DX_PI * 0.75f)) * MOVE;
			pos.z += cosf(CameraRot.y + (D3DX_PI * 0.75f)) * MOVE;
		}
		else if (InputKeyboard->GetPress(DIK_A) == true)
		{//A�L�[�������� ����Ɉړ�

		 //�ړ���
			pos.x += sinf(CameraRot.y + (D3DX_PI * 0.25f)) * MOVE;
			pos.z += cosf(CameraRot.y + (D3DX_PI * 0.25f)) * MOVE;
		}
		else
		{//W�L�[����������	�^��Ɉړ�

		 //�ړ���
			pos.x -= sinf(CameraRot.y) * MOVE;
			pos.z -= cosf(CameraRot.y) * MOVE;
		}
	}
	//���Ɉړ�----------------------------------------------
	else if (InputKeyboard->GetPress(DIK_S) == true)
	{//S�L�[�������ꂽ

		if (InputKeyboard->GetPress(DIK_D) == true)
		{//D�L�[�������� �E���Ɉړ�

		 //�ړ���
			pos.x += sinf(CameraRot.y + (-D3DX_PI * 0.25f)) * MOVE;
			pos.z += cosf(CameraRot.y + (-D3DX_PI * 0.25f)) * MOVE;
		}
		else if (InputKeyboard->GetPress(DIK_A) == true)
		{//A�L�[�������� �����Ɉړ�

		 //�ړ���
			pos.x += sinf(CameraRot.y + (D3DX_PI * 0.25f)) * MOVE;
			pos.z += cosf(CameraRot.y + (D3DX_PI * 0.25f)) * MOVE;
		}
		else
		{//S�L�[���������� �^���Ɉړ�

		 //�ړ���
			pos.x += sinf(CameraRot.y) * MOVE;
			pos.z += cosf(CameraRot.y) * MOVE;
		}
	}
	//�E�Ɉړ�----------------------------------------------
	else if (InputKeyboard->GetPress(DIK_D) == true)
	{//D�L�[����������

	 //�ړ���
		pos.x += sinf(CameraRot.y + (-D3DX_PI * 0.5f)) * MOVE;
		pos.z += cosf(CameraRot.y + (-D3DX_PI * 0.5f)) * MOVE;
	}
	//���Ɉړ�----------------------------------------------
	else if (InputKeyboard->GetPress(DIK_A) == true)
	{//A�L�[����������

		//�ړ���
		pos.x += sinf(CameraRot.y + (D3DX_PI * 0.5f)) * MOVE;
		pos.z += cosf(CameraRot.y + (D3DX_PI * 0.5f)) * MOVE;
	}

	//�ړ��ʂ��X�V(����������)--------------------------------------------
	pos.x += (0.0f - m_Model[m_nIdx].m_move.x) * 0.1f;
	pos.z += (0.0f - m_Model[m_nIdx].m_move.z) * 0.1f;

	//�ړ��ʉ��Z
	pos.x += m_Model[m_nIdx].m_move.x;
	pos.z += m_Model[m_nIdx].m_move.z;

	m_aModel[m_nIdx]->SetPosition(pos);

	//�ʒu����
	//m_Model[m_nIdx].m_pos = m_pos;
}

//===========================================================
//��]����
//===========================================================
void CEdit::Rotation(void)
{
	//�L�[�{�[�h�̏����擾
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	if (InputKeyboard == nullptr)
		return;

	D3DXVECTOR3 rot = m_aModel[m_nIdx]->GetRotition();

	if (InputKeyboard->GetPress(DIK_U) == true)
	{//U�L�[��������

		rot.y += 0.01f;
	}

	if (InputKeyboard->GetPress(DIK_I) == true)
	{//I�L�[��������

		rot.y -= 0.01f;
	}

	if (InputKeyboard->GetTrigger(DIK_O) == true)
	{//O�L�[��������

		rot.y = 0.0f;
	}

	if (rot.y > D3DX_PI)
	{
		rot.y = -D3DX_PI;
	}

	else if (rot.y < -D3DX_PI)
	{
		rot.y = D3DX_PI;
	}

	m_aModel[m_nIdx]->SetRotition(rot);
}

//===========================================================
//�z�u�����I�u�W�F�N�g�����ׂď������鏈��
//===========================================================
void CEdit::AllDelete(void)
{
	for (int nCount = 0; nCount < m_SetModel; nCount++)
	{
		if (m_apModel[nCount] != nullptr)
		{//�g�p����Ă���Ƃ�

			//�I������
			m_apModel[nCount]->Uninit();

			//�g�p���Ă��Ȃ���Ԃɂ���
			m_apModel[nCount] = nullptr;
		}
	}

	m_SetModel = 0;
}

//===========================================================
//�f�o�b�O���
//===========================================================
void CEdit::Debug(void)
{
	//�f�o�b�O�v���b�N�̏����擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	pDebugProc->Print("\n<< -----------------�G�f�B�b�g�̏��----------------- >>");
	pDebugProc->Print("\n<< ���[�h�̐؂�ւ� : F2 >>");

	//���݂̃��[�h���N���G�C�g���[�h��������
	if (m_mode == MODE_CREATE)
		pDebugProc->Print("\n<< ���݂̃��[�h : �N���G�C�g >>");

	//���݂̃��[�h���f���[�g���[�h��������
	if (m_mode == MODE_DELETE)
		pDebugProc->Print("\n<< ���݂̃��[�h : �f���[�g >>");

	pDebugProc->Print("\n<< �I�u�W�F�N�g�̈ʒu�FX��<%f>,Y��<%f>,Z��<%f> >>", m_Model[m_nIdx].m_pos.x, m_Model[m_nIdx].m_pos.y, m_Model[m_nIdx].m_pos.z);
	pDebugProc->Print("\n<< �I�u�W�F�N�g�̌����FX��<%f>,Y��<%f>,Z��<%f> >>", m_Model[m_nIdx].m_rot.x, m_Model[m_nIdx].m_rot.y, m_Model[m_nIdx].m_rot.z);
	pDebugProc->Print("\n<< ���݂̃I�u�W�F�N�g�̔ԍ��F%d/%d >>", m_nIdx, m_nNumAll);
	pDebugProc->Print("\n<< �ݒu�����I�u�W�F�N�g�̐��F%d >>", m_SetModel);
	pDebugProc->Print("\n<< �z�u���Z�b�g�FF7 >>");
	pDebugProc->Print("\n<< �O���t�@�C�������o���FF8 >>");
	pDebugProc->Print("\n<< �O���t�@�C���ǂݍ��݁FF9 >>");
	pDebugProc->Print("\n<< ���f���̐��� : U/I >>");
	pDebugProc->Print("\n<< ���f���̌������Z�b�g : O >>");
	pDebugProc->Print("\n<< --------------------------------------------------- >>");
}