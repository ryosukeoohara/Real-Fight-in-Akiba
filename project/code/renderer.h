//===========================================================
//
// レンダラー処理[renderer.h]
// Author 大原怜将
//
//===========================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//===========================================================
// レンダラークラス定義
//===========================================================
class CRenderer
{
public:		//誰でもアクセスできる

	CRenderer();  // コンストラクタ
	~CRenderer(); // デストラクタ

	HRESULT Init(HWND hWnd, BOOL bWindow);       // 初期化処理
	void Uninit(void);                           // 終了処理
	void Update(void);                           // 更新処理
	void Draw(void);                             // 描画処理
	
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }  // デバイスの取得

private:	//自分だけがアクセスできる

	LPDIRECT3D9 m_pD3D = NULL;                    // Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;        // Direct3Dオブジェクトへのポインタ

};

#endif