//=============================================================================
//
// 当たり判定処理 [collision.cpp]
// Author : 大原　怜将
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
#include "MyEffekseer.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CCollision *CCollision::m_pColli = nullptr;

//=============================================================================
//コンストラクタ
//=============================================================================
CCollision::CCollision()
{
	m_pColli = this;
	m_bColli = false;
}

//=============================================================================
//デストラクタ
//=============================================================================
CCollision::~CCollision()
{

}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CCollision::Init(void)
{
	return S_OK;
}

//=============================================================================
//終了処理
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
// 生成処理
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
//敵とオブジェクトの当たり判定処理
//=============================================================================
bool CCollision::Enemy(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, float fWidthX, float fWidthZ, CEnemy **pEnemy)
{
	return false;
}

//=============================================================================
//円の当たり判定処理
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
//円の当たり判定処理
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
			pEnemy->Damege(CPlayer::GetInstance()->GetMotion()->GetAttackDamege(), CPlayer::GetInstance()->GetMotion()->GetKnockBack(), CPlayer::GetInstance()->GetActType());
			CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_HIT, ::Effekseer::Vector3D(pMyPos->x, pMyPos->y, pMyPos->z),
				::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(25.0f, 25.0f, 25.0f));
		}

		pEnemy = pEnemyNext;
	}
}

//=============================================================================
//敵とオブジェクトの当たり判定処理
//=============================================================================
bool CCollision::Player(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, float fWidthX, float fWidthZ, CPlayer *pPlayer)
{
	return false;
}

//=============================================================================
//マップにある建物との当たり判定
//=============================================================================
void CCollision::Map(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, float fRadius)
{
	int nNum = 0;
	CObjectX **pMap = nullptr;

	D3DXVECTOR3 Mappos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (CGame::GetMap() != nullptr)
	{
		nNum = CGame::GetMap()->GetNum();
		pMap = CGame::GetMap()->GetObjectX();
	}

	if (CTutorial::GetMap() != nullptr)
	{
		nNum = CTutorial::GetMap()->GetNum();
		pMap = CTutorial::GetMap()->GetObjectX();
	}

	for (int nCount = 0; nCount < nNum; nCount++)
	{
		if (pMap[nCount] != nullptr)
		{
			Mappos = pMap[nCount]->GetPosition();

			vtxMin = pMap[nCount]->GetVtxMin();

			vtxMax = pMap[nCount]->GetVtxMax();
		}

		if (pMap[nCount]->IsEnable() == true)
		{
			if (pos->z + fRadius > Mappos.z + vtxMin.z
				&& pos->z + -fRadius < Mappos.z + vtxMax.z)
			{
				//ブロックの右側面==================================
				if (pos->x + -fRadius <= Mappos.x + vtxMax.x
					&& posOld->x + -fRadius >= Mappos.x + vtxMax.x)
				{
					pos->x = (Mappos.x + vtxMax.x) - -fRadius;

					//return true;
				}
				//ブロックの左側面==================================
				if (pos->x + fRadius >= Mappos.x + vtxMin.x
					&& posOld->x + fRadius <= Mappos.x + vtxMin.x)
				{
					pos->x = (Mappos.x + vtxMin.x) - fRadius;

					//return true;
				}
			}

			if (pos->x + fRadius > Mappos.x + vtxMin.x
			 && pos->x + -fRadius < Mappos.x + vtxMax.x)
			{
				//ブロックの上======================================
				if (pos->z + -fRadius <= Mappos.z + vtxMax.z
				 && posOld->z + -fRadius >= Mappos.z + vtxMax.z)
				{
					pos->z = (Mappos.z + vtxMax.z) - -fRadius;

					//return true;
				}

				//ブロックの下======================================
				if (pos->z + fRadius >= Mappos.z + vtxMin.z
				 && posOld->z + fRadius <= Mappos.z + vtxMin.z)
				{
					pos->z = (Mappos.z + vtxMin.z) - fRadius;

					//return true;
				}
			}
		}
	}

	//return false;
}

//=============================================================================
//剣の当たり判定
//=============================================================================
bool CCollision::Sword(D3DXMATRIX matrix1, D3DXMATRIX matrix2, float flength, CEnemy *pEnemy)
{
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX Matrix;
	D3DXVECTOR3 offPos = { 0.0f, 0.0f, -flength };
	D3DXVECTOR3 Pos = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 offRot = { 0.0f, 0.0f, 0.0f };

	//計算用マトリックス
	D3DXMATRIX m_mtxRot, m_mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&Matrix);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, offRot.y, offRot.x, offRot.z);

	D3DXMatrixMultiply(&Matrix, &Matrix, &m_mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&m_mtxTrans, offPos.x, offPos.y, offPos.z);

	D3DXMatrixMultiply(&Matrix, &Matrix, &m_mtxTrans);

	//算出したパーツのワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&Matrix, &Matrix, &matrix2);

	Pos.x = Matrix._41;
	Pos.y = Matrix._42;
	Pos.z = Matrix._43;

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &Matrix);

	if (pEnemy != nullptr)
	{
		D3DXVECTOR3 Enemypos = pEnemy->GetPosition();

		if ((matrix1._41 >= Enemypos.x - 30.0f && Pos.x <= Enemypos.x + 30.0f || matrix1._41 <= Enemypos.x + 30.0f && Pos.x >= Enemypos.x - 30.0f)
		&& ( matrix1._43 >= Enemypos.z - 30.0f && Pos.z <= Enemypos.z + 30.0f || matrix1._43 <= Enemypos.z + 30.0f && Pos.z >= Enemypos.z - 30.0f))
		{
			pEnemy->SetState(CEnemy::STATE_DAMEGE);
			pEnemy->SetMove(D3DXVECTOR3(sinf(CGame::GetPlayer()->GetRotition().y), 1.0f, cosf(CGame::GetPlayer()->GetRotition().y)));
			return true;
		}
	}
	
	return false;
}

