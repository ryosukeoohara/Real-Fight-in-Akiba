//=============================================================================
//
// �����蔻�菈�� [collision.cpp]
// Author : �匴�@�叫
//
//=============================================================================
#include "main.h"
#include "collision.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "debugproc.h"
#include "collision.h"
#include "object.h"
#include "objectX.h"
#include "game.h"
#include "enemy.h"
#include "enemy_weak.h"
#include "enemymanager.h"
#include "itemmanager.h"
#include "character.h"
#include "motion.h"
#include "map.h"
#include "motion.h"
#include "particle.h"
#include "animation.h"
#include "tutorial.h"
#include "sound.h"
#include "mapobject.h"
#include "MyEffekseer.h"
#include "mapobject.h"

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CCollision *CCollision::m_pColli = nullptr;

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CCollision::CCollision()
{
	m_pColli = this;
	m_bColli = false;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CCollision::~CCollision()
{

}

//=============================================================================
//����������
//=============================================================================
HRESULT CCollision::Init(void)
{
	return S_OK;
}

//=============================================================================
//�I������
//=============================================================================
void CCollision::Uninit(void)
{
	if (m_pColli != nullptr)
	{
		delete m_pColli;
		m_pColli = nullptr;
	}
}

//=============================================================================
// ��������
//=============================================================================
CCollision *CCollision::Create(void)
{
	CCollision *pCollision = nullptr;

	pCollision = new CCollision;

	if (pCollision != nullptr)
		pCollision->Init();

	return pCollision;
}

//=============================================================================
//�G�ƃI�u�W�F�N�g�̓����蔻�菈��
//=============================================================================
bool CCollision::Enemy(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, float fWidthX, float fWidthZ, CEnemy **pEnemy)
{
	return false;
}

//=============================================================================
//�~�̓����蔻�菈��
//=============================================================================
bool CCollision::Circle(D3DXVECTOR3 pMyPos, D3DXVECTOR3 pTargetPos, float fMyRadius, float fTargetRadius)
{
	float circleX = pMyPos.x - pTargetPos.x;
	float circleZ = pMyPos.z - pTargetPos.z;
	float c = 0.0f;

	c = sqrtf(circleX * circleX + circleZ * circleZ);

	if (c <= fMyRadius + fTargetRadius)
		return true;
	
	return false;
}

//=============================================================================
//�~�̓����蔻�菈��
//=============================================================================
void CCollision::AttackCircle(D3DXVECTOR3 * pMyPos, float fMyRadius, float fTargetRadius, float fHeight)
{
	CEnemy *pEnemy = CEnemy::GetTop();

	while (pEnemy != nullptr)
	{
		CEnemy *pEnemyNext = pEnemy->GetNext();

		float circleX = pMyPos->x - pEnemy->GetPosition().x;
		float circleZ = pMyPos->z - pEnemy->GetPosition().z;
		float c = 0.0f;

		c = sqrtf(circleX * circleX + circleZ * circleZ);

		if (c <= fMyRadius + fTargetRadius && (pMyPos->y >= pEnemy->GetPosition().y && pMyPos->y <= pEnemy->GetPosition().y + fHeight) && pEnemy->GetState() != CEnemy::STATE_DAMEGE)
		{
			//pEnemy->Damege(CPlayer::GetInstance()->GetMotion()->GetAttackDamege(), CPlayer::GetInstance()->GetMotion()->GetKnockBack(), CPlayer::GetInstance()->GetActType());

			pEnemy->Damege();
			CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_HIT, ::Effekseer::Vector3D(pMyPos->x, pMyPos->y, pMyPos->z));
		}

		pEnemy = pEnemyNext;
	}
}

//=============================================================================
//�G�ƃI�u�W�F�N�g�̓����蔻�菈��
//=============================================================================
bool CCollision::Player(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, float fWidthX, float fWidthZ, CPlayer *pPlayer)
{
	return false;
}

