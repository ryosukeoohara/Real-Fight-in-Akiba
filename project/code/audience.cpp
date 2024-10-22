//===========================================================
//
// 周りで見てる人[audience.cpp]
// Author 大原怜将
//
//===========================================================
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "audience.h"
#include "objectX.h"
#include "debugproc.h"
#include "object.h"
#include "object3D.h"
#include "collision.h"
#include "player.h"
#include "mapobject_can.h"
#include "utility.h"

//================================================================
// 静的メンバ変数
//================================================================
CAudience* CAudience::m_pTop = nullptr;
CAudience* CAudience::m_pCur = nullptr;

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const int JUMP_WAIT_TIME = 60;  // 再びジャンプできるようになるまでの時間
	const int THROW_WAIT_TIME = 60;  // 再び缶を投げられるようになるまでの時間
	const float JUMP = 8.0f;      // ジャンプの高さ
	const float GRAVITY = -1.0f;  // 重力
}

//================================================================
// コンストラクタ
//================================================================
CAudience::CAudience()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nJumpWaitTime = 0;
	m_nThrowWaitTime = 0;
	m_bJump = false;
	m_bThrow = false;

	if (m_pTop != nullptr)
	{// 先頭が存在している場合

		m_pCur->m_pNext = this;
		m_pPrev = m_pCur;
		m_pCur = this;
	}
	else
	{// 存在しない場合

		m_pTop = this;
		m_pCur = this;
	}
}

//================================================================
// コンストラクタ(オーバーロード)
//================================================================
CAudience::CAudience(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filename) : CObjectX(filename)
{
	SetPosition(pos);
	SetRotition(rot);

	m_pos = pos;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nJumpWaitTime = 0;
	m_bJump = false;

	if (m_pTop != nullptr)
	{// 先頭が存在している場合

		m_pCur->m_pNext = this;
		m_pPrev = m_pCur;
		m_pCur = this;
	}
	else
	{// 存在しない場合

		m_pTop = this;
		m_pCur = this;
	}
}

//================================================================
// デストラクタ
//================================================================
CAudience::~CAudience()
{

}

//================================================================
// 生成処理
//================================================================
CAudience *CAudience::Create(void)
{
	//オブジェクト2Dのポインタ
	CAudience *pAudience = nullptr;

	if (pAudience == nullptr)
	{
		//オブジェクト2Dの生成
		pAudience = new CAudience();

		//初期化処理
		pAudience->Init();
	}
	
	return pAudience;
}

//================================================================
// 生成処理
//================================================================
CAudience* CAudience::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filename)
{
	//オブジェクト2Dのポインタ
	CAudience* pAudience = nullptr;

	if (pAudience == nullptr)
	{
		// 生成
		pAudience = new CAudience(pos, rot, filename);
		pAudience->m_pos = pos;

		// 初期化処理
		pAudience->Init();
	}
	
	return pAudience;
}

//================================================================
// 初期化処理
//================================================================
HRESULT CAudience::Init(void)
{
	CObjectX::Init();

	// 丸影の生成
	CObject3D* pShadow = CObject3D::Create();

	if (pShadow != nullptr)
	{
		pShadow->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\shadow000.jpg"));
		pShadow->SetSize(20.0f, 20.0f);
		pShadow->SetTexPosition(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
		pShadow->SetPosition(D3DXVECTOR3(m_pos.x, 1.0f, m_pos.z));
		pShadow->SetbSubBlend(true);
		pShadow->SetDraw(true);
	}
	
	return S_OK;
}

//================================================================
// 終了処理
//================================================================
void CAudience::Uninit(void)
{
	CAudience* pObject = m_pTop;

	while (pObject != nullptr)
	{
		CAudience* pObjectNext = pObject->m_pNext;

		CObjectX::Uninit();
		pObject = nullptr;

		pObject = pObjectNext;
	}
}

//================================================================
// 更新処理
//================================================================
void CAudience::Update(void)
{
	CObjectX::Update();

	// 重力を与える
	m_move.y += GRAVITY;

	if (m_bJump == false)
	{// ジャンプしていないとき

		int i = rand() % 3;
		
		if (i == 1)
		{
			float fjump = (rand() % 31 - 10) * 0.1f;
			// ジャンプさせて再びジャンプできるまでの時間を設定
			m_bJump = true;
			m_move.y = JUMP + fjump;
			m_nJumpWaitTime = JUMP_WAIT_TIME;
		}
	}

	m_pos += m_move;
	
	// 地面にめり込まないように
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
	}

	// 再びジャンプできるようになるまでカウント
	if (m_bJump == true)
	{
		m_nJumpWaitTime--;
	}

	// 0になったら再びジャンプできるようにする
	if (m_nJumpWaitTime <= 0)
	{
		m_bJump = false;
	}

	CCollision *pColl = CCollision::GetInstance();

	// 位置設定
	SetPosition(m_pos);

	if (pColl == nullptr)
		return;

	float fDistance = utility::Distance(m_pos, CPlayer::GetInstance()->GetPosition());

	// プレイヤーが範囲内に入ったかつ、缶を投げていない
	if (fDistance <= 300.0f && !m_bThrow)
	{
		int i = rand() % 7;

		if (i == 1)
		{
			CMapObject_Can* pCan = CMapObject_Can::Create("data\\MODEL\\map\\00_can.x");
			pCan->SetPosition(D3DXVECTOR3(m_pos.x, m_pos.y + 50.0f, m_pos.z));
			D3DXVECTOR3 rot = GetRotition();
			pCan->SetRotition(rot);
			pCan->ChangeState(new CCanThrow);
		}

		m_bThrow = true;
		m_nThrowWaitTime = THROW_WAIT_TIME;
	}

	// 缶を投げた
	if (m_bThrow)
		m_nThrowWaitTime--;

	// 再び投げられるようにする
	if (m_nThrowWaitTime <= 0)
		m_bThrow = false;
		
}

//================================================================
// 描画処理
//================================================================
void CAudience::Draw(void)
{
	CObjectX::Draw();
}