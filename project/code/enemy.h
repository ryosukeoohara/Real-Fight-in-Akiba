//===========================================================
//
// 敵の処理 [enemy.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "object.h"
#include "player.h"

//===========================================================
// 前方宣言
//===========================================================
class CMotion;
class CCharacter;
class CEnemyState;

//===========================================================
// クラス定義
//===========================================================
class CEnemy : public CObject
{
public:

	// 敵の種類
	enum TYPE
	{
		WEAK = 0,  // ザコ
		BOSS,      // ボス
		MAX
	};

	// 追尾
	enum CHASE
	{
		CHASE_OFF = 0,
		CHASE_ON,
		CHASE_MAX
	};

	enum MOBILITY
	{
		Mobile = 0,  // 動ける
		Immobile,    // 動けない
		MOBILITY_MAX
	};

	MOBILITY m_Mobility;

	CHASE m_Chase;

public:

	// モーション
	enum MOTIONTYPE
	{
		MOTION_NEUTRAL = 0,          // 待機
		MOTION_DASH,                 // 移動
		MOTION_ATTACK,               // 攻撃
		MOTION_DAMEGE,               // 攻撃受けた
		MOTION_GRAP,                 // 投げられ待ち
		MOTION_FALLDOWN,             // 転ぶ
		
		MOTION_HEATACTELECTROWAIT,   // ヒートアクション:電子レンジ待機
		MOTION_HEATACTELECTRO,       // ヒートアクション:電子レンジびりびり
		MOTION_HEATACTFAINTING,      // ヒートアクション:電子レンジ気絶

		MOTION_GETUP,                // 起き上がり
		MOTION_DEATH,                // 死亡

		MOTION_PAINFULDAMAGE,        // 痛い攻撃を受けた
		MOTION_HEATDAMEGE,           // 攻撃受けた
		//MOTION_ONSTEGE,
		MOTION_MAX
	};

	// 状態
	enum STATE
	{
		STATE_NONE = 0,             // なんもない
		STATE_NEUTRAL,              // 待機
		STATE_DASH,                 // 移動
		STATE_ATTACK,               // 攻撃
		STATE_DAMEGE,               // 攻撃受けた
		STATE_GRAP,                 // 投げられ待ち
		STATE_PAINFULDAMAGE,        // 痛い攻撃を受けた
		STATE_HEATDAMEGE,           // 攻撃受けた
		STATE_GETUP,                // 起き上がり
		STATE_HEATACTELECTROWAIT,   // ヒートアクション:電子レンジ待機
		STATE_HEATACTELECTRO,       // ヒートアクション:電子レンジびりびり
		STATE_HEATACTFAINTING,      // ヒートアクション:電子レンジ気絶
		STATE_FALLDOWN,             // 転ぶ
		STATE_DEATH,                // 死亡
		STATE_MAX
	};

	// 情報
	struct INFO
	{							    
		D3DXVECTOR3 pos;            // 位置
		D3DXVECTOR3 posOld;         // 前回の位置
		D3DXVECTOR3 rot;            // 向き
		D3DXVECTOR3 move;           // 移動量
		D3DXMATRIX mtxWorld;        // ワールドマトリックス
		STATE state;                // 状態
		int nIdxID;                 // インデックス番号
		int nLife;                  // 体力
		bool bDraw;                 // 描画するかどうか
	};

