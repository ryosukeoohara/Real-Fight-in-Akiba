//===========================================================
//
// テクスチャアニメーション[animation.h]
// Author 大原怜将
//
//===========================================================
#ifndef _ANIMATION_H__             // このマクロ定義がされてなかったら
#define _ANIMATION_H__             // 2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "billboard.h"

//===========================================================
// アニメーションクラス定義
//===========================================================
class CAnimation : public CBillBoard
{
public:

	enum TYPE
	{
		TYPE_NONE = 0,    // なんもない
		TYPE_HIT,         // 攻撃ヒット
		TYPE_MAX
	};

	CAnimation();  //コンストラクタ
	CAnimation(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, TYPE type);  //コンストラクタ
	~CAnimation();  //デストラクタ

	// 情報
	struct INFO
	{
		D3DXVECTOR3 pos;        // 位置
		D3DXVECTOR3 move;       // 移動量
		D3DXCOLOR col;          // 色
		TYPE type;              // 種類
		float fRadius;          // 半径(大きさ)
		int nLife;              // 寿命(表示時間)
	};

	HRESULT Init(void); // 初期化処理    
	void Uninit(void);  // 終了処理
	void Update(void);  // 更新処理
	void Draw(void);    // 描画処理

	static CAnimation *Create(D3DXVECTOR3 pos, float fRadius, CBillBoard::TYPE type);  //生成

	INFO m_Info;

private:

	
	int m_nIdxTexture;         // テクスチャのインデックス番号
	static const char *m_apTexName[TYPE_MAX];

};

#endif