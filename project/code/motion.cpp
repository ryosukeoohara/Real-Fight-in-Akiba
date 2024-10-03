//===========================================================
//
// ���[�V��������[motion.cpp]
// Author �匴�叫
//
//===========================================================
#include "motion.h"
#include "character.h"
#include<stdio.h>
#include<time.h>
#include<string.h>

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
int CMotion::m_nObj = 0;

//===========================================================
// �R���X�g���N�^
//===========================================================
CMotion::CMotion()
{
	m_nNumAll = 0;
	m_nType = 0;
	m_nTypeold = 0;
	m_nNumKey = 0;
	m_nKey = 0;
	m_nCounter = 0;
	m_nCntkeySet = 0;
	m_nNumFrame = 0;
	m_nNowFrame = 0;
	m_bLoop = false;
	m_bFinish = false;
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CMotion::~CMotion()
{
	
}

//===========================================================
// ����������
//===========================================================
void CMotion::Init(void)
{
	
}

//===========================================================
// �I������
//===========================================================
void CMotion::Uninit(void)
{

}

//===========================================================
// �X�V����
//===========================================================
void CMotion::Update(void)
{
	if (m_bBlend)
		BlendON();   // �u�����h����
	else
		BlendOFF();  // �u�����h����
}

//===========================================================
// �ݒ菈��
//===========================================================
void CMotion::Set(int nType, bool value)
{	
	SetInfoBlendON();

	// �O��̎�ނ�ݒ�
	m_nTypeold = m_nType;

	// ����̎�ނ�ݒ�
	m_nType = nType;

	// ���[�V�����u�����h���邩�ݒ�
	m_bBlend = value;

	m_aInfo[nType].bFinish = false;
	m_nCounter = 0;
	m_nKey = 0;
	m_nNowFrame = 0;
}

//===========================================================
// ��ގ擾����
//===========================================================
int CMotion::GetType(void)
{
	return m_nType;
}

//===========================================================
// ���݂̃t���[���擾
//===========================================================
int CMotion::GetNowFrame(void)
{
	return m_nNowFrame;
}

//===========================================================
// �U�����蔭���t���[���擾
//===========================================================
int CMotion::GetAttackOccurs(void)
{
	return m_aInfo[m_nType].nAttackOccurs;
}

//===========================================================
// �U������I���t���[���擾
//===========================================================
int CMotion::GetAttackEnd(void)
{
	return m_aInfo[m_nType].nAttackEnd;
}

//===========================================================
// �U����
//===========================================================
int CMotion::GetAttackDamege(void)
{
	return m_aInfo[m_nType].nAttackDamege;
}

//===========================================================
// ���G�J�n�t���[��
//===========================================================
int CMotion::GetInvincibleStrat(void)
{
	return m_aInfo[m_nType].nInvincibleStart;
}

//===========================================================
// ���G�I���t���[��
//===========================================================
int CMotion::GetInvincibleEnd(void)
{
	return m_aInfo[m_nType].nInvincibleEnd;
}

//===========================================================
// �L�����Z���\�t���[���擾
//===========================================================
int CMotion::GetCancellFrame(void)
{
	return m_aInfo[m_nType].nCancellFrame;
}

//===========================================================
// �L�����Z���\�I���t���[���擾
//===========================================================
int CMotion::GetCancellFrameEnd(void)
{
	return m_aInfo[m_nType].nCancellFrameEnd;
}

//===========================================================
// �������
//===========================================================
float CMotion::GetKnockBack(void)
{
	return m_aInfo[m_nType].nKnockBack;
}

//===========================================================
// �ݒ菈��
//===========================================================
CMotion::INFO CMotion::GetInfo(void)
{
	return m_aInfo[m_nType];
}

//===========================================================
// �I���������ǂ���
//===========================================================
bool CMotion::IsFinish(void)
{
	return m_aInfo[m_nType].bFinish;
}

//===========================================================
// ���ݒ菈��
//===========================================================
void CMotion::SetInfoBlendON(void)
{
	for (int nCount = 0; nCount < m_nNumModel; nCount++)
	{
		m_aOldInfo[nCount].pos = m_ppModel[nCount]->GetPosition();
		m_aOldInfo[nCount].rot = m_ppModel[nCount]->GetRot();
	}
}

//===========================================================
//���ݒ菈��
//===========================================================
void CMotion::SetInfoBlendOFF(KEY key)
{
	
}

//===========================================================
// ���ݒ菈��
//===========================================================
void CMotion::SetModel(CCharacter **ppModel, int nNumModel)
{
	m_ppModel = ppModel;
	m_nNumModel = nNumModel;
}

//===========================================================
// �������[�V�����ݒ�
//===========================================================
void CMotion::InitPose(int type)
{
	if (m_ppModel == nullptr)
		return;

	for (int nCount = 0; nCount < m_nNumModel; nCount++)
	{
		//D3DXVECTOR3 pos = m_aInfo[type].KeySet[0].aKey[nCount].pos;
		D3DXVECTOR3 rot = m_aInfo[type].KeySet[0].aKey[nCount].rot;

		//m_ppModel[nCount]->SetPosition(pos);
		m_ppModel[nCount]->SetRot(rot);
	}

	SetInfoBlendON();

	m_bBlend = true;
}

//===========================================================
// �O���t�@�C���ǂݍ���
//===========================================================
void CMotion::ReadText(const char *TextFilename)
{
	char aString[128] = {};
	char aComment[128] = {};
	int nCntParts = 0, nCntParts2 = 0, nCntParts3 = 0;
	int nCntMotion = 0;
	int nCntKeySet = 0;
	int nCntKey = 0;
	int nCntModel = 0;
	int nCntMotionIdx = 0;

	FILE *pFile;   //�t�@�C���|�C���^��錾

	pFile = fopen(TextFilename, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		fscanf(pFile, "%s", &aString[0]);

		if (strcmp("SCRIPT", aString) == 0)
		{
			while (strcmp("END_SCRIPT", aString) != 0)
			{
				fscanf(pFile, "%s", &aString[0]);

				if (strcmp("MOTIONSET", aString) == 0)
				{
					while (strcmp("END_MOTIONSET", aString) != 0)
					{
						fscanf(pFile, "%s", &aString);

						if (strcmp("LOOP", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nLoop);
						}

						if (strcmp("NUM_KEY", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nNumKey);
						}

						if (strcmp("NUM_FRAME", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nNumFrame);
						}

						if (strcmp("STRAT_COLLI", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nAttackOccurs);
						}

						if (strcmp("END_COLLI", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nAttackEnd);
						}

						if (strcmp("ATTACK", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nAttackDamege);
						}

						if (strcmp("STRAT_CANCEL", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nCancellFrame);
						}

						if (strcmp("END_CANCEL", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nCancellFrameEnd);
						}

						if (strcmp("KNOCKBACK", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%f", &m_aInfo[nCntMotion].nKnockBack);
						}

						if (strcmp("STRAT", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nInvincibleStart);
						}

						if (strcmp("END", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);
							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nInvincibleEnd);
						}

						if (strcmp("KEYSET", aString) == 0)
						{
							while (strcmp("END_KEYSET", aString) != 0)
							{
								fscanf(pFile, "%s", &aString);

								if (strcmp("FRAME", aString) == 0)
								{
									fscanf(pFile, "%s", &aString);
									fscanf(pFile, "%d", &m_aInfo[nCntMotion].KeySet[nCntKeySet].nFrame);
								}

								if (m_aInfo[nCntMotion].nNumKey != nCntKeySet)
								{
									if (strcmp("PARTS", aString) == 0)
									{
										while (strcmp("END_PARTS", aString) != 0)
										{
											fscanf(pFile, "%s", &aString);

											if (strcmp("POS", aString) == 0)
											{
												fscanf(pFile, "%s", &aString);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].KeySet[nCntKeySet].aKey[nCntKey].pos.x);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].KeySet[nCntKeySet].aKey[nCntKey].pos.y);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].KeySet[nCntKeySet].aKey[nCntKey].pos.z);
											}

											if (strcmp("ROT", aString) == 0)
											{
												fscanf(pFile, "%s", &aString);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].KeySet[nCntKeySet].aKey[nCntKey].rot.x);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].KeySet[nCntKeySet].aKey[nCntKey].rot.y);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].KeySet[nCntKeySet].aKey[nCntKey].rot.z);
											}
										}


										nCntKey++;
									}
								}
							}

							nCntKeySet++;
							nCntKey = 0;
						}
					}

					nCntMotion++;
					nCntKeySet = 0;
				}
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//===========================================================
// �p�x����
//===========================================================
void CMotion::RotCorrect(float rot)
{
	if (rot > D3DX_PI)
	{
		rot -= D3DX_PI * 2.0f;
	}
	else if (rot < -D3DX_PI)
	{
		rot += D3DX_PI * 2.0f;
	}
}

