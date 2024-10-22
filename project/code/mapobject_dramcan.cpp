//===========================================================
//
// マップに配置れているドラム缶処理[mapobject_dramcan.cpp]
// Author 大原怜将
//
//===========================================================
#include "mapobject_dramcan.h"
#include "mapobject.h"
#include "object3D.h"
#include "collision.h"
#include "player.h"
#include "manager.h"
#include "texture.h"
#include "ripples.h"
#include "manager.h"
#include "sound.h"
#include "utility.h"

//===========================================================
// 静的メンバ変数
//===========================================================
//CMapObject_Dramcan* CMapObject_Dramcan::m_pDramcan = nullptr;  // 自分自身
CMapObject_Dramcan* CMapObject_Dramcan::m_pTop = nullptr;      // 先頭のポインタ
CMapObject_Dramcan* CMapObject_Dramcan::m_pCur = nullptr;      // 最後尾のポインタ

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const char* TEXTURE_PATH = "data\\TEXTURE\\water.png";  // 水たまりのテクスチャ
	const D3DXVECTOR2 TARGET_POS = { 100.0f, 100.0f };        // 目標の値
	const float LERP_SPEED = 0.0015f;                        // 目標の値までの補正係数
}

//===========================================================
// コンストラクタ
//===========================================================
CMapObject_Dramcan::CMapObject_Dramcan()
{
	//m_pDramcan = this;
	m_pState = nullptr;

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
CMapObject_Dramcan::CMapObject_Dramcan(const char* aModelFliename, int nPriority) : CMapObject(aModelFliename, nPriority)
{
	//m_pDramcan = this;
	m_pState = nullptr;

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
CMapObject_Dramcan::~CMapObject_Dramcan()
{

}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CMapObject_Dramcan::Init(void)
{
	CMapObject::Init();

	m_pState = new CDramcanNeutral;

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CMapObject_Dramcan::Uninit(void)
{
	CMapObject::Uninit();
}

//===========================================================
// 更新処理
//===========================================================
void CMapObject_Dramcan::Update(void)
{
	CMapObject::Update();

	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// プレイヤーが攻撃をしていて、その攻撃が発生していたら
	if (pPlayer->GetState() == CPlayer::STATE_ATTACK && pPlayer->IsHitCollision())
	{
		// 攻撃が当たっていたら
		if (CCollision::GetInstance()->AttackHitCheck(pPlayer, this, 30.0f, 30.0f))
		{
			// ステートの変更：殴られた状態
			ChangeState(new CDramcanBlowAway);

			CSound* pSound = CManager::GetInstance()->GetSound();

			if (pSound != nullptr)
				pSound->Play(CSound::SOUND_LABEL_SE_IRON_ROT_HIT);
		}
	}

	//CCollision::GetInstance()->CheckPlayerMapObject(pPlayer, this, 20.0f);

	// ステートの変更
	if (m_pState != nullptr)
		m_pState->Update(this);
}

//===========================================================
// 描画処理
//===========================================================
void CMapObject_Dramcan::Draw(void)
{
	CMapObject::Draw();
}

//===========================================================
// 生成処理
//===========================================================
CMapObject_Dramcan* CMapObject_Dramcan::Create(const char* aModelFliename, int nPriority)
{
	CMapObject_Dramcan* pTrashBox = new CMapObject_Dramcan(aModelFliename, nPriority);

	if (pTrashBox != nullptr)
	{
		pTrashBox->Init();
	}

	return pTrashBox;
}

//===========================================================
// ステイトの変更処理
//===========================================================
void CMapObject_Dramcan::ChangeState(CDramcanState* pState)
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
//===========================================================
// ニュートラル状態の処理
//===========================================================
CDramcanNeutral::CDramcanNeutral()
{

}

//===========================================================
// ニュートラル状態の更新処理
//===========================================================
void CDramcanNeutral::Update(CMapObject_Dramcan* pDramcan)
{

}

//===========================================================
// 吹き飛ぶ状態の処理
//===========================================================
CDramcanBlowAway::CDramcanBlowAway()
{

}

//===========================================================
// 吹き飛ぶ状態の更新処理
//===========================================================
void CDramcanBlowAway::Update(CMapObject_Dramcan* pDramcan)
{
	D3DXVECTOR3 Playerrot = CPlayer::GetInstance()->GetRotition();
	D3DXVECTOR3 Playermove = CPlayer::GetInstance()->GetMove();

	CObjectX::INFO* pInfo = pDramcan->GetInfo();

	pInfo->rot.x = 1.57f;
	pInfo->rot.y = Playerrot.y;
	pInfo->rot.z = 1.57f;

	pInfo->move.x += (Playermove.x * 1.2f);
	pInfo->move.y += 1.0f;
	pInfo->move.z += (Playermove.z * 1.2f);

	if (pInfo->pos.y - pInfo->vtxMax.x >= 0.0f)
	{
		pInfo->pos.y -= 1.0f;
		pDramcan->ChangeState(new CDramcanWaterLeak);
	}
		
	if (pInfo->pos.y <= 0.0f)
	{
		pInfo->pos.y = 1.0f;
		pDramcan->ChangeState(new CDramcanWaterLeak);
	}

	pInfo->pos += pInfo->move;

	// 移動量を更新(減衰させる)
	pInfo->move.x += (0.0f - pInfo->move.x) * 0.1f;
	pInfo->move.y += (0.0f - pInfo->move.y) * 0.1f;
	pInfo->move.z += (0.0f - pInfo->move.z) * 0.1f;
}

//===========================================================
// 水が漏れる状態の処理
//===========================================================
CDramcanWaterLeak::CDramcanWaterLeak()
{

}

//===========================================================
// 水が漏れる状態の更新処理
//===========================================================
void CDramcanWaterLeak::Update(CMapObject_Dramcan* pDramcan)
{
	CObjectX::INFO* pInfo = pDramcan->GetInfo();

	// 水たまりを生成
	if (m_pPuddle == nullptr)
	{
		m_pPuddle = CObject3D::Create();
		m_pPuddle->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist(TEXTURE_PATH));
		m_pPuddle->SetSize(1.0f, 1.0f);
		m_pPuddle->SetColor({ 1.0f, 1.0f, 1.0f, 0.8f });
		m_pPuddle->SetDraw(true);
		m_pPuddle->SetPosition(D3DXVECTOR3(pInfo->pos.x, 0.5f, pInfo->pos.z));
	}

	// 目標の値まで補正
	if (m_pPuddle != nullptr)
		m_pPuddle->EaseToTarget(TARGET_POS, LERP_SPEED);
		
	// プレイヤーの情報取得
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	// プレイヤーの位置取得
	D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();

	// プレイヤーが範囲内に入ったとき、波紋エフェクトを生成する
	if (CCollision::GetInstance()->Circle(m_pPuddle->GetPosition(), PlayerPos, 75.0f, 50.0f))
		pPlayer->CreateRippleshEffect();

}