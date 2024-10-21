//===========================================================
//
//ポリゴンを出すやつ[bullet.h]
//Author 大原怜将
//
//===========================================================
#ifndef _BULLET_H_             //このマクロ定義がされてなかったら
#define _BULLET_H_             //2重インクルード防止のマクロ定義をする

#include "object2D.h"
#include "objectX.h"

//弾クラス
class CBullet : public CObjectX
{
public:

	typedef enum
	{
		TYPE_NONE = 0,  //何もない
		TYPE_PLAYER,    //プレイヤー
		TYPE_ENEMY,     //敵
		TYPE_BOM,       //爆弾
		TYPE_MAX
	} TYPE;

	CBullet();  //コンストラクタ
	CBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot);  //コンストラクタ(オーバーロード)
	~CBullet();  //デストラクタ

	HRESULT Init(void);   // 初期化処理    
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理
	void Draw(void);      // 描画処理

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);  //生成
	//static HRESULT Load(void);  //テクスチャの生成(読み込み)
	//static void UnLoad(void);   //テクスチャの破棄
	static CBullet *GetBullet(void) { return m_Bullet; };  //弾を取得

	void SetLife(int nLife) { m_nLife = nLife; }  //寿命取得
	int GetLife(void) { return m_nLife; }  //寿命設定

private:
	//void ControlPlayer(void);  //プレイヤーの制御
	//static LPDIRECT3DTEXTURE9 m_pTexture;        //テクスチャへのポインタ
	D3DXVECTOR3 m_move; //移動
	D3DXVECTOR3 m_posOld; //前回の位置
	int m_nLife;  //寿命
	int m_nIdxTexture;
	TYPE m_type;  //種類
	static CBullet *m_Bullet;

	bool CollisionEnemy(D3DXVECTOR3 pos);  //敵との当たり判定
};

#endif