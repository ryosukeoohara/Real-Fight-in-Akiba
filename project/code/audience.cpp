//===========================================================
//
// 周りで見てる人[audience.cpp]
// Author 大原怜将
//
//===========================================================
#include "renderer.h"
#include "manager.h"
#include "audience.h"
#include "objectX.h"
#include "debugproc.h"
#include "object.h"

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
	const int JUMPWAITTIME = 60;  // 再びジャンプできるようになるまでの時間
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
			m_nJumpWaitTime = JUMPWAITTIME;
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

	// 位置設定
	SetPosition(m_pos);
}

//================================================================
// 描画処理
//================================================================
void CAudience::Draw(void)
{
	CObjectX::Draw();
}