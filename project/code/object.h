//===========================================================
//
// オブジェクト処理[object.h]
// Author 大原怜将
//
//===========================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//マクロ定義
#define NUM_PRIORITY  (8)     // 優先順位

//===========================================================
// オブジェクトクラス定義
//===========================================================
class CObject
{
public:		//誰でもアクセスできる

	CObject(int nPriority = 3);             //コンストラクタ
	virtual ~CObject();    //デストラクタ

	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,    // プレイヤー
		TYPE_ENEMY,     // 敵
		TYPE_BULLET,    // 弾
		TYPE_EXPLOSION, // 爆発
		TYPE_EFFECT,    // エフェクト
		TYPE_PARTICL,   // パーティクル
		TYPE_ITEM,      // アイテム
		TYPE_TIME,      // 時間
		TYPE_SCORE,     // スコア
		TYPE_MAX
	} TYPE;

	virtual HRESULT Init(void) = 0;   // 初期化処理
	virtual void Uninit(void) = 0;    // 終了処理
	virtual void Update(void) = 0;    // 更新処理
	virtual void Draw(void) = 0;      // 描画処理
										 
	static void ReleaseAll(void);     // すべて終了
	static void UpdateAll(void);      // すべて更新
	static void DrawAll(void);        // すべて描画
	void Release(void);

	

	// 設定系
	void SetType(TYPE type) { m_ntype = type; }                   // 種類
	void SetPriority(int nPriority) { m_nPriority = nPriority; }  //　優先順位

	// 取得系
	TYPE GetType(void) { return m_ntype; }             // 種類
	static CObject *Getobject(int nIdx);               // 
	static int GetNumAll(void) { return m_nNumAll; }   // 総数
	int GetID(void) { return m_nID; }                  // ID
	
	
private:	//自分だけがアクセスできる

	static void DeadChuck(void);           // 死亡しているかチェック

	static int m_nNumAll;                  // オブジェクトの総数
	int m_nID;                             // 自分自身のID
	int m_nPriority;                       // 優先順位
	static CObject *m_pTop[NUM_PRIORITY];  // 先頭のオブジェクトへのポインタ
	static CObject *m_pCur[NUM_PRIORITY];  // 最後尾のオブジェクトへのポインタ
	CObject *m_pNext;                      // 次のオブジェクトへのポインタ
	CObject *m_pPrev;                      // 前のオブジェクトへのポインタ
	bool m_bDeath;                         // 死亡フラグ
	TYPE m_ntype;                          // 種類

};

#endif // !1