//=============================================================================
// �}�b�v�ɂ��錚���Ƃ̓����蔻��
//=============================================================================
void CCollision::Map(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, float fRadius)
{
	int nNum = 0;
	D3DXVECTOR3 Mappos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CMap* pMap = CMap::GetInstance();

	// �}�b�v�I�u�W�F�N�g�̐擪�A�h���X���擾
	CObjectX* pObjectX = pMap->GetTopObjX();

	while (pObjectX != nullptr)
	{
		CObjectX* pNext = pObjectX->GetNext();

		Mappos = pObjectX->GetPosition();

		vtxMin = pObjectX->GetVtxMin();

		vtxMax = pObjectX->GetVtxMax();

		if (pObjectX->IsEnable() == false)
		{
			pObjectX = pNext;

			continue;
		}
			
		if (pos->z + fRadius > Mappos.z + vtxMin.z
			&& pos->z + -fRadius < Mappos.z + vtxMax.z)
		{
			//�u���b�N�̉E����==================================
			if (pos->x + -fRadius <= Mappos.x + vtxMax.x
				&& posOld->x + -fRadius >= Mappos.x + vtxMax.x)
			{
				pos->x = (Mappos.x + vtxMax.x) + fRadius;
			}
			//�u���b�N�̍�����==================================
			if (pos->x + fRadius >= Mappos.x + vtxMin.x
				&& posOld->x + fRadius <= Mappos.x + vtxMin.x)
			{
				pos->x = (Mappos.x + vtxMin.x) - fRadius;
			}
		}

		if (pos->x + fRadius > Mappos.x + vtxMin.x
			&& pos->x - fRadius < Mappos.x + vtxMax.x)
		{
			//�u���b�N�̏�======================================
			if (pos->z - fRadius * 1.1f <= Mappos.z + vtxMax.z
				&& posOld->z - fRadius >= Mappos.z + vtxMax.z)
			{
				pos->z = (Mappos.z + vtxMax.z) + fRadius * 1.1f;
			}

			//�u���b�N�̉�======================================
			if (pos->z + fRadius >= Mappos.z + vtxMin.z
				&& posOld->z + fRadius <= Mappos.z + vtxMin.z)
			{
				pos->z = (Mappos.z + vtxMin.z) - fRadius;
			}
		}

		pObjectX = pNext;
	}
}

//=============================================================================
// �}�b�v�I�u�W�F�N�g(�O����W��)�Ƃ̓����蔻��
//=============================================================================
bool CCollision::HitMapObject(D3DXVECTOR3 pMyPos, D3DXVECTOR3 pTargetPos, float fMyRadius, float fTargetRadius)
{
	int nNum = 0;
	D3DXVECTOR3 Mappos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CMap* pMap = CMap::GetInstance();

	// �}�b�v�I�u�W�F�N�g�̐擪�A�h���X���擾
	CObjectX* pObjectX = pMap->GetTopObjX();

	while (pObjectX != nullptr)
	{
		CObjectX* pNext = pObjectX->GetNext();

		int nIdx = pObjectX->GetIdxModel();

		if (pObjectX->IsEnable() == false || nIdx < 43 && nIdx > 51)
		{
			pObjectX = pNext;

			continue;
		}

		Mappos = pObjectX->GetPosition();

		vtxMin = pObjectX->GetVtxMin();

		vtxMax = pObjectX->GetVtxMax();

		float circleX = pMyPos.x - Mappos.x;
		float circleZ = pMyPos.z - Mappos.z;
		float c = 0.0f;

		c = sqrtf(circleX * circleX + circleZ * circleZ);

		if (c <= fMyRadius + fTargetRadius)
		{
			pObjectX->SetbHitAttack(true);

			if (nIdx == 46)
			{
				int n = pObjectX->GetHitCounter();
				n++;
				pObjectX->SetHitCounter(n);

				if (n >= 3 && !pObjectX->GetDamage())
				{
					pObjectX->SetbDamage(true);

					CObjectX *p = CObjectX::Create("data\\MODEL\\map\\light_rubble_L.x");
					p->SetPosition(D3DXVECTOR3(Mappos.x, 400.0f, Mappos.z + 50.0f));
					p->SetGravity(5.0f);
					
					CObjectX* pp = CObjectX::Create("data\\MODEL\\map\\light_rubble_R.x");
					pp->SetPosition(D3DXVECTOR3(Mappos.x, 400.0f, Mappos.z - 50.0f));
					pp->SetGravity(5.0f);
				}
			}
		
			return true;
		}

		pObjectX = pNext;
	}

	return false;
}

