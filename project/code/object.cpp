//===========================================================
//
// オブジェクト処理[object.cpp]
// Author 大原怜将
//
//===========================================================
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "camera.h"
#include "MyEffekseer.h"

//===========================================================
// 静的メンバ変数
//===========================================================
CObject *CObject::m_pTop[NUM_PRIORITY] = {};
CObject *CObject::m_pCur[NUM_PRIORITY] = {};
int CObject::m_nNumAll = 0;

//===========================================================
// コンストラクタ
//===========================================================
CObject::CObject(int nPriority)
{
	m_bDeath = false;
	m_pNext = nullptr;
	m_pPrev = nullptr;

	this->SetPriority(nPriority);

	CObject *pObject = m_pTop[nPriority];

	if (m_pTop[nPriority] != nullptr)
	{// 先頭が存在している場合

		m_pCur[nPriority]->m_pNext = this;
		m_pPrev = m_pCur[nPriority];
		m_pCur[nPriority] = this;
	}
	else
	{// 存在しない場合

		m_pTop[nPriority] = this;
		m_pCur[nPriority] = this;
	}
}

//===========================================================
// デストラクタ
//===========================================================
CObject::~CObject()
{

}

//===========================================================
// すべて破棄
//===========================================================
void CObject::ReleaseAll(void)
{
	for (int i = 0; i < NUM_PRIORITY; i++)
	{
		CObject *pObject = m_pTop[i];

		while (pObject != nullptr)
		{
			CObject *pObjectNext = pObject->m_pNext;

			pObject->Uninit();

			pObject = pObjectNext;
		}
	}
}

//===========================================================
// すべて更新
//===========================================================
void CObject::UpdateAll(void)
{
	DeadChuck();

	for (int i = 0; i < NUM_PRIORITY; i++)
	{
		CObject *pObject = m_pTop[i];

		while (pObject != nullptr)
		{
			CObject *pObjectNext = pObject->m_pNext;

			pObject->Update();
			pObject = pObjectNext;
		}
	}

	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	pDebugProc->Print("\n\nオブジェクトの総数：%d", m_nNumAll);
}

//===========================================================
// すべて描画
//===========================================================
void CObject::DrawAll(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	DeadChuck();

	//カメラの設定
	if(pCamera != nullptr)
	   pCamera->SetCamera();

	for (int i = 0; i < NUM_PRIORITY; i++)
	{
		CObject *pObject = m_pTop[i];

		while (pObject != nullptr)
		{
			CObject *pObjectNext = pObject->m_pNext;

			pObject->Draw();
			pObject = pObjectNext;
		}

		if (i == 3)
		{
			if(CManager::GetInstance()->GetMyEffekseer() != nullptr)
			   CManager::GetInstance()->GetMyEffekseer()->Draw();

		}
	}
}

//===========================================================
// 破棄
//===========================================================
void CObject::Release(void)
{
	// 死んだ
	m_bDeath = true;
}

//===========================================================
// 死亡しているかチェック
//===========================================================
void CObject::DeadChuck(void)
{
	for (int i = 0; i < NUM_PRIORITY; i++)
	{
		CObject *pObject = m_pTop[i];

		while (pObject != nullptr)
		{
			CObject *pObjectNext = pObject->m_pNext;

			if (pObject->m_bDeath == true)
			{
				// リストから自分自身を削除する
				if (m_pTop[i] == pObject)
				{// 自身が先頭

					if (pObject->m_pNext != nullptr)
					{// 次が存在している

						m_pTop[i] = pObject->m_pNext;	// 次を先頭にする
						pObject->m_pNext->m_pPrev = nullptr;	// 次の前のポインタを覚えていないようにする
					}
					else
					{// 存在していない

						m_pTop[i] = nullptr;	// 先頭がない状態にする
						m_pCur[i] = nullptr;	// 最後尾がない状態にする
					}
				}
				else if (m_pCur[i] == pObject)
				{// 自身が最後尾

					if (pObject->m_pPrev != nullptr)
					{// 次が存在している

						m_pCur[i] = pObject->m_pPrev;			// 前を最後尾にする
						pObject->m_pPrev->m_pNext = nullptr;	// 前の次のポインタを覚えていないようにする
					}
					else
					{// 存在していない

						m_pTop[i] = nullptr;	// 先頭がない状態にする
						m_pCur[i] = nullptr;	// 最後尾がない状態にする
					}
				}
				else
				{
					if (pObject->m_pNext != nullptr)
					{
						pObject->m_pNext->m_pPrev = pObject->m_pPrev;	// 自身の次に前のポインタを覚えさせる
					}
					if (pObject->m_pPrev != nullptr)
					{
						pObject->m_pPrev->m_pNext = pObject->m_pNext;	// 自身の前に次のポインタを覚えさせる
					}
				}
			}

			pObject = pObjectNext;
		}
	}
}