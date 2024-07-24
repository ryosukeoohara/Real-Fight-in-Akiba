//===========================================================
//
// 周りで見ている人の管理[audiencemanager.h]
// Author 大原怜将
//
//===========================================================
#ifndef _AUDIENCEMANAGER_H_      //このマクロ定義がされてなかったら
#define _AUDIENCEMANAGER_H_      //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "main.h"
#include "objectX.h"

//===========================================================
// クラス定義
//===========================================================
class CAudienceManager
{
public:

	// 種類
	enum TYPE
	{
		TYPE_01 = 0,
		TYPE_02,
		TYPE_03,
		TYPE_04,
		TYPE_MAX
	};

	CAudienceManager();   // コンストラクタ
	~CAudienceManager();  // デストラクタ

	HRESULT Init(void);   // 初期化処理    
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理
	void Draw(void);      // 描画処理

	static CAudienceManager* Create(void);  // 生成処理

private:

};

#endif