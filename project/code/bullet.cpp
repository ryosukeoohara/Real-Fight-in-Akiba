//===========================================================
//
// 敵が投げてくる缶[bullet.cpp]
// Author 大原怜将
//
//===========================================================
#include "manager.h"
#include "player.h"
#include "sound.h"
#include "bullet.h"
#include "enemy.h"
#include "effect.h"
#include "collision.h"
#include "MyEffekseer.h"

//================================================================
// 定数定義
//================================================================
namespace
{
	const float WIDTH = 10.0f;       // 幅
	const float HEIGHT = 10.0f;      // 高さ
	const float SPEED = 7.0f;        // 移動スピード
	const float KNOCK_BACK = 5.0f;   // プレイヤーがノックバックする値
	const int LIFE = 120;            // 体力
	const int DAMAGE = 10;           // 攻撃力
	const char* MODEL_PATH = "data\\MODEL\\01_can.x";  // モデルのパス
}

//================================================================
// 静的メンバ変数宣言
//================================================================
CBullet *CBullet::m_Bullet = NULL;

//================================================================
// コンストラクタ
//================================================================
CBullet::CBullet()
{
	m_nLife = 0;
	m_move = { 0.0f, 0.0f, 0.0f };  //移動量
}

//================================================================
// コンストラクタ
//================================================================
CBullet::CBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	SetPosition(pos);
	SetRotition(rot);
	
	m_nLife = 0;
	m_move = { 0.0f, 0.0f, 0.0f };  //移動量
}

//================================================================
// デストラクタ
//================================================================
CBullet::~CBullet()
{

}

//================================================================
// 生成処理
//================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//オブジェクト2Dのポインタ
	CBullet *pBullet = nullptr;

	if (pBullet == nullptr)
	{
		//オブジェクト2Dの生成
		pBullet = new CBullet(pos, rot);

		//初期化処理
		pBullet->Init();
	}
	
	return pBullet;
}

//================================================================
// 初期化処理
//================================================================
HRESULT CBullet::Init(void)
{
	SetModelFileName(MODEL_PATH);

	//初期化処理
	CObjectX::Init();

	//弾の寿命
	m_nLife = LIFE;

	return S_OK;
}

//================================================================
// 終了処理
//================================================================
void CBullet::Uninit(void)
{
	//終了処理
	CObjectX::Uninit();
}

//================================================================
// 更新処理
//================================================================
void CBullet::Update(void)
{
	// 当たり判定の情報取得
	CCollision* pCollision = CCollision::GetInstance();

	// プレイヤーの情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	// 位置を代入
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 rot = GetRotition();

	// 前回の位置を記録
	m_posOld = pos;

	// 位置を更新
	pos.x -= sinf(rot.y) * SPEED;
	pos.z -= cosf(rot.y) * SPEED;

	// 更新処理
	CObjectX::Update();

	// 寿命を減らす
	m_nLife--;

	if (pPlayer != nullptr)
	{
		// プレイヤーとの当たり判定
		if (pCollision->Circle(pos, pPlayer->GetPosition(), WIDTH, 50.0f))
		{
			// 寿命をなくす
			m_nLife = 0;

			// プレイヤーのダメージ処理
			pPlayer->Damage(DAMAGE, D3DXVECTOR3(sinf(rot.y) * KNOCK_BACK, 0.0f, cosf(rot.y) * KNOCK_BACK));

			// エフェクトの再生
			CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_HIT, ::Effekseer::Vector3D(pos.x, pos.y, pos.z));
		}
	}

	pos.y -= 1.0f;
		
	if (m_nLife <= 0 || pos.y <= 0.0f)
	{// 寿命がなくなったら

		// 終了処理
		Uninit();

		return;
	}

	// 位置を設定
	SetPosition(pos);
}

//================================================================
// 描画処理
//================================================================
void CBullet::Draw(void)
{
	//描画処理
	CObjectX::Draw();
}