//===========================================================
//
// リザルト画面[result.h]
// Author 大原怜将
//
//===========================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "manager.h"

//===========================================================
// 前方宣言
//===========================================================
class CField;
class CMap;
class CAppearanceUI;
class CUserRankUI;

//===========================================================
// リザルトクラス定義
//===========================================================
class CResult : public CScene
{
public:
	CResult();                     // コンストラクタ
	~CResult();                    // デストラクタ
						           
	HRESULT Init(void);            // 初期化処理
	void Uninit(void);             // 終了処理
	void Update(void);             // 更新処理
	void Draw(void);               // 描画処理

	static CResult *Create(void);  // 生成処理

private:
	CMap *m_pMap;
	CField *m_pField;
	CAppearanceUI *m_pNumber;
	CUserRankUI *m_pUserRank;

	int m_nCounter;
};

#endif