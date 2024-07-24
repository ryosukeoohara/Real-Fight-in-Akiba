//===========================================================
//
// エフェクト2D[effect2D.h]
// Author 大原怜将
//
//===========================================================
#ifndef _EFFECT2D_H__             //このマクロ定義がされてなかったら
#define _EFFECT2D_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "object2D.h"

//===========================================================
// エフェクト2Dクラス定義
//===========================================================
class CEffect2D : public CObject2D
{
public:

	enum TYPE
	{
		TYPE_GROUND = 0,   // 土煙
		TYPE_SMOOK,        // 煙
		TYPE_MAX
	};

	CEffect2D();  //コンストラクタ
	CEffect2D(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, TYPE type);  //コンストラクタ
	~CEffect2D();  //デストラクタ

				 // 情報
	struct INFO
	{
		D3DXVECTOR3 pos;        // 位置
		D3DXVECTOR3 move;       // 移動量
		D3DXCOLOR col;          // 色
		float fRadius;          // 半径(大きさ)
		int nLife;              // 寿命(表示時間)
	};

	HRESULT Init(void); // 初期化処理    
	void Uninit(void);  // 終了処理
	void Update(void);  // 更新処理
	void Draw(void);    // 描画処理

	void Ground(void);
	void Blood(void);
	void Smook(void);
	void Circle(void);

	static CEffect2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, TYPE type);  //生成

	INFO m_Info;

private:

	TYPE m_type;               // 種類
	int m_nIdxTexture;         // テクスチャのインデックス番号
	static const char *m_apTexName[TYPE_MAX];

};

#endif