//=============================================================================
// �d�@�{�b�N�X�Ƃ̔���
//=============================================================================
bool CCollision::HitElectricBox(D3DXVECTOR3 pMyPos, D3DXVECTOR3 pTargetPos, float fMyRadius, float fTargetRadius)
{
	int nNum = 0;
	D3DXVECTOR3 Mappos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CMap* pMap = CMap::GetInstance();

	// �}�b�v�I�u�W�F�N�g�̐擪�A�h���X���擾
	CMapObject* pMapObject = pMap->GetTop();

	while (pMapObject != nullptr)
	{
		CMapObject* pNext = pMapObject->GetNext();

		Mappos = pMapObject->GetPosition();

		vtxMin = pMapObject->GetVtxMin();

		vtxMax = pMapObject->GetVtxMax();

		int nIdx = pMapObject->GetIdxModel();

		if (pMapObject->IsEnable() == false || nIdx > 43 && nIdx < 51)
		{
			pMapObject = pNext;

			continue;
		}

		float circleX = pMyPos.x - Mappos.x;
		float circleZ = pMyPos.z - Mappos.z;
		float c = 0.0f;

		c = sqrtf(circleX * circleX + circleZ * circleZ);

		if (c <= fMyRadius + fTargetRadius && nIdx == 3)
		{
			pMapObject->SetbFall(true);
			int n = pMapObject->GetFallCounter();
			n++;
			//pMapObject->SetFallCounter(n);

			return true;
		}

		if (c <= fMyRadius + fTargetRadius && nIdx == 4)
		{
			pMapObject->SetbShut(true);
			pMapObject->SetbWaterLeak(true);

			return true;
		}

		pMapObject = pNext;
	}

	return false;
}

//=============================================================================
// �}�b�v�ɂ��錚���Ƃ̓����蔻��
//=============================================================================
void CCollision::MapObject(D3DXVECTOR3* pos, D3DXVECTOR3* posOld, float fRadius)
{
	int nNum = 0;
	D3DXVECTOR3 Mappos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CMap* pMap = CMap::GetInstance();

	// �}�b�v�I�u�W�F�N�g�̐擪�A�h���X���擾
	CMapObject* pMapObject = pMap->GetTop();

	while (pMapObject != nullptr)
	{
		CMapObject* pNext = pMapObject->GetNext();

		Mappos = pMapObject->GetPosition();

		vtxMin = pMapObject->GetVtxMin();

		vtxMax = pMapObject->GetVtxMax();

		if (pMapObject->IsEnable() == false)
		{
			pMapObject = pNext;

			continue;
		}

		pMapObject->SetbDown(false);

		int nIdx = pMapObject->GetIdxModel();

		if (pos->z + fRadius > Mappos.z + vtxMin.z
			&& pos->z + -fRadius < Mappos.z + vtxMax.z)
		{
			//�u���b�N�̉E����==================================
			if (pos->x + -fRadius <= Mappos.x + vtxMax.x
				&& posOld->x + -fRadius >= Mappos.x + vtxMax.x)
			{
				if (nIdx != 0 && nIdx != 3 && nIdx != 4)
					pMapObject->SetbShut(true);

				else if (nIdx != 3 && nIdx != 4)
					pMapObject->SetbDown(true);

				else if (nIdx == 3 || nIdx == 4)
					pos->x = (Mappos.x + vtxMax.x) + fRadius;

			}
			//�u���b�N�̍�����==================================
			if (pos->x + fRadius >= Mappos.x + vtxMin.x
				&& posOld->x + fRadius <= Mappos.x + vtxMin.x)
			{
				if (nIdx != 0 && nIdx != 3 && nIdx != 4)
					pMapObject->SetbShut(true);

				else if (nIdx != 3 && nIdx != 4)
					pMapObject->SetbDown(true);

				else if (nIdx == 3 || nIdx == 4)
					pos->x = (Mappos.x + vtxMin.x) - fRadius;

			}
		}

		if (pos->x + fRadius > Mappos.x + vtxMin.x
			&& pos->x + -fRadius < Mappos.x + vtxMax.x)
		{
			//�u���b�N�̏�======================================
			if (pos->z + -fRadius <= Mappos.z + vtxMax.z
				&& posOld->z + -fRadius >= Mappos.z + vtxMax.z)
			{
				if (nIdx != 0 && nIdx != 3 && nIdx != 4)
					pMapObject->SetbShut(true);

				else if (nIdx != 3 && nIdx != 4)
					pMapObject->SetbDown(true);

				else if (nIdx == 3 || nIdx == 4)
					pos->z = (Mappos.z + vtxMax.z) + fRadius * 1.1f;

			}

			//�u���b�N�̉�======================================
			if (pos->z + fRadius >= Mappos.z + vtxMin.z
				&& posOld->z + fRadius <= Mappos.z + vtxMin.z)
			{
				if (nIdx != 0 && nIdx != 3 && nIdx != 4)
					pMapObject->SetbShut(true);

				else if (nIdx != 3 && nIdx != 4)
					pMapObject->SetbDown(true);

				else if (nIdx == 3 || nIdx == 4)
					pos->z = (Mappos.z + vtxMin.z) - fRadius;

			}
		}

		pMapObject = pNext;
	}
}

