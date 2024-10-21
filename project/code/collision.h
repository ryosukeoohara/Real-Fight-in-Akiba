//===========================================================
//
// 当たり判定処理 [collision.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _CONLLISION_H_
#define _CONLLISION_H_

//===========================================================
// 前方宣言
//===========================================================
class CEnemyManager;
class CPlayer;
class CEnemy;
class CBullet;
class CObjectX;
class CItem;
class CMapObject;

//===========================================================
// コリジョンクラス定義
//===========================================================
class CCollision
{
public:
	CCollision();
	~CCollision();

	HRESULT Init(void);
	void Uninit(void);

	static CCollision *Create(void);

	bool Enemy(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, float fWidthX, float fWidthZ, CEnemy **pEnemy);    // 敵との当たり判定(プレイヤー基準)
	bool Circle(D3DXVECTOR3 pMyPos, D3DXVECTOR3 pTargetPos, float fMyRadius, float fTargetRadius);     // 円の当たり判定
	void AttackCircle(D3DXVECTOR3 *pMyPos, float fMyRadius, float fTargetRadius, float fHeight);         // 円の当たり判定
	bool Player(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, float fWidthX, float fWidthZ, CPlayer *pPlayer);  // ぷれいやーとの当たり判定
	void Map(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, float fRadius);                                      // マップにある建物との当たり判定
	bool HitMapObject(D3DXVECTOR3 pMyPos, D3DXVECTOR3 pTargetPos, float fMyRadius, float fTargetRadius);
	bool HitElectricBox(D3DXVECTOR3 pMyPos, D3DXVECTOR3 pTargetPos, float fMyRadius, float fTargetRadius);
	void MapObject(D3DXVECTOR3* pos, D3DXVECTOR3* posOld, float fRadius);
	bool CheckPlayerMapObject(CPlayer* pPlayer, CMapObject* pMapObject, float fRadius);
	bool AttackHitCheck(CPlayer* pPlayer, CMapObject* pMapObject, float fMyRadius, float fTargetRadius);
	void MapEnemy(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, CObjectX **pObjectX, CEnemy *enemy);            // マップにある建物との当たり判定
	bool Item(D3DXVECTOR3 *pos);
	void ItemAttack(CObjectX *pobj);
	bool ItemEnemy(CItem *pItem, CEnemy *pEnemy, float fMyRadius, float fTargetRadius, float fHeight);   // アイテムと敵の判定
	D3DXVECTOR3 *CheckEnemy(D3DXVECTOR3 *pMyPos, D3DXVECTOR3 *pMyOldPos, D3DXVECTOR3 *pTargetPos, float fMyRadius);                      // 敵との当たり判定
	bool aaa(CMapObject* p);

	void SetbColli(bool bValue) { m_bColli = bValue; }
	bool GetbColli(void) { return m_bColli; }
	static CCollision *GetInstance(void) { return m_pColli; }
	
private:
	static CCollision *m_pColli;
	bool m_bColli;

};

#endif // !_CONLLISION_H_