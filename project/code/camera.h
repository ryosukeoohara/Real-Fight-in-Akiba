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
class CCameraState;

//===========================================================
// カメラクラスの定義
//===========================================================
class CCamera
{
public:
	CCamera();
	~CCamera();

	struct Info
	{
		D3DXMATRIX mtxView;        // ビューマトリックス
		D3DXMATRIX mtxProjection;  // プロジェクションマトリックス
		D3DXVECTOR3 posV;          // 視点
		D3DXVECTOR3 OldposV;       // 前回の視点の位置
		D3DXVECTOR3 posR;          // 注視点
		D3DXVECTOR3 OldposR;       // 前回の注視点の位置
		D3DXVECTOR3 posU;          // 上方向ベクトル
		D3DXVECTOR3 posVDest;      // 目的の視点
		D3DXVECTOR3 posRDest;      // 目的の注視点
		D3DXVECTOR3 rot;           // 向き
		D3DXVECTOR3 Oldrot;        // 前回の向き 
		D3DXVECTOR3 move;          // 移動

		int nCounter;              // カメラが切り替わっている時間
		float fLength;             // 距離
		float fOldLength;          // 前回の距離
	};

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

	//void GetInstance(void);

	// 設定系
	void SetMode(MODE type);
	void SetPositionR(D3DXVECTOR3 pos);
	void SetPositionV(D3DXVECTOR3 pos);
	void SetRotation(D3DXVECTOR3 Rot);
	void SetDistnce(float fLen);
	void ChangeState(CCameraState* pBehaviour);
	void AdjustToTarget(D3DXVECTOR3 targetPosR, D3DXVECTOR3 targetPosV, float fCorrent);
	void Follow(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot);

	// 取得系
	MODE GetMode(void);
	D3DXVECTOR3 GetRotation(void) { return m_Info.rot; }
	D3DXMATRIX GetView(void) { return m_Info.mtxView; }
	D3DXMATRIX GetProjection(void) { return m_Info.mtxProjection; }
	Info *GetInfo(void) { return &m_Info; }
	static CCamera* GetInstance(void) { return m_pCamera; }

private:
	void Mode(void);
	void Heat(void);
	void Return(void);
	void Debug(void);
	void OnStage(void);
	void Target(void);

	Info m_Info;
	MODE m_mode;

	bool m_bTarget = false;
	CCameraState*m_pState = nullptr;
	CEnemy* m_pEnemy;
	static CCamera* m_pCamera;

	// デバッグ用
	D3DXVECTOR3 m_DebugPosR = {};

};

//===========================================================
// カメラのステイト
//===========================================================
class CCameraState
{
public:
	CCameraState();
	~CCameraState();

	virtual void Update(CCamera* pCamera) = 0;

private:

};

//===========================================================
// 追従するカメラ
//===========================================================
class FollowPlayerCamera : public CCameraState
{
public:
	FollowPlayerCamera();
	~FollowPlayerCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// ゲームスタート時に敵を様々な角度から見るカメラ
//===========================================================
class CameraEnemyOverview : public CCameraState
{
public:
	CameraEnemyOverview();
	~CameraEnemyOverview() {}

	void Update(CCamera* pCamera) override;

private:

	CEnemy* m_pEnemy = nullptr;

	int m_nFocusCounter = 0;
	int m_nLookCount = 0;

};

//===========================================================
// 敵を追従するカメラ
//===========================================================
class FollowEnemyCamera : public CCameraState
{
public:
	FollowEnemyCamera();
	~FollowEnemyCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// 敵を追従するカメラ
//===========================================================
class FollowEnemyOverviewCamera : public CCameraState
{
public:
	FollowEnemyOverviewCamera();
	~FollowEnemyOverviewCamera() {}

	void Update(CCamera* pCamera) override;

private:

	CEnemy* m_pEnemy = nullptr;
	
	int m_nFocusCounter = 0;
	int m_nLookCount = 0;
};

//===========================================================
// 引いていくカメラ
//===========================================================
class CameraZoomOut : public CCameraState
{
public:
	CameraZoomOut();
	~CameraZoomOut() {}

	void Update(CCamera* pCamera) override;

private:

	D3DXVECTOR3 TargetPosV = {};
	int n = 0;
};

//===========================================================
// 目標の位置を向くカメラ
//===========================================================
class CameraTargetFocus : public CCameraState
{
public:
	CameraTargetFocus();
	~CameraTargetFocus() {}

	void Update(CCamera* pCamera) override;

private:

	int n = 0;
	D3DXVECTOR3 TargetPosV = {};
};

//===========================================================
// 固定カメラ
//===========================================================
class FixedCamera : public CCameraState
{
public:
	FixedCamera();
	~FixedCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// キャラクター登場演出用カメラ
//===========================================================
class CutSceneCamera : public CCameraState
{
public:
	CutSceneCamera();
	~CutSceneCamera();

	void Update(CCamera* pCamera) override;

private:

	D3DXVECTOR3 TargetPosV = {};
};

//===========================================================
// キャラクター登場演出用カメラ
//===========================================================
class ReturnPlayerBehindCamera : public CCameraState
{
public:
	ReturnPlayerBehindCamera();
	~ReturnPlayerBehindCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// ボス撃破演出用カメラ
//===========================================================
class FinalBlowCamera : public CCameraState
{
public:
	FinalBlowCamera();
	~FinalBlowCamera() {}

	void Update(CCamera* pCamera) override;

private:

	CEnemy* m_pEnemy = nullptr;

	int m_nFocusCounter = 0;
	int m_nLookCount = 0;

	D3DXVECTOR3 m_ShakePosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ShakePosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float m_fShankeX = 0.0f;
	float m_fShankeZ = 0.0f;

	int m_nShakeTimeCounter = 0;
	int m_nFallDownCount = 0;
	float m_fShakeAngle = 0.0f;
};

//===========================================================
// ヒートアクションカメラ
//===========================================================
class HeatActionCamera : public CCameraState
{
public:
	HeatActionCamera();
	~HeatActionCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// エディットカメラ
//===========================================================
class EditCamera : public CCameraState
{
public:
	EditCamera();
	~EditCamera();

	void Update(CCamera* pCamera) override;

private:

};

namespace camera
{
	CCamera *GetInstance(void);
}


#endif