//=============================================================================
// �}�b�v��
//=============================================================================
bool CCollision::CheckPlayerMapObject(CPlayer* pPlayer, CMapObject* pMapObject, float fRadius)
{
	D3DXVECTOR3 Mappos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CPlayer::INFO* pInfo = pPlayer->GetInfo();

	Mappos = pMapObject->GetPosition();

	vtxMin = pMapObject->GetVtxMin();

	vtxMax = pMapObject->GetVtxMax();

	int nIdx = pMapObject->GetIdxModel();

	if (pInfo->pos.z + fRadius > Mappos.z + vtxMin.z
		&& pInfo->pos.z + -fRadius < Mappos.z + vtxMax.z)
	{
		//�u���b�N�̉E����==================================
		if (pInfo->pos.x + -fRadius <= Mappos.x + vtxMax.x
			&& pInfo->posOld.x + -fRadius >= Mappos.x + vtxMax.x)
		{
			if (nIdx == 3 || nIdx == 4 || nIdx == 5 || nIdx == 6)
				pInfo->pos.x = (Mappos.x + vtxMax.x) + fRadius;
			else
			    return true;
		}

		//�u���b�N�̍�����==================================
		if (pInfo->pos.x + fRadius >= Mappos.x + vtxMin.x
			&& pInfo->posOld.x + fRadius <= Mappos.x + vtxMin.x)
		{
			if (nIdx == 3 || nIdx == 4 || nIdx == 5 || nIdx == 6)
				pInfo->pos.x = (Mappos.x + vtxMin.x) - fRadius;
			else
			    return true;
		}
	}

	if (pInfo->pos.x + fRadius > Mappos.x + vtxMin.x
		&& pInfo->pos.x + -fRadius < Mappos.x + vtxMax.x)
	{
		//�u���b�N�̏�======================================
		if (pInfo->pos.z + -fRadius <= Mappos.z + vtxMax.z
			&& pInfo->posOld.z + -fRadius >= Mappos.z + vtxMax.z)
		{
			if (nIdx == 3 || nIdx == 4 || nIdx == 5 || nIdx == 6)
				pInfo->pos.z = (Mappos.z + vtxMax.z) + fRadius * 1.1f;
			else
			    return true;
		}

		//�u���b�N�̉�======================================
		if (pInfo->pos.z + fRadius >= Mappos.z + vtxMin.z
			&& pInfo->posOld.z + fRadius <= Mappos.z + vtxMin.z)
		{
			if (nIdx == 3 || nIdx == 4 || nIdx == 5 || nIdx == 6)
				pInfo->pos.z = (Mappos.z + vtxMin.z) - fRadius;
			else
			    return true;
		}
	}

	return false;
}

