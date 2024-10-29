//===========================================================
//
// �G�̏��� [enemy.cpp]
// Author : �匴�@�叫
//
//===========================================================
#include "enemy.h"
#include "enemy_weak.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "motion.h"
#include "sound.h"
#include "player.h"
#include "game.h"
#include "debugproc.h"
#include "score.h"
#include "player.h"
#include "character.h"
#include "collision.h"
#include "enemymanager.h"
#include "gage.h"
#include "camera.h"
#include "particle.h"
#include "effect.h"
#include "item.h"
#include "MyEffekseer.h"
#include "utility.h"
#include <assert.h>

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
CEnemy *CEnemy::m_pTop = nullptr;
CEnemy *CEnemy::m_pCur = nullptr;
int CEnemy::m_nIdx = 0;
int CEnemy::m_nNumAll = 0;

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const int DAMAGECOUNT = 15;     // �_���[�W���
	const float RADIUS = 20.0f;     // ����
	const float ATTACKLENGHT = 50.0f;  // �U���\�͈�
	const float ENEMY_MOVE = 2.0f;  // �ړ���
	const float SPEED = 2.0f;          // ���鑬��
	const char *ENEMY_TEXT = "data\\TEXT\\motion_set_enemy.txt";  // �e�L�X�g�t�@�C��
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CEnemy::CEnemy()
{
	// �l���N���A
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_Info.mtxWorld);
	m_Mobility = Immobile;
	m_Info.nLife = 0;
	m_Info.nIdxID = -1;
	m_Info.bDraw = true;
	m_nDamageCounter = 0;
	m_pCurrent = nullptr;
	m_pNext = nullptr;
	m_bDeath = false;
	
	if (m_pTop != nullptr)
	{// �擪�����݂��Ă���ꍇ

		m_pCur->m_pNext = this;	// ���ݍŌ���̃I�u�W�F�N�g�̃|�C���^�ɂȂ���
		m_pPrev = m_pCur;
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
	else
	{// ���݂��Ȃ��ꍇ

		m_pTop = this;	// �������g���擪�ɂȂ�
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CEnemy::CEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
{
	// �l���N���A
	m_Info.pos = pos;
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = rot;
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_Info.mtxWorld);
	m_Mobility = Immobile;
	m_Info.nLife = nlife;
	m_Info.nIdxID = -1;
	m_Info.bDraw = true;
	m_nDamageCounter = 0;
	m_pCurrent = nullptr;
	m_pNext = nullptr;
	m_bDeath = false;
	
	if (m_pTop != nullptr)
	{// �擪�����݂��Ă���ꍇ

		m_pCur->m_pNext = this;	// ���ݍŌ���̃I�u�W�F�N�g�̃|�C���^�ɂȂ���
		m_pPrev = m_pCur;
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
	else
	{// ���݂��Ȃ��ꍇ

		m_pTop = this;	// �������g���擪�ɂȂ�
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CEnemy::~CEnemy()
{

}

////===========================================================
//// ��������
////===========================================================
//CEnemy * CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority)
//{
//	CEnemy *pEnemy = nullptr;
//
//	if (pEnemy == nullptr)
//	{
//		pEnemy = new CEnemy(pos, rot, nlife);
//
//		pEnemy->Init();
//	}
//
//	return pEnemy;
//}

//===========================================================
// ����������
//===========================================================
HRESULT CEnemy::Init(void)
{
	if (m_pMotion == nullptr)
	{// �g�p����Ă��Ȃ�������

		// ����
		m_pMotion = new CMotion;

		// ����������
		m_pMotion->Init();
	}

	m_nDamageCounter = DAMAGECOUNT;

	m_Info.nIdxID = m_nNumAll;
	m_nNumAll++;

	return S_OK;
}

//===========================================================
// �I������
//===========================================================
void CEnemy::Uninit(void)
{
	m_nNumAll = 0;

	m_bDeath = true;

	// ���������X�g�������
	ListOut();

	// �e�̃|�C���^
	if (m_pCurrent != nullptr)
		m_pCurrent = nullptr;

	// ���[�V�����̔j��
	if (m_pMotion != nullptr)
	{
		//�I������
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	// �p�[�c�̔j��
	if (m_apModel != nullptr)
	{
		for (int nCount = 0; nCount < m_nNumModel; nCount++)
		{
			if (m_apModel[nCount] != nullptr)
			{
				m_apModel[nCount]->Uninit();
				m_apModel[nCount] = nullptr;
			}
		}

		delete m_apModel;
		m_apModel = nullptr;
	}

	CObject::Release();
}

//===========================================================
// �X�V����
//===========================================================
void CEnemy::Update(void)
{
	// ���[�V�����̍X�V
	if (m_pMotion != nullptr)
		m_pMotion->Update();

	// ���f���̍X�V
	if (m_apModel == nullptr)
		return;
	
	for (int nCount = 0; nCount < m_nNumModel; nCount++)
	{
		if (m_apModel[nCount] != nullptr)
			m_apModel[nCount]->Update();

	}

	CManager::GetInstance()->GetDebugProc()->Print("\n�G�̈ʒu�F%f,%f,%f\n", m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("�G�̌����F%f,%f,%f\n", m_Info.rot.x, m_Info.rot.y, m_Info.rot.z);
}

//===========================================================
// �`�揈��
//===========================================================
void CEnemy::Draw(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �`���؂��Ă����珈���𔲂���
	if (!m_Info.bDraw)
		return;
	
	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	if (m_pCurrent != nullptr)
	{
		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);

		D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);

		D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);

		// �}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
		D3DXMatrixMultiply(&m_Info.mtxWorld,
			&m_Info.mtxWorld, m_pCurrent);
	}
	else
	{
		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);

		D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);

		D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_Info.mtxWorld);

	// �`�揈��
	for (int nCount = 0; nCount < m_nNumModel; nCount++)
	{
		if (m_apModel[nCount] == nullptr)
			continue;

		m_apModel[nCount]->Draw();
	}
}

//===========================================================
// ���X�g���玩������������
//===========================================================
void CEnemy::ListOut(void)
{
	if (m_pTop == this)
	{// ���g���擪

		if (m_pNext != nullptr)
		{// �������݂��Ă���

			m_pTop = m_pNext;	// ����擪�ɂ���
			m_pNext->m_pPrev = nullptr;	// ���̑O�̃|�C���^���o���Ă��Ȃ��悤�ɂ���
		}
		else
		{// ���݂��Ă��Ȃ�

			m_pTop = nullptr;	// �擪���Ȃ���Ԃɂ���
			m_pCur = nullptr;	// �Ō�����Ȃ���Ԃɂ���
		}
	}
	else if (m_pCur == this)
	{// ���g���Ō��

		if (m_pPrev != nullptr)
		{// �������݂��Ă���

			m_pCur = m_pPrev;			// �O���Ō���ɂ���
			m_pPrev->m_pNext = nullptr;	// �O�̎��̃|�C���^���o���Ă��Ȃ��悤�ɂ���
		}
		else
		{// ���݂��Ă��Ȃ�

			m_pTop = nullptr;	// �擪���Ȃ���Ԃɂ���
			m_pCur = nullptr;	// �Ō�����Ȃ���Ԃɂ���
		}
	}
	else
	{
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = m_pPrev;	// ���g�̎��ɑO�̃|�C���^���o��������
		}
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = m_pNext;	// ���g�̑O�Ɏ��̃|�C���^���o��������
		}
	}
}

//===========================================================
// �e�L�X�g�t�@�C���ǂݍ���
//===========================================================
void CEnemy::ReadText(const char *fliename)
{
	char aString[128] = {};
	char aComment[128] = {};
	int nCntParts = 0, nCntParts2 = 0, nCntParts3 = 0;
	int nCntMotion = 0;
	int nCntKeySet = 0;
	int nCntKey = 0;
	int nCntModel = 0;
	int nCntMotionIdx = 0;

	//�e�N�X�`���̏��擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	FILE *pFile;   //�t�@�C���|�C���^��錾

	pFile = fopen(fliename, "r");

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
					fscanf(pFile, "%d", &m_nNumModel);  //���f���̑���

					m_apModel = new CCharacter*[m_nNumModel];

				}  //NUM_MODEL�̂�����

				if (strcmp("MODEL_FILENAME", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);          //=
					fscanf(pFile, "%s", &m_filename[0]);  //���f���̖��O

					m_apModel[nCntModel] = CCharacter::Create(m_filename);
					nCntModel++;

					nCntParts++;

				}  //MODEL_LILENAME�̂�����

				if (strcmp("CHARACTERSET", aString) == 0)
				{
					while (strcmp("END_CHARACTERSET", aString) != 0)
					{
						fscanf(pFile, "%s", &aString);

						if (strcmp("PARTSSET", aString) == 0)
						{
							while (strcmp("END_PARTSSET", aString) != 0)
							{
								fscanf(pFile, "%s", &aString);

								if (strcmp("INDEX", aString) == 0)
								{
									fscanf(pFile, "%s", &aString);          //=
									fscanf(pFile, "%d", &m_nIdx);  //���f���̔ԍ�
								}

								if (strcmp("PARENT", aString) == 0)
								{
									fscanf(pFile, "%s", &aString);          //=
									fscanf(pFile, "%d", &m_nParent);  //�e���f���̔ԍ�

									if (m_nParent > -1 && m_nNumModel > m_nParent)
									{
										m_apModel[nCntParts2]->SetParent(m_apModel[m_nParent]);
									}
									else
									{
										m_apModel[nCntParts2]->SetParent(nullptr);
									}
								}

								if (strcmp("POS", aString) == 0)
								{
									fscanf(pFile, "%s", &aString);      //=
									fscanf(pFile, "%f", &m_Readpos.x);  //���f���̑���
									fscanf(pFile, "%f", &m_Readpos.y);  //���f���̑���
									fscanf(pFile, "%f", &m_Readpos.z);  //���f���̑���

									m_apModel[nCntParts2]->SetPositionOri(m_Readpos);

									m_apModel[nCntParts2]->SetPosition(m_Readpos);
								}

								if (strcmp("ROT", aString) == 0)
								{
									fscanf(pFile, "%s", &aString);      //=
									fscanf(pFile, "%f", &m_Readrot.x);  //���f���̑���
									fscanf(pFile, "%f", &m_Readrot.y);  //���f���̑���
									fscanf(pFile, "%f", &m_Readrot.z);  //���f���̑���

									m_apModel[nCntParts2]->SetRotOrigin(m_Readrot);

									m_apModel[nCntParts2]->SetRot(m_Readrot);
								}

							}//END_PARTSSET�̂�����

							nCntParts2++;

						}//PARTSSET�̂�����

					}//END_CHARACTERSET�̂����� 

				}//CHARACTERSET�̂����� 
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{
		return;
	}

	if (m_pMotion != nullptr)
	{
		//���f���̐ݒ�
		m_pMotion->SetModel(m_apModel, m_nNumModel);

		//����������
		m_pMotion->ReadText(fliename);

		m_pMotion->InitPose(MOTION_NEUTRAL);
	}
}

void CEnemy::SetChase(CHASE cha)
{
}

//===========================================================
// �U�����������Ă��邩�ǂ�������
//===========================================================
void CEnemy::HitDetection(D3DXVECTOR3 MyPos, float attackrange, float targetradius)
{
	if (m_pMotion == nullptr)
		return;

	if (m_pMotion->GetNowFrame() == m_pMotion->GetAttackOccurs())
	{
		D3DXMATRIX mtx = *GetCharcter()[0]->GetMtxWorld();
		MyEffekseer::EffectCreate(CMyEffekseer::TYPE_ATTACK, false, D3DXVECTOR3(mtx._41, mtx._42, mtx._43));
	}

	if (m_pMotion->GetAttackOccurs() <= m_pMotion->GetNowFrame() && m_pMotion->GetAttackEnd() >= m_pMotion->GetNowFrame())
	{// ���݂̃t���[�����U�����蔭���t���[���ȏォ�U������I���t���[���Ȃ�

		if (CGame::GetCollision()->Circle(MyPos, CGame::GetPlayer()->GetPosition(), attackrange, targetradius) == true)
			CPlayer::GetInstance()->Damage(GetMotion()->GetAttackDamage(), D3DXVECTOR3(sinf(GetRotition().y) * GetMotion()->GetKnockBack(), 0.0f, cosf(GetRotition().y) * GetMotion()->GetKnockBack()));
	}
}