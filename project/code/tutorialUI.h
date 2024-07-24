//===========================================================
//
// チュートリアルのUI[tutorialUI.h]
// Author 大原怜将
//
//===========================================================
#ifndef _TUTORIALUI_H_
#define _TUTORIALUI_H_

//===========================================================
// 前方宣言
//===========================================================
class CObject2D;

//===========================================================
// チュートリアルUIクラス定義
//===========================================================
class CTutorialUI
{
public:

	//種類
	enum TYPE
	{
		TYPE_PRACTICE = 0,  // 見出し
		TYPE_MOVE,          // 移動
		TYPE_CAMERA,        // ザコ
		TYPE_ATTACK,        // ボス
		TYPE_AVOID,         // 回避
		TYPE_GRAP,          // 掴み
		TYPE_HEAT,          // 激アツ
		TYPE_MAX
	};

public:

	CTutorialUI();                    // コンストラクタ
	~CTutorialUI();                   // デストラクタ
									  
	HRESULT Init(void);               // 初期化処理
	void Uninit(void);                // 終了処理
	void Update(void);                // 更新処理
	void Draw(void);                  // 描画処理

	static CTutorialUI *Create(void); // 生成処理

private:

	CObject2D *m_apObject2D[TYPE_MAX];
	
};

#endif