bool CCollision::HitOBJ(CPlayer* pPlayer, CMapObject* pMapObject, float fMyRadius, float fTargetRadius)
{
	int nNum = 0;
	D3DXVECTOR3 Mappos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	CPlayer::INFO* pInfo = pPlayer->GetInfo();

	Mappos = pMapObject->GetPosition();

	vtxMin = pMapObject->GetVtxMin();

	vtxMax = pMapObject->GetVtxMax();

	int nIdx = pMapObject->GetIdxModel();

	D3DXMATRIX* mtx = {};

	// �v���C���[�̃��f�����擾
	CCharacter** ppChar = pPlayer->GetChar();

	if (ppChar == nullptr)
		return false;

	// �v���C���[�̉E���Ԃ��̃}�g���b�N�X���擾
	mtx = ppChar[9]->GetMtxWorld();

	float circleX = mtx->_41 - Mappos.x;
	float circleZ = mtx->_43 - Mappos.z;
	float c = 0.0f;

	c = sqrtf(circleX * circleX + circleZ * circleZ);

	if (c <= fMyRadius + fTargetRadius)
	{
		return true;
	}
	
	return false;
}

//=============================================================================
//�}�b�v�ɂ��錚���Ƃ̓����蔻��
//=============================================================================
void CCollision::MapEnemy(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, CObjectX ** pObjectX, CEnemy *penemy)
{
	
}

//=============================================================================
//�}�b�v�ɂ��錚���Ƃ̓����蔻��
//=============================================================================
bool CCollision::Item(D3DXVECTOR3 *pos)
{
	// �A�C�e���̐擪�擾
	CItem* pItem = CItem::GetTop();

	if (pItem == nullptr)
		return false;

	float fPlayerRadius = 50.0f;  // �v���C���[�̔��a
	float fItemRadius = 25.0f;    // �A�C�e���̔��a

	while (pItem != nullptr)
	{
		// ���̃A�C�e���擾
		CItem* pItemNext = pItem->GetNext();

		pItem->GetPosition();

		float circleX = pItem->GetPosition().x - pos->x;
		float circleZ = pItem->GetPosition().z - pos->z;
		float c = 0.0f;

		c = (float)sqrt(circleX * circleX + circleZ * circleZ);

		if (c <= fItemRadius + fPlayerRadius)
		{
			CCharacter** pChar = CGame::GetPlayer()->GetChar();
			pItem->SetCurrent(pChar[9]->GetMtxWorld());
			pItem->SetPosition(D3DXVECTOR3(50.0f, 5.0f, -15.0f));
			pItem->SetRotition(D3DXVECTOR3(0.0f, -D3DX_PI, -D3DX_PI * 0.5f));
			CGame::GetPlayer()->SetGrapItem(pItem);
			return true;
		}

		pItem = pItemNext;
	}

	return false;
}

