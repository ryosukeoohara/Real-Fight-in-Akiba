//===========================================================
//
// マネージャ処理[manager.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//===========================================================
//前方宣言
//===========================================================
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoyPad;
class CSound;
class CPlayer;
class CDebugProc;
class CCamera;
class CLight;
class CTexture;
class CModel;
class CObject3D;
class CEnemy;
class CCollision;
class CEdit;
class CMap;
class CGame;
class CTutorial;
class CTitle;
class CResult;
class CFade;
class CPause;
class CUtility;
class CMyEffekseer;

//===========================================================
// シーンクラス定義
//===========================================================
class CScene
{
public:

	enum MODE
	{
		MODE_TITLE = 0,   // タイトル
		MODE_TUTORIAL,    // チュートリアル
		MODE_GAME,        // ゲーム
		MODE_RESULT,      // リザルト
		MODE_RANKING,     // ランキング
		MODE_MAX
	};

	CScene();             // コンストラクタ
	CScene(MODE mode);    // コンストラクタ
	~CScene();            // デストラクタ

	HRESULT Init(void);   // 初期化処理
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理
	void Draw(void);      // 描画処理

	static CScene *Create(MODE mode);  // 生成処理
	static CScene::MODE GetMode(void); // モード取得

private:
	static MODE m_mode;
	static CGame *m_Game;
	static CTutorial *m_Tutorial;
	static CTitle *m_Title;
	static CResult *m_Result;

};

//===========================================================
// マネージャクラス定義
//===========================================================
class CManager
{
public:		// 誰でもアクセスできる

	static CManager *GetInstance();
	static HRESULT Release(void);
	CManager();  // コンストラクタ
	~CManager(); // デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);       // 初期化処理
	void Uninit(void);  // 終了処理
	void Update(void);  // 更新処理
	void Draw(void);    // 描画処理

	CRenderer *GetRenderer(void);
	CInputKeyboard *GetKeyBoard(void);
	CInputMouse *GetInputMouse(void);
	CInputJoyPad *GetInputJoyPad(void);
	CSound *GetSound(void);
	CPlayer *GetPlayer(void);
	CDebugProc *GetDebugProc(void);
	CCamera *GetCamera(void);
	CLight *GetLight(void);
	CTexture *GetTexture(void);
	CModel *GetModel(void);
	CObject3D *GetObject3D(void);
	CEnemy *GetEnemy(void);
	CCollision *GetCollsion(void);
	CEdit *GetEdit(void);
	CMap *GetMap(void);
	CScene *GetScene(void);
	CFade *GetFade(void);
	CPause *GetPause(void);
	CUtility *GetUtility(void);
	CMyEffekseer* GetMyEffekseer(void);

	void SetMode(CScene::MODE mode);
	void SetDefeat(int nValue) 
	{ 
		m_nDefeat = nValue; 
	}

	void SetbPause(bool bpause) { m_bPause = bpause; }
	bool GetbPause(void) { return m_bPause; }
	int GetDefeat(void) 
	{ 
		return m_nDefeat; 
	}

private:	//自分だけがアクセスできる

	static CManager *m_pManager;
	CRenderer *m_Renderer;
	CInputKeyboard *m_InputKeyboard;
	CInputMouse *m_InputMouse;
	CInputJoyPad *m_InputJoyPad;
	CSound *m_Sound;
	CPlayer *m_Player;
	CDebugProc *m_DebugProc;
	CCamera *m_Camera;
	CLight *m_Light;
	CTexture *m_Texture;
	CModel *m_Model;
	CObject3D *m_Object3D;
	CEnemy *m_Enemy;
	CCollision *m_Collision;
	CEdit *m_Edit;
	CMap *m_Map;
	CScene *m_pScene;
	CFade *m_Fade;
	CPause *m_Pause;
	CUtility *m_pUtility;
	CMyEffekseer* m_pMyEffekseer;

	int m_ResetCounter;  // リセットした回数(デバッグ用)
	int m_nDefeat;       // プレイヤーが敵を倒した数
	bool m_bPause;       // ポーズ中かどうか
	bool m_EditUse;      // エディットモードにするかどうか
};

#endif // !1