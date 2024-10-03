//===========================================================
//
// モデル処理[objectX.cpp]
// Author : 大原　怜将
//
//===========================================================
#include "main.h"
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "texture.h"
#include "player.h"

//===========================================================
// コンストラクタ
//===========================================================
CObjectX::CObjectX()
{
	//値をクリア
	m_Info.vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_Info.vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	m_Info.vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_Info.vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);

	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_Info.Fliename = "a";
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	m_bDraw = true;
}

//===========================================================
// コンストラクタ
//===========================================================
CObjectX::CObjectX(const char *aModelFliename, int nPriority) : CObject(nPriority)
{
	//値をクリア
	m_Info.vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_Info.vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	m_Info.vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_Info.vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);

	m_Info.Fliename = aModelFliename;
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	m_bDraw = true;
}

//===========================================================
// デストラクタ
//===========================================================
CObjectX::~CObjectX()
{

}

//===========================================================
// 生成処理
//===========================================================
CObjectX *CObjectX::Create(const char *aModelFliename, int nPriority)
{
	//オブジェクトXのポインタ
	CObjectX *pObjectX = NULL;

	//モデル情報を取得
	CModel *pModel = CManager::GetInstance()->GetModel();

	if (pObjectX == NULL)
	{
		//オブジェクト2Dの生成
		pObjectX = new CObjectX(aModelFliename, nPriority);

		//初期化処理
		pObjectX->Init();
	}
	
	return pObjectX;
}

//===========================================================
// 初期化処理
//===========================================================
HRESULT CObjectX::Init(void)
{
	int nNumVtx;      //頂点数
	DWORD dwSizeFVF;  //頂点フォーマットのサイズ
	BYTE *pVtxBuff;   //頂点バッファへのポインタ

	//デバイスの取得
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//Xファイル読み込み
	D3DXLoadMeshFromX(m_Info.Fliename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	//頂点数を取得
 	nNumVtx = m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//テクスチャの情報取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//マテリアルへのポインタ
	D3DXMATERIAL *pMat;

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	m_nIdxTexture = new int[(int)m_dwNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャが存在する

			m_nIdxTexture[nCntMat] = pTexture->Regist(pMat[nCntMat].pTextureFilename);
		}
		else
		{
			m_nIdxTexture[nCntMat] = -1;
		}
	}

	//頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;     //頂点座標の代入

		//最小値-------------------------------------------------
		if (m_Info.vtxMini.x > vtx.x)
		{
			m_Info.vtxMini.x = vtx.x;
		}

		if (m_Info.vtxMini.y > vtx.y)
		{
			m_Info.vtxMini.y = vtx.y;
		}

		if (m_Info.vtxMini.z > vtx.z)
		{
			m_Info.vtxMini.z = vtx.z;
		}

		//最大値-------------------------------------------------
		if (m_Info.vtxMax.x < vtx.x)
		{
			m_Info.vtxMax.x = vtx.x;
		}

		if (m_Info.vtxMax.y < vtx.y)
		{
			m_Info.vtxMax.y = vtx.y;
		}

		if (m_Info.vtxMax.z < vtx.z)
		{
			m_Info.vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;    //頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//===========================================================
// 終了処理
//===========================================================
void CObjectX::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		delete m_pTexture;
		m_pTexture = NULL;
	}
	
	//テクスチャの処理
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//頂点バッファの破壊
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//===========================================================
// 更新処理
//===========================================================
void CObjectX::Update(void)
{
	if (m_Info.pos.y >= 0.0f)
	{
		m_Info.pos.y -= 1.0f;
		m_bShut = false;
	}
	    
	if (m_bShut)
	{
		D3DXVECTOR3 rot = CPlayer::GetInstance()->GetRotition();

		m_Info.rot.x = 1.57f;
		m_Info.rot.y = sinf(rot.y);
		m_Info.rot.z = 1.57f;

		m_Info.move.x -= sinf(rot.y) * 5.0f;
		m_Info.move.y += 1.0f;
		m_Info.move.z -= cosf(rot.y) * 5.0f;
	}

	m_Info.pos += m_Info.move;

	// 移動量を更新(減衰させる)
	m_Info.move.x += (0.0f - m_Info.move.x) * 0.1f;
	m_Info.move.y += (0.0f - m_Info.move.y) * 0.1f;
	m_Info.move.z += (0.0f - m_Info.move.z) * 0.1f;
}

//===========================================================
// 描画処理
//===========================================================
void CObjectX::Draw(void)
{
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//計算用マトリックス
	D3DXMATRIX m_mtxRot, m_mtxTrans;

	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	//マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// 描画をする
	if (m_bDraw)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_Info.mtxWorld);

		if (m_pCurrent != nullptr)
		{
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &m_mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&m_mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &m_mtxTrans);

			// マトリックスと親のマトリックスをかけ合わせる
			D3DXMatrixMultiply(&m_Info.mtxWorld,
				&m_Info.mtxWorld, m_pCurrent);
		}
		else
		{
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &m_mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&m_mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);

			D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &m_mtxTrans);
		}

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_Info.mtxWorld);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture[nCntMat]));

			//モデル(パーツ)の描画
			m_pMesh->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}