//=============================================================================
// �A�C�e���������čU������ۂ̓����蔻��
//=============================================================================
void CCollision::ItemAttack(CObjectX * pobj)
{
	if (pobj == nullptr)
		return;

	int nNum = 0;
	float PlayerfRadius = 50.0f;
	float fRadius = 75.0f;
	CEnemy* pEnemy = CEnemy::GetTop();

	while (pEnemy != nullptr)
	{
		CEnemy* pEnemyNext = pEnemy->GetNext();

		float circleX = pEnemy->GetPosition().x - (CGame::GetPlayer()->GetPosition().x + pobj->GetPosition().x);
		float circleZ = pEnemy->GetPosition().z - (CGame::GetPlayer()->GetPosition().z + pobj->GetPosition().z);
		float c = 0.0f;

		c = (float)sqrt(circleX * circleX + circleZ * circleZ);

		if (c <= fRadius + PlayerfRadius)
		{
			pEnemy->SetRotition(-CGame::GetPlayer()->GetRotition());
			pEnemy->SetMove(D3DXVECTOR3(sinf(CGame::GetPlayer()->GetRotition().y) * -3.0f, 1.0f, cosf(CGame::GetPlayer()->GetRotition().y) * -3.0f));
			pEnemy->SetState(CEnemy::STATE_DAMEGE);
			int nLife = pEnemy->GetLife();
			nLife -= 1;
			pEnemy->SetLife(nLife);
		}
	}
}

//=============================================================================
// �A�C�e���ƓG�̔���
//=============================================================================
bool CCollision::ItemEnemy(CItem *pItem, CEnemy *pEnemy, float fMyRadius, float fTargetRadius, float fHeight)
{
	if (pEnemy != nullptr)
	{
		float circleX = pItem->GetMtxWorld()->_41 - pEnemy->GetPosition().x;
		float circleZ = pItem->GetMtxWorld()->_43 - pEnemy->GetPosition().z;
		float c = 0.0f;

		c = (float)sqrt(circleX * circleX + circleZ * circleZ);

		if (c <= fMyRadius + fTargetRadius && (pItem->GetPosition().y >= pEnemy->GetPosition().y && pItem->GetPosition().y <= pEnemy->GetPosition().y + fHeight) && pEnemy->GetState() != CEnemy::STATE_DAMEGE && pEnemy->GetState() != CEnemy::STATE_HEATDAMEGE)
		{
			//pEnemy->Damege(CPlayer::GetInstance()->GetMotion()->GetAttackDamege(), CPlayer::GetInstance()->GetMotion()->GetKnockBack(), CPlayer::GetInstance()->GetActType());

			CParticle::Create(pEnemy->GetPosition(), CParticle::TYPE_BLOOD);

			return true;
		}
	}
	
	return false;
}

//=============================================================================
//���̓����蔻��
//=============================================================================
D3DXVECTOR3 *CCollision::CheckEnemy(D3DXVECTOR3 * pMyPos, D3DXVECTOR3 * pMyOldPos, D3DXVECTOR3 * pTargetPos, float fMyRadius)
{
	if (pMyPos->x + fMyRadius >= pTargetPos->x - fMyRadius
	 && pMyPos->x - fMyRadius <= pTargetPos->x + fMyRadius
	 && pMyPos->z + fMyRadius >= pTargetPos->z - fMyRadius
	 && pMyPos->z - fMyRadius <= pTargetPos->z + fMyRadius)
	{
		// ����======================================
		if (pMyPos->z + fMyRadius >= pTargetPos->z - fMyRadius
		 && pMyOldPos->z + fMyRadius <= pTargetPos->z - fMyRadius)
		{
			pMyPos->z = pTargetPos->z - fMyRadius - fMyRadius;
		}

		// �w��======================================
		if (pMyPos->z - fMyRadius <= pTargetPos->z + fMyRadius
			&& pMyOldPos->z - fMyRadius >= pTargetPos->z + fMyRadius)
		{
			pMyPos->z = pTargetPos->z + fMyRadius + fMyRadius;
		}

		// ��==================================
		if (pMyPos->x + fMyRadius >= pTargetPos->x - fMyRadius
		 && pMyOldPos->x + fMyRadius <= pTargetPos->x - fMyRadius)
		{
			pMyPos->x = pTargetPos->x - fMyRadius - fMyRadius;
		}

		// �E==================================
		if (pMyPos->x - fMyRadius <= pTargetPos->x + fMyRadius
			&& pMyOldPos->x - fMyRadius >= pTargetPos->x + fMyRadius)
		{
			pMyPos->x = pTargetPos->x + fMyRadius + fMyRadius;
		}
	}	

	return pMyPos;
}