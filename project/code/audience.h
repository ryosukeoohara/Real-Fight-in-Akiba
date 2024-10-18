//===========================================================
//
// 周りで見ている人[audience.h]
// Author 大原怜将
//
//===========================================================
#ifndef _AUDIENCE_H_      //このマクロ定義がされてなかったら
#define _AUDIENCE_H_      //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "main.h"
#include "objectX.h"

//===========================================================
// オーディエンスクラス定義
//===========================================================
class CAudience : public CObjectX
{
public:

	CAudience();  //コンストラクタ
	CAudience(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filename);
	~CAudience();  //デストラクタ

	HRESULT Init(void);   // 初期化処理    
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理
	void Draw(void);      // 描画処理

	static CAudience *Create(void);
	static CAudience* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filename);

	// 設定系

	// 取得系
	static CAudience* GetTop(void) { return m_pTop; }
	CAudience* GetNext(void) { return m_pNext; }

private:

	static CAudience* m_pTop;  //先頭のオブジェクトへのポインタ
	static CAudience* m_pCur;  //最後尾のオブジェクトへのポインタ
	CAudience* m_pNext;        //次のオブジェクトへのポインタ
	CAudience* m_pPrev;        //前のオブジェクトへのポインタ

	D3DXVECTOR3 m_pos;         // 位置
	D3DXVECTOR3 m_move;        // 移動量
	int m_nJumpWaitTime;       // 再びジャンプ可能になるまでの時間
	int m_nThrowWaitTime;      // 再び缶を投げれるようになるまでの時間
	bool m_bJump;              // ジャンプしているかどうか
	bool m_bThrow;             // 缶を投げたかどうか
};

#endif