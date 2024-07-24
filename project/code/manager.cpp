//===========================================================
//
// マネージャ処理[manager.cpp]
// Author 大原怜将
//
//===========================================================
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "input.h"
#include "time.h"
#include "debugproc.h"
#include "camera.h"
#include "texture.h"
#include "game.h"
#include "fade.h"
#include "pause.h"
#include "title.h"
#include "light.h"
#include "result.h"
#include "tutorial.h"
#include "utility.h"
#include "MyEffekseer.h"
#include "InputMouse.h"
#include "InputJoyPad.h"

//===========================================================
//静的メンバ変数
//===========================================================
CManager *CManager::m_pManager = nullptr;
CGame  *CScene::m_Game = nullptr;
CTutorial *CScene::m_Tutorial = nullptr;
CTitle *CScene::m_Title = nullptr;
CResult *CScene::m_Result = nullptr;
CScene::MODE CScene::m_mode = CScene::MODE_TITLE;

//===========================================================
// 生成
//===========================================================
CManager * CManager::GetInstance()
{
	if (m_pManager == NULL)
	{
		return m_pManager = new CManager;
	}
	else
	{
		return m_pManager;
	}
}

//================================================================
// 破棄
//================================================================
HRESULT CManager::Release(void)
{
	if (m_pManager != NULL)
	{
		m_pManager->Uninit();

		delete m_pManager;

		m_pManager = NULL;
	}

	return S_OK;
}

//================================================================
// コンストラクタ
//================================================================
CManager::CManager()
{
	m_ResetCounter = 0;
	m_nDefeat = 0;
	m_bPause = false;
	m_pUtility = nullptr;
}

//================================================================
// デストラクタ
//================================================================
CManager::~CManager()
{

}

//================================================================
// 初期化処理
//================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	if (m_Renderer == NULL)
	{//使用されていなかったら

		//レンダラーの生成
		m_Renderer = new CRenderer;

		//初期化処理
		m_Renderer->Init(hWnd, TRUE);
	}

	if (m_InputKeyboard == NULL)
	{//使用されていなかったら

		//キーボードの生成
		m_InputKeyboard = new CInputKeyboard;

		//初期化処理
		m_InputKeyboard->Init(hInstance, hWnd);
	}

	if (m_InputMouse == NULL)
	{//使用されていなかったら

		//マウス生成
		m_InputMouse = new CInputMouse;

		//初期化処理
		m_InputMouse->Init(hInstance, hWnd);
	}

	if (m_InputJoyPad == NULL)
	{//使用されていなかったら

		//ゲームパッドの生成
		m_InputJoyPad = new CInputJoyPad;

		//初期化処理
		m_InputJoyPad->Init();
	}

	if (m_Sound == NULL)
	{//使用されていなかったら

		//サウンドの生成
		m_Sound = new CSound;

		//初期化処理
		m_Sound->Init(hWnd);
	}

	if (m_DebugProc == NULL)
	{//使用されていなかったら

		//デバッグプロックの生成
		m_DebugProc = new CDebugProc;

		//初期化処理
		m_DebugProc->Init();
	}

	if (m_Camera == NULL)
	{//使用されていなかったら

		//カメラを生成
		m_Camera = new CCamera;

		//初期化処理
		m_Camera->Init();
	}

	if (m_Light == NULL)
	{//使用されていなかったら

	 //ライトを生成
		m_Light = new CLight;

		//初期化処理
		m_Light->Init();
	}

	if (m_Texture == NULL)
	{//使用されていなかったら

		//テクスチャを生成
		m_Texture = new CTexture;

		//初期化処理
		m_Texture->Load();
	}

	if (m_pScene == NULL)
	{//使用されていなかったら

		//シーンの生成
		m_pScene = CScene::Create(CScene::MODE_GAME);

		//初期化処理
		m_pScene->Init();
	}

	if (m_Fade == NULL)
	{//使用されていなかったら

		//フェードを生成
		m_Fade = new CFade;

		//初期化処理
		m_Fade->Init();
	}

	if (m_pMyEffekseer == nullptr)
	{// 使用されていなかったら

		// エフェクシアの生成
		m_pMyEffekseer = new CMyEffekseer;

		// 初期化処理
		m_pMyEffekseer->Init();
	}

	return S_OK;
}

