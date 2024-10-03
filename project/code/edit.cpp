//===========================================================
//
//エディット[edit.cpp]
//Author 大原怜将
//
//===========================================================
#include "edit.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "player.h"
#include "InputKeyBoard.h"

#include <stdio.h>
#include<string.h>

//マクロ定義
#define MODEL_SET_TEXT  ("data\\TEXT\\model_set.txt")
#define MODEL  ("data\\TEXT\\model.txt")
#define MOVE   (4.0f)  //移動量

//===========================================================
//コンストラクタ
//===========================================================
CEdit::CEdit()
{
	//m_pos = { 0.0f, 0.0f, 0.0f };
	m_Model[0] = {};
	m_nIdx = 0;
	m_nNumAll = 0;
	m_SetModel = 0;
	m_nSelectModel = 0;
	m_mode = MODE_CREATE;
}

//===========================================================
//デストラクタ
//===========================================================
CEdit::~CEdit()
{

}

//===========================================================
//初期化処理
//===========================================================
HRESULT CEdit::Init(void)
{
	for (int nCount = 0; nCount < MAX_MODELSET; nCount++)
	{
		m_apModel[nCount] = nullptr;
	}

	//テキストファイル読み込み
	ReadText();

	//Xファイル読み込み
	XLoad();

	return S_OK;
}

//===========================================================
//終了処理
//===========================================================
void CEdit::Uninit(void)
{
	for (int nCount = 0; nCount < m_nNumAll; nCount++)
	{
		if (m_Model[nCount].m_pTexture != NULL)
		{
			m_Model[nCount].m_pTexture = NULL;
		}
	}

	for (int nCount = 0; nCount < m_nNumAll; nCount++)
	{
		//テクスチャの処理
		if (m_Model[nCount].m_pMesh != NULL)
		{
			m_Model[nCount].m_pMesh->Release();
			m_Model[nCount].m_pMesh = NULL;
		}
	}

	for (int nCount = 0; nCount < m_nNumAll; nCount++)
	{
		//頂点バッファの破壊
		if (m_Model[nCount].m_pBuffMat != NULL)
		{
			m_Model[nCount].m_pBuffMat->Release();
			m_Model[nCount].m_pBuffMat = NULL;
		}
	}
}

//===========================================================
//更新処理
//===========================================================
void CEdit::Update(void)
{
	//キーボードの情報を取得
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	for (int nCount = 0; nCount < MAX_MODELSET; nCount++)
	{
		// 更新処理
		if (m_apModel[nCount] != NULL)
			m_apModel[nCount]->Update();
	}

	if (InputKeyboard == nullptr)
		return;

	if (InputKeyboard->GetTrigger(DIK_F2) == true)
	{//F2キーが押された

		if (m_mode == MODE_CREATE)
		{//現在のモードがクリエイトモードだったら

			m_mode = MODE_DELETE;  //モードをデリートにする
		}
		else if (m_mode == MODE_DELETE)
		{//現在のモードがデリートモードだったら

			m_mode = MODE_CREATE;  //モードをクリエイトにする
		}
	}

	switch (m_mode)
	{
	case MODE_CREATE:

		//制御処理
		Control();

		break;

	case MODE_DELETE:

		//セットしたモデルを消す処理
		SetModelDelete();

		break;
	}

	// デバック情報
	Debug();
}

//===========================================================
//描画処理
//===========================================================
void CEdit::Draw(void)
{
	m_aModel[m_nIdx]->Draw();


	//デバイスの取得
	CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX m_mtxRot, m_mtxTrans;

	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;

	//マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	if (m_mode == MODE_CREATE)
	{//現在のモードがクリエイトモードだったら

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_Model[m_nIdx].m_mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_Model[m_nIdx].m_rot.y, m_Model[m_nIdx].m_rot.x, m_Model[m_nIdx].m_rot.z);

		D3DXMatrixMultiply(&m_Model[m_nIdx].m_mtxWorld, &m_Model[m_nIdx].m_mtxWorld, &m_mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&m_mtxTrans, m_Model[m_nIdx].m_pos.x, m_Model[m_nIdx].m_pos.y, m_Model[m_nIdx].m_pos.z);

		D3DXMatrixMultiply(&m_Model[m_nIdx].m_mtxWorld, &m_Model[m_nIdx].m_mtxWorld, &m_mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_Model[m_nIdx].m_mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_Model[m_nIdx].m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIdx].m_dwNumMat; nCntMat++)
		{
			pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, nullptr);

			//モデル(パーツ)の描画
			m_Model[m_nIdx].m_pMesh->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//===========================================================
