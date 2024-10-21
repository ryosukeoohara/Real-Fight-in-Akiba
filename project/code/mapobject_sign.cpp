//===========================================================
//
// マップに配置れている標識	処理[mapobject_sign.cpp]
// Author 大原怜将
//
//===========================================================
#include "mapobject_sign.h"
#include "player.h"
#include "collision.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CMapObject_Sign* CMapObject_Sign::m_pTop = nullptr;      // 先頭のポインタ
CMapObject_Sign* CMapObject_Sign::m_pCur = nullptr;      // 最後尾のポインタ

//===========================================================
// 定数定義
//===========================================================
namespace
{
	int FALL_DOWN = 3;  // ３回殴られたら倒れる
	int SHAKE_TIME = 60;  // 殴られてから揺れている時間
}

//===========================================================
// コンストラクタ
//===========================================================
CMapObject_Sign::CMapObject_Sign()
{
	if (m_pTop != nullptr)
	{// 先頭が存在している場合

		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合

		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}
}

//===========================================================
// コンストラクタ
//===========================================================
CMapObject_Sign::CMapObject_Sign(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
{
	if (m_pTop != nullptr)
	{// 先頭が存在している場合

		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合

		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}
}

//===========================================================
// デストラクタ
//===========================================================
CMapObject_Sign::~CMapObject_Sign()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMapObject_Sign::Init(void)
{
	CMapObject::Init();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject_Sign::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CMapObject_Sign::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// ステートの更新
	if (m_pState != nullptr)
		m_pState->Update(this);

	// プレイヤーとの当たり判定：プレイヤーが範囲内に入った時
	CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f);

	// プレイヤーが攻撃をしていて、その攻撃が発生していたら
	if (pPlayer->GetState() == CPlayer::STATE_ATTACK && pPlayer->IsHitCollision())
	{
		// 攻撃が当たっていたら
		if (CCollision::GetInstance()->AttackHitCheck(pPlayer, this, 30.0f, 30.0f))
		{
			// ステートの変更：殴られた状態
			ChangeState(new CSignBeaten);
		}
	}
}

//===========================================================
// 描画処理
//===========================================================
void CMapObject_Sign::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// 生成処理
//===========================================================
CMapObject_Sign* CMapObject_Sign::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_Sign* pTrashBox = new CMapObject_Sign(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// ステートの変更処理
//===========================================================
void CMapObject_Sign::ChangeState(CSignState* pState)
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	m_pState = pState;
}

//=======================================================================
// ステート
//=======================================================================
CSignBeaten::CSignBeaten()
{
	m_nShakeTimeCounter = 0;
	m_nFallDownCount = 0;
	m_fShakeAngle = 0.0f;
}

void CSignBeaten::Update(CMapObject_Sign* pElectricBox)
{
	CObjectX::INFO* pInfo = pElectricBox->GetInfo();

	// sin を使用した揺らし座標の算出
	pInfo->move.y = 0.0f;

	pInfo->move.x = sinf(m_fShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter / SHAKE_TIME)) * 2.0f;

	pInfo->move.z = sinf(m_fShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter / SHAKE_TIME)) * 2.0f;

	// 揺らし処理に使用する sin に渡す角度の変更処理
	m_fShakeAngle += 2.0f * 1.0f;

	// 揺らす時間が経過したら揺らし処理を終了する
	m_nShakeTimeCounter += 1;
	if (m_nShakeTimeCounter >= SHAKE_TIME)
	{
		m_nShakeTimeCounter = 0;
		m_nFallDownCount++;

		// ステートの変更：ニュートラル
		pElectricBox->ChangeState(nullptr);
	}

	pInfo->pos += pInfo->move;

	// 移動量を更新(減衰させる)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}