//================================================================
// 終了処理
//================================================================
void CManager::Uninit(void)
{
	//サウンドを止める
	m_Sound->Stop();

	//シーンの破棄
	if (m_pScene != NULL)
	{
		//シーンの終了処理
		m_pScene->Uninit();

		//メモリを開放
		delete m_pScene;

		//使用していない状態にする
		m_pScene = NULL;
	}

	//レンダラーの破棄
	if (m_Renderer != NULL)
	{
		//レンダラーの終了処理
		m_Renderer->Uninit();

		//メモリを開放
		delete m_Renderer;

		//使用していない状態にする
		m_Renderer = NULL;
	}

	//キーボードの破棄
	if (m_InputKeyboard != NULL)
	{
		//キーボードの終了処理
		m_InputKeyboard->Uninit();

		//メモリを開放
		delete m_InputKeyboard;

		//使用していない状態にする
		m_InputKeyboard = NULL;
	}

	//マウスの破棄
	if (m_InputMouse != NULL)
	{
		//マウスの終了処理
		m_InputMouse->Uninit();

		//メモリを開放
		delete m_InputMouse;

		//使用していない状態にする
		m_InputMouse = NULL;
	}

	//ゲームパッドの破棄
	if (m_InputJoyPad != NULL)
	{
		//ゲームパッドの終了処理
		m_InputJoyPad->Uninit();

		//メモリを開放
		delete m_InputJoyPad;

		//使用していない状態にする
		m_InputJoyPad = NULL;
	}

	//サウンドの破棄
	if (m_Sound != NULL)
	{
		//サウンドの終了処理
		m_Sound->Uninit();
	
		//メモリを開放
		delete m_Sound;

		//使用していない状態にする
		m_Sound = NULL;
	}

	//デバッグプロックの破棄
	if (m_DebugProc != NULL)
	{
		//デバッグプロックの終了処理
		m_DebugProc->Uninit();

		//メモリを開放
		delete m_DebugProc;

		//使用していない状態にする
		m_DebugProc = NULL;
	}

	//カメラの破棄
	if (m_Camera != NULL)
	{
		//デバッグプロックの終了処理
		m_Camera->Uninit();

		//メモリを開放
		delete m_Camera;

		//使用していない状態にする
		m_Camera = NULL;
	}

	//ライトの破棄
	if (m_Light != NULL)
	{
		//デバッグプロックの終了処理
		m_Light->Uninit();

		//メモリを開放
		delete m_Light;

		//使用していない状態にする
		m_Light = NULL;
	}

	//テクスチャの破棄
	if (m_Texture != NULL)
	{
		//テクスチャの終了処理
		m_Texture->UnLoad();

		//メモリを開放
		delete m_Texture;

		//使用していない状態にする
		m_Texture = NULL;
	}

	//フェードの破棄
	if (m_Fade != NULL)
	{
		//フェードの終了処理
		m_Fade->Uninit();

		//メモリを開放
		delete m_Fade;

		//使用していない状態にする
		m_Fade = NULL;
	}
}

