//=============================================================================
//
// キャラクター [character.cpp]
// Author : 大原　怜将
//
//=============================================================================
#include "main.h"
#include "character.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "object.h"
#include "texture.h"

//================================================================
//コンストラクタ
//================================================================
CCharacter::CCharacter()
{
	//値をクリア
	m_pTexture = NULL;  //テクスチャへのポインタ
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_pTexture = NULL;
	m_pCurrent = nullptr;
	m_dwNumMat = 0;
	m_nNumModel = 0;
	m_vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	m_vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	//m_nIdxTexture = -1;
}

//================================================================
//コンストラクタ(オーバーロード)
//================================================================
CCharacter::CCharacter(const char *aModelFliename)
{
	//値をクリア
	m_pTexture = NULL;  //テクスチャへのポインタ
	m_pParent = NULL;
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_pTexture = NULL;
	m_pCurrent = nullptr;
	m_dwNumMat = 0;
	m_nNumModel = 0;
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	m_vtxMax = D3DXVECTOR3(-900000.0f, -900000.0f, -900000.0f);
	m_vtxMini = D3DXVECTOR3(900000.0f, 900000.0f, 900000.0f);
	m_Fliename = aModelFliename;
	//m_nIdxTexture = -1;
}

//================================================================
//デストラクタ
//================================================================
CCharacter::~CCharacter()
{

}

//================================================================
//生成処理
//================================================================
CCharacter *CCharacter::Create(const char *aModelFliename)
{
	//オブジェクト2Dのポインタ
	CCharacter *pCharacter = nullptr;

	if (pCharacter == nullptr)
	{
		//オブジェクト2Dの生成
		pCharacter = new CCharacter(aModelFliename);

		//初期化処理
		pCharacter->Init();
	}
	
	return pCharacter;
}

//================================================================
//ポリゴンの初期化処理
//================================================================
HRESULT CCharacter::Init(void)
{
	int nNumVtx;      //頂点数
	DWORD dwSizeFVF;  //頂点フォーマットのサイズ
	BYTE *pVtxBuff;   //頂点バッファへのポインタ

	//デバイスの取得
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャの情報取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//Xファイル読み込み
	D3DXLoadMeshFromX(m_Fliename,
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
		if (m_vtxMini.x > vtx.x)
		{
			m_vtxMini.x = vtx.x;
		}

		if (m_vtxMini.y > vtx.y)
		{
			m_vtxMini.y = vtx.y;
		}

		if (m_vtxMini.z > vtx.z)
		{
			m_vtxMini.z = vtx.z;
		}

		//最大値-------------------------------------------------
		if (m_vtxMax.x < vtx.x)
		{
			m_vtxMax.x = vtx.x;
		}

		if (m_vtxMax.y < vtx.y)
		{
			m_vtxMax.y = vtx.y;
		}

		if (m_vtxMax.z < vtx.z)
		{
			m_vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;    //頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//================================================================
//ポリゴンの終了処理
//================================================================
void CCharacter::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		//delete m_pTexture;
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
}

//================================================================
//ポリゴンの更新処理
//================================================================
void CCharacter::Update(void)
{
	
}

//================================================================
//ポリゴンの描画処理
//================================================================
void CCharacter::Draw(void)
{
	//デバイスの取得
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャの情報取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//計算用マトリックス
	D3DXMATRIX m_mtxRot, m_mtxTrans;

	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	//親のマトリックス
	D3DXMATRIX mtxParent;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	if (m_pCurrent != nullptr)
	{
		//位置を反映
		D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);

		// マトリックスと親のマトリックスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld, m_pCurrent);
	}
	else
	{
		//位置を反映
		D3DXMatrixTranslation(&m_mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxTrans);
	}
	

	//パーツの親のマトリックスを設定
	if (m_pParent != NULL)
	{//親モデルがある場合

		mtxParent = *m_pParent->GetMtxWorld();
	}
	else
	{//親モデルがない場合

		//現在のマトリックスを取得する
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//算出したパーツのワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

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

	D3DXMATRIX mtxShadow;
	D3DLIGHT9 light;
	D3DXVECTOR4 posLight;
	D3DXVECTOR3 pos, normal;
	D3DXPLANE plane;

	// ライトの位置を設定
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	// 平面情報を設定
	pos = D3DXVECTOR3(m_mtxWorld._41, 0.5f, m_mtxWorld._43);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// シャドウマトリックスの作成
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		m_ShadowMat.Emissive.r = 0.0f;
		m_ShadowMat.Emissive.g = 0.0f;
		m_ShadowMat.Emissive.b = 0.0f;
		m_ShadowMat.Emissive.a = 1.0f;
		m_ShadowMat.Diffuse.r = 0.0f;
		m_ShadowMat.Diffuse.g = 0.0f;
		m_ShadowMat.Diffuse.b = 0.0f;
		m_ShadowMat.Diffuse.a = 1.0f;

		//マテリアルの設定
		pDevice->SetMaterial(&m_ShadowMat);

		//テクスチャの設定
		pDevice->SetTexture(0, nullptr);

		//モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//================================================================
//テクスチャ取得
//================================================================
void CCharacter::BindTexture(LPDIRECT3DTEXTURE9 *m_Texture)
{
	m_pTexture = m_Texture;
}