//===========================================================
// ���[�V�����u�����h���čX�V
//===========================================================
void CMotion::BlendON(void)
{
	D3DXVECTOR3 fDiffpos, fDiffrot;  //�ʒu�A�����̍���
	D3DXVECTOR3 fDestpos, fDestrot;  //�ʒu�A�����̖ڕW

	if (m_nCounter >= m_aInfo[m_nType].KeySet[m_nKey].nFrame)
	{
		m_nKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

		m_nCounter = 0;
		m_nKeyFrame = 0;
	}

	int Next = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

	if (!m_aInfo[m_nType].bFinish)
	{
		for (int nCount = 0; nCount < m_nNumModel; nCount++)
		{
			D3DXVECTOR3 posOrigin = m_ppModel[nCount]->GetPositionOri();
			D3DXVECTOR3 rotOrigin = m_ppModel[nCount]->GetRotOrigin();

			fDiffpos = posOrigin + m_aInfo[m_nType].KeySet[Next].aKey[nCount].pos - m_aOldInfo[nCount].pos;
			fDiffrot = rotOrigin + m_aInfo[m_nType].KeySet[Next].aKey[nCount].rot - m_aOldInfo[nCount].rot;

			// �␳
			if (fDiffrot.x > D3DX_PI)
			{
				fDiffrot.x -= D3DX_PI * 2.0f;
			}
			else if (fDiffrot.x < -D3DX_PI)
			{
				fDiffrot.x += D3DX_PI * 2.0f;
			}

			if (fDiffrot.y > D3DX_PI)
			{
				fDiffrot.y -= D3DX_PI * 2.0f;
			}
			else if (fDiffrot.y < -D3DX_PI)
			{
				fDiffrot.y += D3DX_PI * 2.0f;
			}

			if (fDiffrot.z > D3DX_PI)
			{
				fDiffrot.z -= D3DX_PI * 2.0f;
			}
			else if (fDiffrot.z < -D3DX_PI)
			{
				fDiffrot.z += D3DX_PI * 2.0f;
			}

			fDestpos = m_aOldInfo[nCount].pos + fDiffpos * ((float)m_nCounter / (float)m_aInfo[m_nType].KeySet[m_nKey].nFrame);
			fDestrot = m_aOldInfo[nCount].rot + fDiffrot * ((float)m_nCounter / (float)m_aInfo[m_nType].KeySet[m_nKey].nFrame);

			// �␳
			if (fDestrot.x > D3DX_PI)
			{
				fDestrot.x -= D3DX_PI * 2.0f;
			}
			else if (fDestrot.x < -D3DX_PI)
			{
				fDestrot.x += D3DX_PI * 2.0f;
			}

			if (fDestrot.y > D3DX_PI)
			{
				fDestrot.y -= D3DX_PI * 2.0f;
			}
			else if (fDestrot.y < -D3DX_PI)
			{
				fDestrot.y += D3DX_PI * 2.0f;
			}

			if (fDestrot.z > D3DX_PI)
			{
				fDestrot.z -= D3DX_PI * 2.0f;
			}
			else if (fDestrot.z < -D3DX_PI)
			{
				fDestrot.z += D3DX_PI * 2.0f;
			}

			m_ppModel[nCount]->SetPosition(fDestpos);
			m_ppModel[nCount]->SetRot(fDestrot);
		}

		m_nCounter++;
		m_nNowFrame++;
		m_nKeyFrame++;


		if (m_aInfo[m_nType].nLoop == 0 && m_nKey + 1 >= m_aInfo[m_nType].nNumKey)
		{
			m_nNowFrame = 0;

			if (m_aInfo[m_nType].nLoop == 0)
			    m_aInfo[m_nType].bFinish = true;
			
		}

		if (m_aInfo[m_nType].nLoop == 1 && m_nKey + 1 >= m_aInfo[m_nType].nNumKey)
			m_nNowFrame = 0;

		if (m_nCounter >= m_aInfo[m_nType].KeySet[m_nKey].nFrame)
		{
			SetInfoBlendON();
		}
	}
}