//=============================================================================
//マップにある建物との当たり判定
//=============================================================================
void CCollision::MapEnemy(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, CObjectX ** pObjectX, CEnemy *penemy)
{
	
}

//=============================================================================
//マップにある建物との当たり判定
//=============================================================================
bool CCollision::Item(D3DXVECTOR3 *pos)
{
	// アイテムの先頭取得
	CItem* pItem = CItem::GetTop();

	if (pItem == nullptr)
		return false;

	float fPlayerRadius = 50.0f;  // プレイヤーの半径
	float fItemRadius = 25.0f;    // アイテムの半径

	while (pItem != nullptr)
	{
		// 次のアイテム取得
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
// マップにある建物との当たり判定
//=============================================================================
void CCollision::ItemAttack(CObjectX * pobj)
{
	int nNum = 0;
	float PlayerfRadius = 50.0f;
	float fRadius = 75.0f;
	CEnemy **ppEnemy = nullptr;

	if (CGame::GetEnemyManager() != nullptr)
	{
		ppEnemy = CGame::GetEnemyManager()->GetEnemy();
		nNum = CGame::GetEnemyManager()->GetNum();
	}

	if (pobj != nullptr)
	{
		for (int nCount = 0; nCount < nNum; nCount++)
		{
			if (ppEnemy[nCount] != nullptr)
			{
				float circleX = ppEnemy[nCount]->GetPosition().x - (CGame::GetPlayer()->GetPosition().x + pobj->GetPosition().x);
				float circleZ = ppEnemy[nCount]->GetPosition().z - (CGame::GetPlayer()->GetPosition().z + pobj->GetPosition().z);
				float c = 0.0f;

				c = (float)sqrt(circleX * circleX + circleZ * circleZ);

				if (c <= fRadius + PlayerfRadius)
				{
					ppEnemy[nCount]->SetRotition(-CGame::GetPlayer()->GetRotition());
					ppEnemy[nCount]->SetMove(D3DXVECTOR3(sinf(CGame::GetPlayer()->GetRotition().y) * -3.0f, 1.0f, cosf(CGame::GetPlayer()->GetRotition().y) * -3.0f));
					ppEnemy[nCount]->SetState(CEnemy::STATE_DAMEGE);
					int nLife = ppEnemy[nCount]->GetLife();
					nLife -= 1;
					ppEnemy[nCount]->SetLife(nLife);
				}
			}
		}
	}
}

//=============================================================================
// アイテムと敵の判定
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
			// 敵の番号を保存
			pEnemy->Damege(CPlayer::GetInstance()->GetMotion()->GetAttackDamege(), CPlayer::GetInstance()->GetMotion()->GetKnockBack(), CPlayer::GetInstance()->GetActType());

			CParticle::Create(pEnemy->GetPosition(), CParticle::TYPE_BLOOD);

			return true;
		}
	}
	
	return false;
}

//=============================================================================
//剣の当たり判定
//=============================================================================
D3DXVECTOR3 *CCollision::CheckEnemy(D3DXVECTOR3 * pMyPos, D3DXVECTOR3 * pMyOldPos, D3DXVECTOR3 * pTargetPos, float fMyRadius)
{
	if (pMyPos->x + fMyRadius >= pTargetPos->x - fMyRadius
	 && pMyPos->x - fMyRadius <= pTargetPos->x + fMyRadius
	 && pMyPos->z + fMyRadius >= pTargetPos->z - fMyRadius
	 && pMyPos->z - fMyRadius <= pTargetPos->z + fMyRadius)
	{
		// 正面======================================
		if (pMyPos->z + fMyRadius >= pTargetPos->z - fMyRadius
		 && pMyOldPos->z + fMyRadius <= pTargetPos->z - fMyRadius)
		{
			pMyPos->z = pTargetPos->z - fMyRadius - fMyRadius;
		}

		// 背面======================================
		if (pMyPos->z - fMyRadius <= pTargetPos->z + fMyRadius
			&& pMyOldPos->z - fMyRadius >= pTargetPos->z + fMyRadius)
		{
			pMyPos->z = pTargetPos->z + fMyRadius + fMyRadius;
		}

		// 左==================================
		if (pMyPos->x + fMyRadius >= pTargetPos->x - fMyRadius
		 && pMyOldPos->x + fMyRadius <= pTargetPos->x - fMyRadius)
		{
			pMyPos->x = pTargetPos->x - fMyRadius - fMyRadius;
		}

		// 右==================================
		if (pMyPos->x - fMyRadius <= pTargetPos->x + fMyRadius
			&& pMyOldPos->x - fMyRadius >= pTargetPos->x + fMyRadius)
		{
			pMyPos->x = pTargetPos->x + fMyRadius + fMyRadius;
		}
	}	

	return pMyPos;
}