//セーブ処理
//===========================================================
void CEdit::Save(void)
{
	//デバッグプロックの情報を取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	FILE* pFile;

	pFile = fopen(MODEL, "w");

	if (pFile != NULL)
	{//ファイルを開けた場合

		fprintf(pFile, "SCRIPT\n");

		fprintf(pFile, "\n");

		for (int nCount = 0; nCount < m_SetModel; nCount++)
		{
			D3DXVECTOR3 pos = m_apModel[nCount]->GetPosition();  //位置取得
			D3DXVECTOR3 rot = m_apModel[nCount]->GetRotition();       //向き取得
			int n = m_apModel[nCount]->GetIdxModel();

			fprintf(pFile, "MODELSET\n");
			fprintf(pFile, "    TYPE = %d\n", n);  //種類
			fprintf(pFile, "    POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);      //位置
			fprintf(pFile, "    ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);      //向き
			fprintf(pFile, "END_MODELSET\n");
			fprintf(pFile, "\n");
		}

		fprintf(pFile, "END_SCRIPT\n");

		//ファイルを閉じる
		fclose(pFile);
	}

	pDebugProc->Print("\n保存したよ～");
}

//===========================================================
//Xファイルロード処理
//===========================================================
void CEdit::XLoad(void)
{
	for (int nCount = 0; nCount < m_nNumAll; nCount++)
	{
		int nNumVtx;      //頂点数
		DWORD dwSizeFVF;  //頂点フォーマットのサイズ
		BYTE* pVtxBuff;   //頂点バッファへのポインタ

		//デバイスの取得
		CRenderer* pRenderer = CManager::GetInstance()->GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		//Xファイル読み込み
		D3DXLoadMeshFromX(m_Model[nCount].m_filename,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_Model[nCount].m_pBuffMat,
			NULL,
			&m_Model[nCount].m_dwNumMat,
			&m_Model[nCount].m_pMesh);

		//頂点数を取得
		nNumVtx = m_Model[nCount].m_pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(m_Model[nCount].m_pMesh->GetFVF());

		if (m_Model[nCount].m_pTexture == NULL)
		{
			m_Model[nCount].m_pTexture = new LPDIRECT3DTEXTURE9[(int)m_Model[nCount].m_dwNumMat];
		}

		//マテリアルへのポインタ
		D3DXMATERIAL* pMat;

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)m_Model[nCount].m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_Model[nCount].m_dwNumMat; nCntMat++)
		{
			//if (pMat[nCntMat].pTextureFilename != NULL)
			//{//テクスチャが存在する

			//	D3DXCreateTextureFromFile(pDevice,
			//		pMat[nCntMat].pTextureFilename,
			//		&m_Model[nCount].m_pTexture[nCntMat]);
			//}
			//else
			//{
			//	m_Model[nCount].m_pTexture[nCntMat] = NULL;
			//}

			//if (pMat[nCntMat].pTextureFilename != NULL)
			//{//テクスチャが存在する

			//	m_nIdxTexture[nCntMat] = pTexture->Regist(pMat[nCntMat].pTextureFilename);
			//}
			//else
			//{
			//	m_nIdxTexture[nCntMat] = -1;
			//}
		}

		//頂点バッファをロック
		m_Model[nCount].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;     //頂点座標の代入

			//最小値-------------------------------------------------
			if (m_Model[nCount].m_vtxMini.x > vtx.x)
			{
				m_Model[nCount].m_vtxMini.x = vtx.x;
			}

			if (m_Model[nCount].m_vtxMini.y > vtx.y)
			{
				m_Model[nCount].m_vtxMini.y = vtx.y;
			}

			if (m_Model[nCount].m_vtxMini.z > vtx.z)
			{
				m_Model[nCount].m_vtxMini.z = vtx.z;
			}

			//最大値-------------------------------------------------
			if (m_Model[nCount].m_vtxMax.x < vtx.x)
			{
				m_Model[nCount].m_vtxMax.x = vtx.x;
			}

			if (m_Model[nCount].m_vtxMax.y < vtx.y)
			{
				m_Model[nCount].m_vtxMax.y = vtx.y;
			}

			if (m_Model[nCount].m_vtxMax.z < vtx.z)
			{
				m_Model[nCount].m_vtxMax.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;    //頂点フォーマットのサイズ分ポインタを進める
		}

		//頂点バッファをアンロック
		m_Model[nCount].m_pMesh->UnlockVertexBuffer();
	}
}

