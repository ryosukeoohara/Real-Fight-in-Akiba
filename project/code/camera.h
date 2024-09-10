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
class CCameraBehaviour;

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
	void ChangeBehaviour(CCameraBehaviour* pBehaviour);

	// 取得系
	MODE GetMode(void);
	D3DXVECTOR3 GetRotation(void) { return m_Info.rot; }
	D3DXMATRIX GetView(void) { return m_Info.mtxView; }
	D3DXMATRIX GetProjection(void) { return m_Info.mtxProjection; }
	Info *GetInfo(void) { return &m_Info; }

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
	CCameraBehaviour *m_pBehaviour = nullptr;
	CEnemy* m_pEnemy;
	static CCamera* m_pCamera;

	// デバッグ用
	D3DXVECTOR3 m_DebugPosR = {};

};

//===========================================================
// ビヘイビア
//===========================================================
class CCameraBehaviour
{
public:
	CCameraBehaviour();
	~CCameraBehaviour();

	virtual void Update(CCamera* pCamera) = 0;

private:

};

//===========================================================
// 追従するカメラ
//===========================================================
class FollowPlayerCamera : public CCameraBehaviour
{
public:
	FollowPlayerCamera();
	~FollowPlayerCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// 固定カメラ
//===========================================================
class FixedCamera : public CCameraBehaviour
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
class CutSceneCamera : public CCameraBehaviour
{
public:
	CutSceneCamera();
	~CutSceneCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// キャラクター登場演出用カメラ
//===========================================================
class ReturnPlayerBehindCamera : public CCameraBehaviour
{
public:
	ReturnPlayerBehindCamera();
	~ReturnPlayerBehindCamera();

	void Update(CCamera* pCamera) override;

private:

};

//===========================================================
// ヒートアクションカメラ
//===========================================================
class HeatActionCamera : public CCameraBehaviour
{
public:
	HeatActionCamera();
	~HeatActionCamera();

	void Update(CCamera* pCamera) override;

private:

};

namespace camera
{
	CCamera *GetInstance(void);
}


#endif