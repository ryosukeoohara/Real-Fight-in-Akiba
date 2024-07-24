//===========================================================
//
// スコア処理[score.h]
// Author 大原怜将
//
//===========================================================
#ifndef _SCORE_H_             //このマクロ定義がされてなかったら
#define _SCORE_H_             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "number.h"

//===========================================================
// スコアクラス定義
//===========================================================
class CScore : public CNumber
{
public:
	CScore();                 // コンストラクタ
	CScore(D3DXVECTOR3 pos);  // コンストラクタ(オーバーロード)
	~CScore();                // デストラクタ

	HRESULT Init(void);      // 初期化処理    
	void Uninit(void);       // 終了処理
	void Update(void);       // 更新処理
	void Draw(void);         // 描画処理

	static CScore *Create(void);  // 生成処理

	// 設定系
	static void SetScore(int nScore) { m_nScore += nScore; }  // スコア

	// 取得系
	CScore *GetScore(void) { return m_Score; };               // スコアのポインタ
	static int Getscore(void) { return m_nScore; }            // スコア

	static void AddScore(int nScore);  // スコア加算

protected:
	static int m_nScore;

private:
	D3DXVECTOR3 m_pos;              // 位置
	static CScore *m_Score;         // スコアのポインタ
	static CNumber *m_apNumber[8];  // ナンバーのポインタ
};

#endif