//===========================================================
//マップファイルロード処理
//===========================================================
void CEdit::TEXTLoad(void)
{
	char aString[128] = {};
	char aComment[128] = {};
	int nCntModel = 0;
	int ntype = 0;

	FILE* pFile;   //ファイルポインタを宣言

	pFile = fopen(MODEL, "r");

	if (pFile != NULL)
	{//ファイルを開けた場合

		fscanf(pFile, "%s", &aString[0]);

		if (strcmp("SCRIPT", aString) == 0)
		{
			while (strcmp("END_SCRIPT", aString) != 0)
			{
				fscanf(pFile, "%s", &aString[0]);

				if (strcmp("MODELSET", aString) == 0)
				{
					while (strcmp("END_MODELSET", aString) != 0)
					{
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp("TYPE", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);   //=
							fscanf(pFile, "%d", &ntype);     //種類
						}

						if (strcmp("POS", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);          //=
							fscanf(pFile, "%f", &m_Model[ntype].m_pos.x);  //位置
							fscanf(pFile, "%f", &m_Model[ntype].m_pos.y);  //位置
							fscanf(pFile, "%f", &m_Model[ntype].m_pos.z);  //位置
						}

						if (strcmp("ROT", aString) == 0)
						{
							fscanf(pFile, "%s", &aString);          //=
							fscanf(pFile, "%f", &m_Model[ntype].m_rot.x);  //位置
							fscanf(pFile, "%f", &m_Model[ntype].m_rot.y);  //位置
							fscanf(pFile, "%f", &m_Model[ntype].m_rot.z);  //位置

							//オブジェクトXを生成
							m_apModel[m_SetModel] = CObjectX::Create(m_Model[ntype].m_filename);
							m_apModel[m_SetModel]->SetPosition(m_Model[ntype].m_pos);
							m_apModel[m_SetModel]->SetRotition(m_Model[ntype].m_rot);
							m_apModel[m_SetModel]->SetIdxModel(m_nIdx);

							//種類設定
							//m_apModel[m_SetModel]->SetType(ntype);

							m_SetModel++;  //カウントアップ
						}
					}
				}  //NUM_MODELのかっこ
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//===========================================================
//テキストファイル読み込み
//===========================================================
void CEdit::ReadText(void)
{
	char aString[128] = {};
	char aComment[128] = {};
	int nCntModel = 0;

	FILE* pFile;   //ファイルポインタを宣言

	pFile = fopen(MODEL_SET_TEXT, "r");

	if (pFile != NULL)
	{//ファイルを開けた場合

		fscanf(pFile, "%s", &aString[0]);

		if (strcmp("SCRIPT", aString) == 0)
		{
			while (strcmp("END_SCRIPT", aString) != 0)
			{
				fscanf(pFile, "%s", &aString[0]);

				if (strcmp("NUM_MODEL", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);          //=
					fscanf(pFile, "%d", &m_nNumAll);  //モデルの総数

					m_aModel = new CObjectX*[m_nNumAll];

				}  //NUM_MODELのかっこ

				if (strcmp("MODEL_FILENAME", aString) == 0)
				{
					fscanf(pFile, "%s", &aString);          //=
					fscanf(pFile, "%s", &m_Model[nCntModel].m_filename[0]);  //モデルの名前

					m_aModel[nCntModel] = CObjectX::Create(m_Model[nCntModel].m_filename);
					m_aModel[nCntModel]->SetIdxModel(nCntModel);
					m_Model[nCntModel].m_nType = nCntModel;
					m_Model[nCntModel].m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					m_Model[nCntModel].m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					m_Model[nCntModel].m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					nCntModel++;

				}  //MODEL_LILENAMEのかっこ
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//===========================================================
//制御処理
//===========================================================
void CEdit::Control(void)
{
	//キーボードの情報を取得
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXVECTOR3 CameraRot = {};

	//カメラの向き取得
	if (pCamera != nullptr)
		CameraRot = pCamera->GetRotation();

	if (InputKeyboard == nullptr)
		return;

	if (InputKeyboard->GetTrigger(DIK_1) == true)
	{
		//インデックス番号が-1より大きいとき、インデックス番号を一つ戻す
		if (m_nIdx > -1)
			m_nIdx -= 1;

		//インデックス番号が0より小さいとき、インデックス番号を総数-1にする
		if (m_nIdx < 0)
			m_nIdx = m_nNumAll - 1;
	}

	if (InputKeyboard->GetTrigger(DIK_2) == true)
	{
		//インデックス番号が総数より小さいとき、インデックス番号を一つ進める
		if (m_nIdx < m_nNumAll)
			m_nIdx += 1;

		//インデックス番号が総数以上になったとき、インデックス番号を0にする
		if (m_nIdx >= m_nNumAll)
			m_nIdx = 0;
	}

	// 配置するオブジェクト生成
	if (InputKeyboard->GetTrigger(DIK_RETURN) == true)
		CreateMapModel();

	// 配置したオブジェクト全消去
	if (InputKeyboard->GetTrigger(DIK_F7) == true)
		AllDelete();

	// 外部ファイル書き出し
	if (InputKeyboard->GetTrigger(DIK_F8) == true)
		Save();

	// マップファイル読み込み
	if (InputKeyboard->GetTrigger(DIK_F9) == true)
		TEXTLoad();

	// 移動処理
	Move();

	// 回転処理
	Rotation();
}

//===========================================================
//セットしたモデルを選択して消去する処理
//===========================================================
void CEdit::SetModelDelete(void)
{
	//キーボードの情報を取得
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//デバッグプロックの情報を取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	if (InputKeyboard->GetTrigger(DIK_1) == true)
	{
		//インデックス番号が-1より大きい
		if (m_nSelectModel > -1)
			m_nSelectModel -= 1;
	}

	//インデックス番号が0より小さいとき、インデックス番号を総数-1にする
	if (m_nSelectModel < 0)
		m_nSelectModel = m_SetModel - 1;

	if (InputKeyboard->GetTrigger(DIK_2) == true)
	{
		//インデックス番号が総数より小さいとき
		if (m_nSelectModel < m_SetModel)
			m_nSelectModel += 1;
	}

	//インデックス番号が総数以上になったとき、インデックス番号を0にする
	if (m_nSelectModel >= m_SetModel)
		m_nSelectModel = 0;

	if (InputKeyboard->GetTrigger(DIK_DELETE) == true)
	{
		if (m_apModel[m_nSelectModel] != nullptr)
		{//使用されていたら

			//初期化処理
			m_apModel[m_nSelectModel]->Uninit();

			//使用していない状態にする
			m_apModel[m_nSelectModel] = nullptr;

			//カウントダウン
			m_SetModel--;
		}

		for (int nCount = 0; nCount < m_SetModel; nCount++)
		{
			if (m_apModel[nCount] == nullptr && m_apModel[nCount + 1] != nullptr)
			{//現在のカウントが使用されていないかつ、次のカウントが使用されているとき

				//代入
				m_apModel[nCount] = m_apModel[nCount + 1];

				//次のカウントを使用していない状態にする
				m_apModel[nCount + 1] = nullptr;
			}
		}
	}

	if (m_SetModel > 0)
	{
		pDebugProc->Print("\n<< 削除するオブジェクトの番号：%d/%d >>", m_nSelectModel, m_SetModel);
	}

	if (m_SetModel <= 0)
	{
		pDebugProc->Print("\n<< 削除できるオブジェクトはありません >>");
	}
}

//===========================================================
// 配置するオブジェクト生成処理
//===========================================================
void CEdit::CreateMapModel(void)
{
	if (m_apModel[m_SetModel] == nullptr)
	{//使用されていないとき

		//オブジェクトXを生成
		m_apModel[m_SetModel] = CObjectX::Create(m_aModel[m_nIdx]->GetFileName());

		m_apModel[m_SetModel]->SetPosition(m_aModel[m_nIdx]->GetPosition());

		m_apModel[m_SetModel]->SetRotition(m_aModel[m_nIdx]->GetRotition());
		m_apModel[m_SetModel]->SetIdxModel(m_nIdx);

		//種類設定
		//m_apModel[m_SetModel]->SetType(m_nIdx);

		//総数をカウントアップ
		m_SetModel++;
	}
}

//===========================================================
//移動処理
//===========================================================
void CEdit::Move(void)
{
	//キーボードの情報を取得
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	//カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXVECTOR3 CameraRot = {};

	//カメラの向き取得
	if (pCamera != nullptr)
		CameraRot = pCamera->GetRotation();

	if (InputKeyboard == nullptr)
		return;

	D3DXVECTOR3 pos = m_aModel[m_nIdx]->GetPosition();

	//上に移動----------------------------------------------
	if (InputKeyboard->GetPress(DIK_W) == true)
	{//Wキーが押された

		if (InputKeyboard->GetPress(DIK_D) == true)
		{//Dキーも押した 右上に移動

		 //移動量
			pos.x += sinf(CameraRot.y + (D3DX_PI * 0.75f)) * MOVE;
			pos.z += cosf(CameraRot.y + (D3DX_PI * 0.75f)) * MOVE;
		}
		else if (InputKeyboard->GetPress(DIK_A) == true)
		{//Aキーも押した 左上に移動

		 //移動量
			pos.x += sinf(CameraRot.y + (D3DX_PI * 0.25f)) * MOVE;
			pos.z += cosf(CameraRot.y + (D3DX_PI * 0.25f)) * MOVE;
		}
		else
		{//Wキーだけ押した	真上に移動

		 //移動量
			pos.x -= sinf(CameraRot.y) * MOVE;
			pos.z -= cosf(CameraRot.y) * MOVE;
		}
	}
	//下に移動----------------------------------------------
	else if (InputKeyboard->GetPress(DIK_S) == true)
	{//Sキーが押された

		if (InputKeyboard->GetPress(DIK_D) == true)
		{//Dキーも押した 右下に移動

		 //移動量
			pos.x += sinf(CameraRot.y + (-D3DX_PI * 0.25f)) * MOVE;
			pos.z += cosf(CameraRot.y + (-D3DX_PI * 0.25f)) * MOVE;
		}
		else if (InputKeyboard->GetPress(DIK_A) == true)
		{//Aキーも押した 左下に移動

		 //移動量
			pos.x += sinf(CameraRot.y + (D3DX_PI * 0.25f)) * MOVE;
			pos.z += cosf(CameraRot.y + (D3DX_PI * 0.25f)) * MOVE;
		}
		else
		{//Sキーだけ押した 真下に移動

		 //移動量
			pos.x += sinf(CameraRot.y) * MOVE;
			pos.z += cosf(CameraRot.y) * MOVE;
		}
	}
	//右に移動----------------------------------------------
	else if (InputKeyboard->GetPress(DIK_D) == true)
	{//Dキーだけ押した

	 //移動量
		pos.x += sinf(CameraRot.y + (-D3DX_PI * 0.5f)) * MOVE;
		pos.z += cosf(CameraRot.y + (-D3DX_PI * 0.5f)) * MOVE;
	}
	//左に移動----------------------------------------------
	else if (InputKeyboard->GetPress(DIK_A) == true)
	{//Aキーだけ押した

		//移動量
		pos.x += sinf(CameraRot.y + (D3DX_PI * 0.5f)) * MOVE;
		pos.z += cosf(CameraRot.y + (D3DX_PI * 0.5f)) * MOVE;
	}

	//移動量を更新(減衰させる)--------------------------------------------
	pos.x += (0.0f - m_Model[m_nIdx].m_move.x) * 0.1f;
	pos.z += (0.0f - m_Model[m_nIdx].m_move.z) * 0.1f;

	//移動量加算
	pos.x += m_Model[m_nIdx].m_move.x;
	pos.z += m_Model[m_nIdx].m_move.z;

	m_aModel[m_nIdx]->SetPosition(pos);

	//位置を代入
	//m_Model[m_nIdx].m_pos = m_pos;
}

//===========================================================
//回転処理
//===========================================================
void CEdit::Rotation(void)
{
	//キーボードの情報を取得
	CInputKeyboard* InputKeyboard = CManager::GetInstance()->GetKeyBoard();

	if (InputKeyboard == nullptr)
		return;

	D3DXVECTOR3 rot = m_aModel[m_nIdx]->GetRotition();

	if (InputKeyboard->GetPress(DIK_U) == true)
	{//Uキーを押した

		rot.y += 0.01f;
	}

	if (InputKeyboard->GetPress(DIK_I) == true)
	{//Iキーを押した

		rot.y -= 0.01f;
	}

	if (InputKeyboard->GetTrigger(DIK_O) == true)
	{//Oキーを押した

		rot.y = 0.0f;
	}

	if (rot.y > D3DX_PI)
	{
		rot.y = -D3DX_PI;
	}

	else if (rot.y < -D3DX_PI)
	{
		rot.y = D3DX_PI;
	}

	m_aModel[m_nIdx]->SetRotition(rot);
}

//===========================================================
//配置したオブジェクトをすべて消去する処理
//===========================================================
void CEdit::AllDelete(void)
{
	for (int nCount = 0; nCount < m_SetModel; nCount++)
	{
		if (m_apModel[nCount] != nullptr)
		{//使用されているとき

			//終了処理
			m_apModel[nCount]->Uninit();

			//使用していない状態にする
			m_apModel[nCount] = nullptr;
		}
	}

	m_SetModel = 0;
}

//===========================================================
//デバッグ情報
//===========================================================
void CEdit::Debug(void)
{
	//デバッグプロックの情報を取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	pDebugProc->Print("\n<< -----------------エディットの情報----------------- >>");
	pDebugProc->Print("\n<< モードの切り替え : F2 >>");

	//現在のモードがクリエイトモードだったら
	if (m_mode == MODE_CREATE)
		pDebugProc->Print("\n<< 現在のモード : クリエイト >>");

	//現在のモードがデリートモードだったら
	if (m_mode == MODE_DELETE)
		pDebugProc->Print("\n<< 現在のモード : デリート >>");

	pDebugProc->Print("\n<< オブジェクトの位置：X軸<%f>,Y軸<%f>,Z軸<%f> >>", m_Model[m_nIdx].m_pos.x, m_Model[m_nIdx].m_pos.y, m_Model[m_nIdx].m_pos.z);
	pDebugProc->Print("\n<< オブジェクトの向き：X軸<%f>,Y軸<%f>,Z軸<%f> >>", m_Model[m_nIdx].m_rot.x, m_Model[m_nIdx].m_rot.y, m_Model[m_nIdx].m_rot.z);
	pDebugProc->Print("\n<< 現在のオブジェクトの番号：%d/%d >>", m_nIdx, m_nNumAll);
	pDebugProc->Print("\n<< 設置したオブジェクトの数：%d >>", m_SetModel);
	pDebugProc->Print("\n<< 配置リセット：F7 >>");
	pDebugProc->Print("\n<< 外部ファイル書き出し：F8 >>");
	pDebugProc->Print("\n<< 外部ファイル読み込み：F9 >>");
	pDebugProc->Print("\n<< モデルの旋回 : U/I >>");
	pDebugProc->Print("\n<< モデルの向きリセット : O >>");
	pDebugProc->Print("\n<< --------------------------------------------------- >>");
}