//================================================================
// 更新処理
//================================================================
void CManager::Update(void)
{
	//レンダラーの更新処理
	m_Renderer->Update();

	//キーボードの更新処理
	m_InputKeyboard->Update();

	//マウスの更新処理
	m_InputMouse->Update();

	//ゲームパッドの更新処理
	m_InputJoyPad->Update();

	m_Enemy = GetEnemy();

	if (m_Camera != NULL)
	{
		//カメラの更新処理
		m_Camera->Update();
	}

	if (m_pScene != NULL)
	{
		//シーンの更新
		m_pScene->Update();
	}
	
	if ((m_InputKeyboard->GetTrigger(DIK_RETURN) == true || m_InputJoyPad->GetTrigger(CInputJoyPad::BUTTON_A, 0) == true) && m_pScene->GetMode() == CScene::MODE_RESULT)
	{//ENTERキーを押したかつシーンがゲームのとき

		if (m_Fade->Get() != m_Fade->FADE_OUT)
		{
			//シーンをタイトルに遷移
			m_Fade->Set(CScene::MODE_TITLE);
		}
	}


	//使用しているテクスチャの合計枚数
	int nTexAll = m_Texture->GetNumTexture();

	m_DebugProc->Print("\n<<テクスチャの総数 : %d>>", nTexAll);
	m_DebugProc->Print("\n<<ENTERで次のシーンに遷移>>");
}

//================================================================
//描画処理
//================================================================
void CManager::Draw(void)
{
	//レンダラーの描画処理
	m_Renderer->Draw();

	if (m_pScene != NULL)
	{
		m_pScene->Draw();
	}
}

//================================================================
// コンストラクタ
//================================================================
CScene::CScene()
{
	//m_mode = MODE_TITLE;
}

//================================================================
// コンストラクタ
//================================================================
CScene::CScene(MODE mode)
{
	m_mode = mode;
}

//================================================================
// デストラクタ
//================================================================
CScene::~CScene()
{

}

//================================================================
// 生成
//================================================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = NULL;

	if (pScene == NULL)
	{
		pScene = new CScene(mode);

		pScene->Init();
	}

	return pScene;
}

//================================================================
// 初期化処理
//================================================================
HRESULT CScene::Init(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:

		if (m_Title == NULL)
		{
			m_Title = new CTitle;

			m_Title->Init();

			CManager::GetInstance()->GetCamera()->SetMode(CCamera::MODE_TITLE);
			CManager::GetInstance()->GetCamera()->Reset();
		}

		break;

	case MODE_TUTORIAL:

		if (m_Tutorial == NULL)
		{
			m_Tutorial = new CTutorial;

			m_Tutorial->Init();

			CManager::GetInstance()->GetCamera()->SetMode(CCamera::MODE_TUTORIAL);
			CManager::GetInstance()->GetCamera()->Reset();
		}

		break;

	case MODE_GAME:

		if (m_Game == NULL)
		{
			m_Game = new CGame;

			m_Game->Init();

			CManager::GetInstance()->GetCamera()->SetMode(CCamera::MODE_GAME);
			CManager::GetInstance()->GetCamera()->Reset();
		}

		break;

	case MODE_RESULT:

		if (m_Result == NULL)
		{
			m_Result = new CResult;

			m_Result->Init();

			CManager::GetInstance()->GetCamera()->SetMode(CCamera::MODE_RESULT);
			CManager::GetInstance()->GetCamera()->Reset();
		}

		break;
	}

	return S_OK;
}

//================================================================
// 終了処理
//================================================================
void CScene::Uninit(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:

		if (m_Title != NULL)
		{
			m_Title->Uninit();

			delete m_Title;

			m_Title = NULL;
		}

		break;

	case MODE_TUTORIAL:

		if (m_Tutorial != NULL)
		{
			m_Tutorial->Uninit();

			delete m_Tutorial;

			m_Tutorial = NULL;
		}

		break;

	case MODE_GAME:

		if (m_Game != NULL)
		{
			m_Game->Uninit();

			delete m_Game;

			m_Game = NULL;
		}

		break;

	case MODE_RESULT:

		if (m_Result != NULL)
		{
			m_Result->Uninit();

			delete m_Result;

			m_Result = NULL;
		}

		break;
	}
}

//================================================================
// 更新処理
//================================================================
void CScene::Update(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:

		if (m_Title != NULL)
		{
			m_Title->Update();
		}

		break;

	case MODE_TUTORIAL:

		if (m_Tutorial != NULL)
		{
			m_Tutorial->Update();
		}

		break;

	case MODE_GAME:

		if (m_Game != NULL)
		{
			m_Game->Update();
		}

		break;

	case MODE_RESULT:

		if (m_Result != NULL)
		{
			m_Result->Update();
		}

		break;
	}
}

