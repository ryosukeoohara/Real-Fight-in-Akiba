//===========================================================
//
// エフェクシア[MyEffekseer.h]
// Author 大原怜将
//
//===========================================================
#ifndef _EFFEKSEER_H_     //このマクロが定義されてなかったら
#define _EFFEKSEER_H_     //2重インクルードを防止のマクロを定義する

#define MAX_EFK   (64)

//===========================================================
// エフェクシアクラス定義
//===========================================================
class CMyEffekseer
{
public:
	CMyEffekseer();     // コンストラクタ
	~CMyEffekseer();    // デストラクタ

	// 種類
	enum TYPE
	{
		TYPE_NONE = 0,  // なんもない
		TYPE_IMPACT,    // 衝撃波
		TYPE_HIT,       // ヒット
		TYPE_ATTACK,    // 攻撃の予兆
		TYPE_MAX
	};

	// 情報
	struct Info
	{
		Effekseer::Vector3D pos;       // 位置
		Effekseer::Vector3D rot;       // 向き
		Effekseer::Vector3D scale;     // 大きさ
		Effekseer::EffectRef effect;   // エフェクト
		Effekseer::Handle efkHandle;   // ハンドル
		const char* EfkName;           // 名前
		int32_t time;                  // 時間
	};

	void Init(void);                   // 初期化処理
	void Uninit(void);                 // 終了処理 
	void Update(void);                 // 更新処理
	void Draw(void);                   // 描画処理

	// 設定
	void Set
	(
		TYPE type, 
		::Effekseer::Vector3D pos = {0.0f, 0.0f, 0.0f}, 
		::Effekseer::Vector3D rot = { 0.0f, 0.0f, 0.0f }, 
		::Effekseer::Vector3D scale = { 1.0f, 1.0f, 1.0f }
	);  // エフェクト

	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);                                                     // モジュール

	// 取得
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer(void) { return m_EfkRenderer; }                            // エフェクトレンダラー
	::Effekseer::ManagerRef GetEfkManager(void) { return m_EfkManager; }                                                // エフェクトマネージャ
	

	void Release(int idx);                                   // 指定したエフェクトを終了する
	static const char* m_apEfkName[CMyEffekseer::TYPE_MAX];  // エフェクトの名前

private:

	// メンバ変数 
	Info m_Info[MAX_EFK];
	EffekseerRendererDX9::RendererRef m_EfkRenderer;
	::Effekseer::Matrix44 m_CameraMatrix;
	::Effekseer::Matrix44 m_ProjectionMatrix;
	::Effekseer::ManagerRef m_EfkManager;
	::Effekseer::Vector3D m_ViewerPosition;
	int m_nNum;
	int32_t m_time = 0;                  // 時間
};

#endif