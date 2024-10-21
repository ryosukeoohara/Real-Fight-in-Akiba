//===========================================================
//
// プレイヤーの処理[player.h]
// Author 大原怜将
//
//===========================================================
#ifndef _PLAYER_H_             //このマクロ定義がされてなかったら
#define _PLAYER_H_             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "object.h"

//===========================================================
// 前方宣言
//===========================================================
class CMotion;
class CCharacter;
class CObjectX;
class CGage2D;
class CGage3D;
class CBillBoard;
class CEnemy;
class CObject2D;
class CItem;
class CHeatAction;

//===========================================================
// プレイヤークラス定義
//===========================================================
class CPlayer : public CObject
{
public:
	// 状態
	enum STATE
	{
		STATE_NONE = 0,     // なんもない
		STATE_NEUTRAL,      // ニュートラル
		STATE_GRAPNEUTRAL,  // 掴み状態のニュートラル
		STATE_MOVE,         // 移動
		STATE_ATTACK,       // 攻撃
		STATE_EVASION,      // 回避
		STATE_LIFT,         // アイテム掴んでる
		STATE_GRAP,         // 掴み
		STATE_THROW,        // 投げる
		STATE_HEAT,         // ヒートアクション
		STATE_GRAPDASH,     // 掴んで走る
		STATE_ENEMYGRAP,    // 敵を投げる
		STATE_GRAPWALK,     // 敵を掴んで歩く
		STATE_POWERFUL,     // 地面にたたきつける
		STATE_DAMEGE,       // ダメージ
		STATE_INVINCIBE,    // 無敵
		STATE_DEATH,        // 死亡
		STATE_OTA,          // タイトルのオタ芸
		STATE_MOVESTOP,     // 移動停止
		STATE_MAX
	};

	enum HEAT
	{
		HEAT_NONE = 0,  // なんもない
		HEAT_SMASH,     // たたきつけ
		HEAT_FIRE,      // 電子レンジ
		HEAT_MAX
	};

	// 連撃
	enum ATTACKTYPE
	{
		TYPE_ATTACK1 = 0,       // 一段目
		TYPE_ATTACK2,           // 二段目
		TYPE_ATTACK3,           // 三段目
		TYPE_HEATACTBIKE,       // ヒートアクション・バイク
		TYPE_HEATACTREF,        // ヒートアクション・冷蔵庫
		TYPE_HEATACTMICROWAVE,  // ヒートアクション・電子レンジ
		TYPE_HEATACTPOWERFUL,   // ヒートアクション・地面にたたきつける
		TYPEMAX_ATTACK
	};

	//モーション
	enum MOTIONTYPE
	{
		TYPE_NEUTRAL = 0,       // ニュートラル
		TYPE_MOVE,              // 移動
		TYPE_ATTACK01,          // 連撃1段目
		TYPE_ATTACK02,          // 連撃2段目
		TYPE_ATTACK03,          // 連撃3段目 
		TYPE_EVASION,           // 回避
		TYPE_LIFT,              // 持ち上げる
		TYPE_THROW,             // 投げる
		TYPE_GRAPDASH,          // 持って走る
		TYPE_GRAP,              // 掴み
		TYPE_GRAPNEUTRAL,       // 掴みニュートラル
		TYPE_ENEMYGRAP,         // 敵を掴んで投げる
		TYPE_GRAPWALK,          // 掴んで歩く
		TYPE_POWERFUL,          // 地面たたきつけ
		TYPE_DAMAGE,            // ダメージ
		TYPE_OTA,               // タイトルでするオタ芸
		TYPE_MOVESTOP,          // 移動停止
		TYPE_MAX
	};

	enum MOBILITY
	{
		Mobile = 0,  // 動ける
		Immobile,    // 動けない
		MAX
	};

	// 情報
	struct INFO
	{
		D3DXVECTOR3 pos;                  // 位置
		D3DXVECTOR3 posOld;               // 前回の位置
		D3DXVECTOR3 rot;                  // 向き
		D3DXVECTOR3 move;                 // 移動
		D3DXCOLOR col;                    // 色
		D3DXMATRIX mtxWorld;              // ワールドマトリックス
		STATE state;                      // 状態
		ATTACKTYPE Atc;                   // 攻撃を種類
		int nLife;                        // 体力
		float fRadius;                    // 横幅
	};

private:

	enum DAMEGESTATE
	{
		DAMEGE_NONE = 0,
		DAMEGE_SMALL,
		DAMEGE_BIG,
		DAMEGE_MAX
	};

	struct GRAP
	{
		CItem *pItem;                     // アイテム
		CEnemy *pEnemy;                   // 敵
	};

	INFO m_Info;                          // 情報
	GRAP m_Grap;                          // 掴むでいるもの
	HEAT m_HeatAct;
	MOBILITY m_Mobility;
	
public:
	CPlayer();  //コンストラクタ
	CPlayer(D3DXVECTOR3 pos, int nPriority = 2);  //コンストラクタ(オーバーロード)
	~CPlayer();  //デストラクタ

	HRESULT Init(void);        // 初期化処理    
	void Uninit(void);		   // 終了処理
	void Update(void);         // 更新処理
	void Draw(void);           // 描画処理

	static CPlayer *Create(D3DXVECTOR3 pos, int nPriority = 2);  // 生成
	static CPlayer *Create(void);
	void Damage(int nDamage, D3DXVECTOR3 KnockBack);			 // 攻撃をくらった時の処理
	void TitleWalk(void);                     // タイトル
	void CreateRippleshEffect(void);  // 水たまりに入った時のエフェクトを生成
	void CreateMoveStopEffect(void);  // 移動停止した際に出るエフェクトを生成

