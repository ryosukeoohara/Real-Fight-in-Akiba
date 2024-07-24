//===========================================================
//
// メイン処理[main.cpp]
// Author 大原怜将
//
//===========================================================
#include "main.h"
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "manager.h"
#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

//===========================================================
// 定数定義
//===========================================================
namespace
{
	const char* WINDOWNAME = "アキバでリアルファイト";  // ウィンドウの名前(キャプションに表示)
	const int IDBUTTONFINISH = 101;                     // 終了ボタン
}

//===========================================================
// プロトタイプ宣言
//===========================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);   //ウインドウプロージャ

//===========================================================
// メイン関数
//===========================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdline, int nCmdshow)
{
	CManager *pManager = nullptr;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),                    // WNDCLASSEXのメモリサイズ
		CS_CLASSDC,                            // ウインドウのスタイル
		WindowProc,                            // ウインドウプロージャ
		0,                                     // 0にする（通常は使わない）
		0,                                     // 0にする（通常は使わない）
		hInstance,                             // インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),       // タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),           // マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),            // クライアント領域の背景色
		NULL,                                  // メニューバー
		CLASS_NAME,                            // ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION),       // ファイルのアイコン
	};

	HWND hWnd;      // ウインドウハンドル（識別子）

	MSG msg;        // メッセージを格納する変数

	RegisterClassEx(&wcex);       // ウインドウクラスの登録

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx
	(
		0,                            // 拡張ウインドウスタイル
		CLASS_NAME,                   // ウインドウクラスの名前
		WINDOWNAME,                   // ウインドウの名前
		WS_OVERLAPPEDWINDOW,          // ウインドウスタイル
		CW_USEDEFAULT,                // ウインドウの左上X座標
		CW_USEDEFAULT,                // ウインドウの左上Y座標
		(rect.right - rect.left),     // ウインドウの幅
		(rect.bottom - rect.top),     // ウインドウの高さ
		NULL,                         // 親ウインドウのハンドル
		NULL,                         // メニューハンドルまたは子ウインドウID
		hInstance,                    // インスタンスハンドル
		NULL                          // ウインドウ作成データ
	);

	if (pManager == nullptr)
	{
		pManager = new CManager;
		pManager->GetInstance();
	}

	// マネージャの初期化処理
	if (pManager != nullptr)
	{
		if (FAILED(pManager->GetInstance()->Init(hInstance, hWnd, FALSE)))
		{
			return -1;
		}
	}

	DWORD dwCurrentTime;   // 現在時刻
	DWORD dwExecLastTime;  // 最後に処理した時刻
	DWORD dwwFrameCount;   // フレームカウント
	DWORD dwFPSLastTime;   // 最後にFPSを計測した時刻

	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();
	dwwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//ウインドウの表示
	ShowWindow(hWnd, nCmdshow);       // ウインドウの表示状態を設定
	UpdateWindow(hWnd);               // クライアント領域を更新

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// Windowsの処理

			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらメッセージループを抜ける

				break;
			}
			else
			{
				TranslateMessage(&msg);       // 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);        // ウインドウプロージャへメッセージを送出
			}
		}
		else
		{// DirectXの処理

			dwCurrentTime = timeGetTime();    // 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
			 // FPSを計測

				dwFPSLastTime = dwCurrentTime;       // FPSを測定した時刻を保存
				dwwFrameCount = 0;                   // フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60分の1秒経過

				dwExecLastTime = dwCurrentTime;

				if (pManager->GetInstance() != NULL)
				{
					// マネージャの更新処理
					pManager->GetInstance()->Update();

					// マネージャの描画処理
					pManager->GetInstance()->Draw();
				}

				dwwFrameCount++;     // フレームカウントを加算
			}
		}
	}

	// マネージャの終了
	if (pManager->GetInstance() != nullptr)
	{
		pManager->GetInstance()->Release();

		pManager = nullptr;
	}
	
	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//================================================================
// ウインドウプロージャ
//================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
	
	switch (uMsg)
	{
	case WM_DESTROY:         // ウインドウ破棄のメッセージ

		// WM_QUITメッセージを送る
		PostQuitMessage(0);

		break;

	case WM_KEYDOWN:         // キー押下のメッセージ

		switch (wParam)
		{
		case VK_ESCAPE:      // [ESC]キーが押された   

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				// ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{// 親ウインドウの×が押された

			// ウインドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;     // 0を返さないと終了してしまう
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}