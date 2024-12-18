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
#include "utility.h"
#include "object3D.h"
#include "collision.h"

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
	m_ShadowMat = {};

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
	m_ShadowMat = {};

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

void CObjectX::SetVtx(void)
{
	utility::ChangeVtx(&m_Info.vtxMax, &m_Info.vtxMini, m_Info.rot);
}

//===========================================================
// 缶とゴミ箱が蹴られて吹っ飛ぶ処理
//===========================================================
void CObjectX::Shoot(void)
{
	// プレイヤーに蹴られている
	if (m_bShut)
	{
		D3DXVECTOR3 rot = CPlayer::GetInstance()->GetRotition();
		D3DXVECTOR3 move = CPlayer::GetInstance()->GetMove();

		m_Info.rot.x = 1.57f;
		m_Info.rot.y = rot.y;
		m_Info.rot.z = 1.57f;

		m_Info.move.x += (move.x * 1.2f);
		m_Info.move.y += 1.0f;
		m_Info.move.z += (move.z * 1.2f);
	}
}

//===========================================================
// 柵が倒れる処理
//===========================================================
void CObjectX::FallDown(void)
{
	if (m_Info.rot.x >= 1.3f || m_Info.rot.x <= -1.3f)
	{
		m_bDown = false;

		m_fFallDownSpeed = 0.0f;
	}

	D3DXVECTOR3 move = {};

	// 柵に振れた瞬間だけ通る
	if (m_bDown)
	{
		move = CPlayer::GetInstance()->GetMove();

		m_fFallDownSpeed = (move.x + move.x) - (move.z + move.z);
	}

	if (m_fFallDownSpeed >= 8.0f || m_fFallDownSpeed <= -8.0f)
	{// プレイヤーの速度が一定より速かったら

		move = CPlayer::GetInstance()->GetMove();
		m_Info.move.x += move.x;
		m_Info.move.z += move.z;
	}

	m_Info.rot.x -= (m_fFallDownSpeed * 0.01f);
}

//===========================================================
// プレイヤーの攻撃がヒットしたとき揺らす処理
//===========================================================
void CObjectX::HitAttack(void)
{
	// カメラを揺らす処理を行うかどうかのフラグが立っていたらカメラを揺らす処理を行う
	if (m_bHitAttack)
	{
		// sin を使用した揺らし座標の算出
		m_Info.move.y = 0.0f;
			
		m_Info.move.x = sinf(ShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter / 60)) * 2.0f;

		m_Info.move.z = sinf(ShakeAngle) * (1.0f - ((float)m_nShakeTimeCounter / 60)) * 2.0f;

		// 揺らし処理に使用する sin に渡す角度の変更処理
		ShakeAngle += 2.0f * 1.0f;

		// 揺らす時間が経過したら揺らし処理を終了する
		m_nShakeTimeCounter += 1;
		if (m_nShakeTimeCounter >= 60)
		{
			m_bHitAttack = false;
		}
	}
	else
	{
		// 揺らされていない場合は揺らし処理による加算座標を０にする
		m_nShakeTimeCounter = 0;
	}
}

//===========================================================
// プレイヤーの攻撃がヒットしたとき揺らす処理
//===========================================================
void CObjectX::GraduallyFallDown(void)
{
	// カメラを揺らす処理を行うかどうかのフラグが立っていたらカメラを揺らす処理を行う
	if (m_bFall)
	{
		// sin を使用した揺らし座標の算出
		m_Info.move.y = 0.0f;

		m_Info.move.x = sinf(ShakeAngle) * (1.0f - ((float)m_nShake / 60)) * 2.0f;

		m_Info.move.z = sinf(ShakeAngle) * (1.0f - ((float)m_nShake / 60)) * 2.0f;

		// 揺らし処理に使用する sin に渡す角度の変更処理
		ShakeAngle += 2.0f * m_fAngle;

		// 揺らす時間が経過したら揺らし処理を終了する
		m_nShake += 1;
		if (m_nShake >= 60)
		{
			m_bFall = false;
			m_fAngle += 2.0f;
			//m_nFallDownCounter++;
		}
	}
	else
	{
		// 揺らされていない場合は揺らし処理による加算座標を０にする
		//m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_nShake = 0;
	}

	if ((m_Info.rot.x >= 1.57f || m_Info.rot.x <= -1.57f))
	{
		//m_bFall = false;
		m_bFallDown = true;
		return;
	}
		
	D3DXVECTOR3 pos = {};
	D3DXVECTOR3 rot = {};

	pos = CPlayer::GetInstance()->GetPosition();
	rot = CPlayer::GetInstance()->GetRotition();

	
	if (m_nFallDownCounter >= 15)
	{
		if (pos.x >= m_Info.pos.x && pos.z >= m_Info.pos.z)
		{
			m_Info.rot.x -= 0.1f;
		}

		if (pos.x >= m_Info.pos.x && pos.z <= m_Info.pos.z)
		{
			m_Info.rot.x -= 0.1f;
		}

		if (pos.x <= m_Info.pos.x && pos.z >= m_Info.pos.z)
		{
			m_Info.rot.x += 0.1f;
		}

		if (pos.x <= m_Info.pos.x && pos.z <= m_Info.pos.z)
		{
			m_Info.rot.x += 0.1f;
		}

		utility::ChangeVtx(&m_Info.vtxMax, &m_Info.vtxMini, m_Info.rot);
	}
}