//===========================================================
// ���[�V�����u�����h�����ɍX�V
//===========================================================
void CMotion::BlendOFF(void)
{
	D3DXVECTOR3 fDiffpos, fDiffrot;  //�ʒu�A�����̍���
	D3DXVECTOR3 fDestpos, fDestrot;  //�ʒu�A�����̖ڕW

	if (m_nCounter >= m_aInfo[m_nType].KeySet[m_nKey].nFrame)
	{
		m_nKey = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

		m_nCounter = 0;
	}

	for (int nCount = 0; nCount < m_aInfo[m_nType].nNumKey; nCount++)
	{
		m_nNumFrame += m_aInfo[m_nType].KeySet[nCount].nFrame;
	}

	int Next = (m_nKey + 1) % m_aInfo[m_nType].nNumKey;

	if (m_nType != m_nTypeold && m_nKey + 1 < m_aInfo[m_nType].nNumKey)
	{
		int n = 0;
	}

	if (m_aInfo[m_nType].bFinish == false)
	{
		for (int nCount = 0; nCount < m_nNumModel; nCount++)
		{
			D3DXVECTOR3 pos = m_ppModel[nCount]->GetPosition();
			D3DXVECTOR3 rot = m_ppModel[nCount]->GetRot();
			D3DXVECTOR3 posOrigin = m_ppModel[nCount]->GetPositionOri();
			D3DXVECTOR3 rotOrigin = m_ppModel[nCount]->GetRotOrigin();

			fDiffpos = m_aInfo[m_nType].KeySet[Next].aKey[nCount].pos - m_aOldInfo[nCount].pos;
			fDiffrot = m_aInfo[m_nType].KeySet[Next].aKey[nCount].rot - m_aOldInfo[nCount].rot;

			if (fDiffrot.x > D3DX_PI)
			{
				fDiffrot.x -= D3DX_PI * 2.0f;
			}
			else if (fDiffrot.x < -D3DX_PI)
			{
				fDiffrot.x += D3DX_PI * 2.0f;
			}

			if (fDiffrot.y > D3DX_PI)
			{
				fDiffrot.y -= D3DX_PI * 2.0f;
			}
			else if (fDiffrot.y < -D3DX_PI)
			{
				fDiffrot.y += D3DX_PI * 2.0f;
			}

			if (fDiffrot.z > D3DX_PI)
			{
				fDiffrot.z -= D3DX_PI * 2.0f;
			}
			else if (fDiffrot.z < -D3DX_PI)
			{
				fDiffrot.z += D3DX_PI * 2.0f;
			}

			fDestpos = fDiffpos * ((float)m_nCounter / (float)m_aInfo[m_nType].KeySet[m_nKey].nFrame);
			fDestrot = fDiffrot * ((float)m_nCounter / (float)m_aInfo[m_nType].KeySet[m_nKey].nFrame);

			pos = posOrigin + m_aInfo[m_nType].KeySet[m_nKey].aKey[nCount].pos + fDestpos;
			rot = rotOrigin + m_aInfo[m_nType].KeySet[m_nKey].aKey[nCount].rot + fDestrot;

			m_ppModel[nCount]->SetPosition(pos);
			m_ppModel[nCount]->SetRot(rot);
		}

		m_nCounter++;

		if (m_aInfo[m_nType].nLoop == 0 && m_nKey + 1 >= m_aInfo[m_nType].nNumKey)
		{
			m_aInfo[m_nType].bFinish = true;
		}

		//SetInfoBlendOFF(m_aInfo[m_nType].KeySet[m_nKey].aKey[]);
	}
}