	//　設定系
	void SetPosition(D3DXVECTOR3 pos) { m_Info.pos = pos; }         // 位置設定
	void SetRotition(D3DXVECTOR3 rot) { m_Info.rot = rot; }         // 向き設定
	void SetMove(D3DXVECTOR3 move) { m_Info.move = move; }           // 移動量設定
	void SetState(STATE state) { m_Info.state = state; }             // 状態
	void SetGrapItem(CItem *obj) { m_Grap.pItem = obj; }             // アイテム
	void SetGrapEnemy(CEnemy *enemy) { m_Grap.pEnemy = enemy; }      // 敵
	void SetLife(int nlife) { m_Info.nLife = nlife; }                // 体力
	void SetMobile(void) { m_Mobility = Mobile; }                    // 動けるようにする
	void SetImmobile(void) { m_Mobility = Immobile; }                // 動けないようにする
	void SetDefeat(int nValue) { m_nDefeat = nValue; }
	void SetUseMicroCount(int nValue) { m_nUseCounter = nValue; }
	void SetbHeatActFlag(bool bValue) { m_bHeatActFlag = bValue; }
	void SetDiff(float diff) { m_fDest = diff; }

	// 取得系
	D3DXVECTOR3 GetPosition(void) { return m_Info.pos; }       // 位置取得
	D3DXVECTOR3 GetRotition(void) { return m_Info.rot; }       // 向き取得
	D3DXVECTOR3 GetMove(void) { return m_Info.move; }          // 移動量取得
	STATE GetState(void) { return m_Info.state; }              // 状態
	int GetLife(void) { return m_Info.nLife; }                 // 体力
	float GetRadius(void) { return m_Info.fRadius; }           // 横幅
	D3DXMATRIX *GetMatrix(void) { return &m_Info.mtxWorld; }   // マトリックス
	CCharacter **GetChar(void) { return m_ppCharacter; }
	bool GetbAttack(void) { return m_bAttack; }                // 攻撃中かどうか
	CMotion *GetMotion(void) { return m_pMotion; }
	ATTACKTYPE GetActType(void) { return m_Info.Atc; }
	CItem *GetItem(void) { return m_pItemMicro; }
	CEnemy *GetGrapEnemy(void) { return m_Grap.pEnemy; }
	CItem *GetGrapItem(void) { return m_Grap.pItem; }
	static CPlayer *GetInstance(void) { return m_pPlayer; }
	int GetDefeat(void) { return m_nDefeat; }
	HEAT GetHeatAct(void) { return m_HeatAct; }
	bool GetHeatActFlag(void) { return m_bHeatActFlag; }
	MOBILITY GetMobility(void) { return m_Mobility; }
	CPlayer::INFO* GetInfo(void) { return &m_Info; }
	bool GetbGrap(void) { return m_bGrap; }
	bool IsHitCollision(void);

private:

	// メンバ関数
	void Control(void);                   // 制御
	void Move(void);                      // 移動
	void Attack(void);                    // 攻撃
	void Grap(void);                      // 掴み
	void Evasion(void);                   // 回避
	void State(void);                     // 状態
	void Heat(void);                      // ヒートアクション
	void SetHeatActType(void);
	bool IsHeatAct(D3DXVECTOR3 TargetPos);
	void ReadText(const char *fliename);  // テキストファイル読み込み
	float EnemyDistance(void);              // 敵との距離
	float ItemDistance(void);
	void Collision(void);
	void GrapEnemy(bool value);
	void GrapItem(bool value);
	void MoveLimit(void);
	void DashEffect(void);
	void Hit(void);
	

	// メンバ変数
	int m_nNumModel;                    //モデル(パーツ)の総数
	int m_nIdxTexture;
	int m_nIdxShaadow;
	int m_nIdx;
	int m_nParent;
	int m_nCntColi;
	int m_nDamegeCounter;
	int m_nUseCounter;
	int m_nEffectCounter = 0;
	char m_filename[128] = {};

	D3DXVECTOR3 m_Readpos;
	D3DXVECTOR3 m_Readrot;
	D3DXVECTOR3 m_posOrigin;             
	CMotion *m_pMotion;                   // モーションへのポインタ
	CCharacter **m_ppCharacter;           // キャラクターへのポインタ
	CItem *m_pItemMicro;                  // 電子レンジ保存用
	CGage2D *m_pLife;                     // ゲージのポインタ
	CGage2D *m_pStamina;                  // ゲージのポインタ
	CEnemy *m_pEnemy;                     // 一時保存用
	CItem *m_pItem;                       // 一時保存用
	CObject2D *m_pBotton;
	CObject2D *m_pGekiatu;
	CHeatAction *m_pHeatAct;
	static CPlayer *m_pPlayer;
	int m_nDefeat;  // 敵を倒した数
	int m_nIdxEne;
	int m_nIdxItem;
	int m_nDamageCounter;
	int m_nCntSound;
	float m_fDest;
	float m_fDestOld;
	float m_fDiff;
	float m_fGrapRot;
	float m_fStamina;                     // スタミナ
	bool m_bDesh;                         // ダッシュ
	bool m_bAttack;                       // 攻撃
	bool m_bEvasion;                      // 回避
	bool m_bLift;                         // 持ち上げる
	bool m_bGrap;                         // 投げ
	bool m_bInvi;                         // 無敵かどうか
	bool m_bHeatActFlag;                  // ヒートアクションしているかどうか

	//チュートリアルで使う関数,変数
	void ControlTutorial(void);          // チュートリアルのプレイヤーの制御
	bool m_bPushW;                       // Wキーを押した
	bool m_bPushA;                       // Aキーを押した
	bool m_bPushS;                       // Sキーを押した
	bool m_bPushD;                       // Dキーを押した
	bool m_bAvoi;                        // 回避した
	bool m_bWhee;                        // ホイールを回転した
};

#endif