//===========================================================
// 水が漏れる
//===========================================================
void CObjectX::WaterLeak(void)
{
	if (!m_bWaterLeak)
		return;

	if (!m_bShut)
	{
		if (m_pWater == nullptr)
		{
			m_pWater = CObject3D::Create();
			m_pWater->SetIdxTex(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\water.png"));
			m_pWater->SetSize(100.0f, 100.0f);
			m_pWater->SetDraw(true);
			m_pWater->SetPosition(D3DXVECTOR3(m_Info.pos.x, 1.0f, m_Info.pos.z));
		}

		if (m_pWater != nullptr)
		{
			CPlayer *pPlayer = CPlayer::GetInstance();
			if (CCollision::GetInstance()->Circle(m_pWater->GetPosition(), pPlayer->GetPosition(), 75.0f, 50.0f))
			{
				int i = 0;
			}
			else
			{

			}
		}
	}
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
	//// プレイヤーに蹴られて吹き飛ぶ処理
	//Shoot();

	//// プレイヤーが柵に触れて倒れる
	//FallDown();

	//// 
	//HitAttack();

	//// 
	//GraduallyFallDown();

	//WaterLeak();

	//if (m_Info.pos.y - m_Info.vtxMax.x >= 0.0f)
	//{
	//	m_Info.pos.y -= m_fGravity;
	//	m_bShut = false;                                      
	//}

	//if (m_Info.pos.y <= 0.0f)
	//{
	//	m_Info.pos.y = 0.0f;
	//	m_bShut = false;
	//}

	//if (m_nHitCounter >= 3 && m_bDamage)
	//{
	//	m_nHitCounter = 0;
	//}

	//m_Info.pos += m_Info.move;

	//// 移動量を更新(減衰させる)
	//m_Info.move.x += (0.0f - m_Info.move.x) * 0.1f;
	//m_Info.move.y += (0.0f - m_Info.move.y) * 0.1f;
	//m_Info.move.z += (0.0f - m_Info.move.z) * 0.1f;
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

		//D3DXMATRIX mtxShadow;
		//D3DLIGHT9 light;
		//D3DXVECTOR4 posLight;
		//D3DXVECTOR3 pos, normal;
		//D3DXPLANE plane;

		//// ライトの位置を設定
		//pDevice->GetLight(0, &light);
		//posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

		//// 平面情報を設定
		//pos = D3DXVECTOR3(m_Info.mtxWorld._41, 0.5f, m_Info.mtxWorld._43);
		//normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//D3DXPlaneFromPointNormal(&plane, &pos, &normal);

		//// シャドウマトリックスの初期化
		//D3DXMatrixIdentity(&mtxShadow);

		//// シャドウマトリックスの作成
		//D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
		//D3DXMatrixMultiply(&mtxShadow, &m_Info.mtxWorld, &mtxShadow);

		////ワールドマトリックスの設定
		//pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		////現在のマテリアルを取得
		//pDevice->GetMaterial(&matDef);

		////マテリアルデータへのポインタを取得
		//pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		//for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
		//{
		//	m_ShadowMat.Emissive.r = 0.0f;
		//	m_ShadowMat.Emissive.g = 0.0f;
		//	m_ShadowMat.Emissive.b = 0.0f;
		//	m_ShadowMat.Emissive.a = 1.0f;
		//	m_ShadowMat.Diffuse.r = 0.0f;
		//	m_ShadowMat.Diffuse.g = 0.0f;
		//	m_ShadowMat.Diffuse.b = 0.0f;
		//	m_ShadowMat.Diffuse.a = 1.0f;

		//	//マテリアルの設定
		//	pDevice->SetMaterial(&m_ShadowMat);

		//	//テクスチャの設定
		//	pDevice->SetTexture(0, nullptr);

		//	//モデル(パーツ)の描画
		//	m_pMesh->DrawSubset(nCntMat);
		//}

		////保存していたマテリアルを戻す
		//pDevice->SetMaterial(&matDef);
	}
}