//================================================================
// 描画
//================================================================
void CScene::Draw(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:

		if (m_Title != NULL)
		{
			m_Title->Draw();
		}

		break;

	case MODE_TUTORIAL:

		if (m_Tutorial != NULL)
		{
			m_Tutorial->Draw();
		}

		break;

	case MODE_GAME:

		if (m_Game != NULL)
		{
			m_Game->Draw();
		}

		break;

	case MODE_RESULT:

		if (m_Result != NULL)
		{
			m_Result->Draw();
		}

		break;
	}
}

//================================================================
// レンダラーの取得
//================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_Renderer;
}

//================================================================
// キーボードの取得
//================================================================
CInputKeyboard *CManager::GetKeyBoard(void)
{
	return m_InputKeyboard;
}

//================================================================
// マウス取得
//================================================================
CInputMouse *CManager::GetInputMouse(void)
{
	return m_InputMouse;
}

//================================================================
// ゲームパッド取得
//================================================================
CInputJoyPad *CManager::GetInputJoyPad(void)
{
	return m_InputJoyPad;
}

//================================================================
// サウンドの取得
//================================================================
CSound *CManager::GetSound(void)
{
	return m_Sound;
}

//================================================================
// プレイヤーの取得
//================================================================
CPlayer *CManager::GetPlayer(void)
{
	return m_Player;
}

//================================================================
// デバッグプロックの取得
//================================================================
CDebugProc *CManager::GetDebugProc(void)
{
	return m_DebugProc;
}

//================================================================
// カメラの取得
//================================================================
CCamera *CManager::GetCamera(void)
{
	return m_Camera;
}

//================================================================
// ライトの取得
//================================================================
CLight *CManager::GetLight(void)
{
	return m_Light;
}

//================================================================
// テクスチャの取得
//================================================================
CTexture *CManager::GetTexture(void)
{
	return m_Texture;
}

//================================================================
// モデルの取得
//================================================================
CModel *CManager::GetModel(void)
{
	return m_Model;
}

//================================================================
// オブジェクト3Dの取得
//================================================================
CObject3D *CManager::GetObject3D(void)
{
	return m_Object3D;
}

//================================================================
// 敵の取得
//================================================================
CEnemy *CManager::GetEnemy(void)
{
	return m_Enemy;
}
//================================================================
// 当たり判定の取得
//================================================================
CCollision *CManager::GetCollsion(void)
{
	return m_Collision;
}

//================================================================
// エディットの取得
//================================================================
CEdit *CManager::GetEdit(void)
{
	return m_Edit;
}

//================================================================
// マップの取得
//================================================================
CMap *CManager::GetMap(void)
{
	return m_Map;
}

//================================================================
// シーンの取得
//================================================================
CScene *CManager::GetScene(void)
{
	return m_pScene;
}

//================================================================
// フェードの取得
//================================================================
CFade *CManager::GetFade(void)
{
	return m_Fade;
}

//================================================================
// ポーズの取得
//================================================================
CPause *CManager::GetPause(void)
{
	return m_Pause;
}

//================================================================
// 便利関数の取得
//================================================================
CUtility * CManager::GetUtility(void)
{
	return m_pUtility;
}

//================================================================
// エフェクシアの取得
//================================================================
CMyEffekseer* CManager::GetMyEffekseer(void)
{
	return m_pMyEffekseer;
}

//================================================================
// モード設定
//================================================================
void CManager::SetMode(CScene::MODE mode)
{
	if (m_pScene != NULL)
	{
		m_pScene->Uninit();

		delete m_pScene;

		m_pScene = NULL;
	}

	if (m_pScene == NULL)
	{
		m_pScene = new CScene(mode);

		m_pScene->Init();
	}
}

//================================================================
// モードの取得
//================================================================
CScene::MODE CScene::GetMode(void)
{
	return m_mode;
}