	CEnemy();                                  //コンストラクタ
	CEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);                   //コンストラクタ
	~CEnemy();                                 //デストラクタ
										       
	virtual HRESULT Init(void);                        //初期化
	virtual void Uninit(void);                         //終了
	virtual void Update(void);                         //更新
	virtual void Draw(void);                           //描画

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nlife, int nPriority = 5);    //生成
	void ReadText(const char *fliename);             //テキストファイル読み込み

	//　設定系
	void SetPosition(D3DXVECTOR3 pos) { m_Info.pos = pos; }         // 位置
	void SetRotition(D3DXVECTOR3 rot) { m_Info.rot = rot; }         // 向き
	void SetMove(D3DXVECTOR3 move) { m_Info.move = move; }          // 移動量
	void SetState(STATE state) { m_Info.state = state; }            // 状態
	void SetLife(int nLife) { m_Info.nLife = nLife; }               // 体力
	void SetCurrent(D3DXMATRIX *Current) { m_pCurrent = Current; }  // 親のマトリックス
	void SetIdx(int idx) { m_Info.nIdxID = idx; }                   // ID
	void SetType(TYPE type) { m_Type = type; }                      // 種類
	void SetNumAll(int nNum) { m_nNumAll = nNum; }
	virtual void SetChase(CHASE cha);
	void SetDraw(bool value) { m_Info.bDraw = value; }
	void SetMobile(void) { m_Mobility = Mobile; }                    // 動けるようにする
	void SetImmobile(void) { m_Mobility = Immobile; }                // 動けないようにする

	// 取得系
	D3DXVECTOR3 GetPosition(void) { return m_Info.pos; }       // 位置
	D3DXVECTOR3 GetRotition(void) { return m_Info.rot; }       // 向き
	D3DXVECTOR3 GetMove(void) { return m_Info.move; }          // 移動量
	STATE GetState(void) { return m_Info.state; }              // 状態
	int GetLife(void) { return m_Info.nLife; }                 // 体力
	int GetIdxID(void) { return m_Info.nIdxID; }
	TYPE GetType(void) { return m_Type; }
	CCharacter **GetCharcter(void) { return m_apModel; }
	static int GetNumAll(void) { return m_nNumAll; }
 	CMotion *GetMotion(void) { return m_pMotion; }
	MOBILITY GetMobility(void) { return m_Mobility; }
	static CEnemy *GetTop(void) { return m_pTop; }
	CEnemy *GetNext(void) { return m_pNext; }
	CEnemy::INFO *GetInfo(void) { return &m_Info; }

	virtual void Damege(void) = 0;

	//virtual void ChangeState(CEnemyState *pEnemy) = 0;
	virtual void RecoverFromDamage(void) = 0;

	void HitDetection(D3DXVECTOR3 MyPos, float attackrange, float targetradius);

protected:

	// 制御処理
	void Controll(void);
	TYPE m_Type;

private:

	void ListOut(void);
	
	INFO m_Info;                   // 情報
	CMotion *m_pMotion;            // モーションへのポインタ
	CCharacter **m_apModel;        // モデル(パーツ)へのポインタ
	CGage3D *m_pLife3D;            // ゲージのポインタ
	CGage2D *m_pLife2D;            // ゲージのポインタ
	static int m_nNumAll;          // 敵の総数
	int m_nDamegeCounter;          // ダメージ状態でいるカウント
	static int m_nIdx;             
	D3DXMATRIX *m_pCurrent;        // 親のマトリックス
	static CEnemy *m_pTop;         //先頭のオブジェクトへのポインタ
	static CEnemy *m_pCur;         //最後尾のオブジェクトへのポインタ
	CEnemy *m_pNext;
	CEnemy *m_pPrev;               //前のオブジェクトへのポインタ
	bool m_bDeath;                 //死亡フラグ
	
	//*=============================================================================
	// 外部ファイル読み込み用
	//*=============================================================================
	D3DXVECTOR3 m_Readpos;                     // 位置読み込み用
	D3DXVECTOR3 m_Readrot;                     // 向き読み込み用
	int m_nidx;                                // インデックス番号
	int m_nParent;                             // 親のインデックス番号
	int m_nNumModel;                           // モデル(パーツ)の総数
	char m_filename[128] = {};                 // 文字読み込み用

};

////===========================================================
//// ステイト
////===========================================================
//class CEnemyState
//{
//public:
//	CEnemyState() {};
//	~CEnemyState() {};
//
//	virtual void Update(CEnemy* pEnemy) = 0;
//
//private:
//
//};



#endif // !_ENEMY_H_