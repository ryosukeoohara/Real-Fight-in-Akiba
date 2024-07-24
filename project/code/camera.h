//===========================================================
//
// カメラ処理 [camera.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _CAMERA_H_             //このマクロ定義がされてなかったら
#define _CAMERA_H_             //2重インクルード防止のマクロ定義をする

//===========================================================
// 前方宣言
//===========================================================
class CEnemy;

//===========================================================
// カメラクラスの定義
//===========================================================
class CCamera
{
public:
	CCamera();
	~CCamera();

	enum MODE
	{
		MODE_NONE = 0,
		MODE_TITLE,      // タイトルのカメラ
		MODE_TUTORIAL,   // チュートリアル
		MODE_GAME,       // ゲームのカメラ
		MODE_RESULT,     // リザルトのカメラ
		MODE_HEAT,       // ヒートアクション
		MODE_RETURN,     // 元の位置に戻る
		MODE_DEBUG,      // ちょーせい
		MODE_ONSTAGE,    // 登場
		MODE_TARGET,     // 敵をターゲットするカメラ
		MODE_MAX 
	};

	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_HEAT,
		TYPE_MAX
	};

	void Init(void);          //カメラの初期化処理    
	void Uninit(void);        //カメラの終了処理
	void Update(void);        //カメラの更新処理
	void CameraV(void);
	void Tutorial(void);
	void Edit(void);          //エディットのカメラ
	void CameraR(void);
	void Title(void);
	void Scope(void);
	void SetCamera(void);
	void Reset(void);

	void Boss(void);

	// 設定系
	void SetMode(MODE type);
	void SetPositionR(D3DXVECTOR3 pos);
	void SetPositionV(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 Rot);
	void SetDistnce(float fLen);

	// 取得系
	MODE GetMode(void);
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	D3DXMATRIX GetView(void) { return m_mtxView; }
	D3DXMATRIX GetProjection(void) { return m_mtxProjection; }

private:
	void Mode(void);
	void Heat(void);
	void Return(void);
	void Debug(void);
	void OnStage(void);
	void Target(void);

	D3DXMATRIX m_mtxView;        // ビューマトリックス
	D3DXMATRIX m_mtxProjection;  // プロジェクションマトリックス
	D3DXVECTOR3 m_posV;          // 視点
	D3DXVECTOR3 m_OldposV;       // 前回の視点の位置
	D3DXVECTOR3 m_posR;          // 注視点
	D3DXVECTOR3 m_OldposR;       // 前回の注視点の位置
	D3DXVECTOR3 m_posU;          // 上方向ベクトル
	D3DXVECTOR3 m_posVDest;      // 目的の視点
	D3DXVECTOR3 m_posRDest;      // 目的の注視点
	D3DXVECTOR3 m_rot;           // 向き
	D3DXVECTOR3 m_Oldrot;        // 前回の向き 
	D3DXVECTOR3 m_move;          // 移動
	MODE m_mode;

	int m_nCounter;     // カメラが切り替わっている時間
	float m_fLen;       // 距離
	float m_fOldLen;    // 前回の距離
	bool m_bTarget = false;

	CEnemy* m_pEnemy;

	// デバッグ用
	D3DXVECTOR3 m_DebugPosR = {};

};



#endif
