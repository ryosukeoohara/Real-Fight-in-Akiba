//===========================================================
//
// ゲージ処理[gage.h]
// Author 大原怜将
//
//===========================================================
#ifndef _GAGE_H_             // このマクロ定義がされてなかったら
#define _GAGE_H_             // 2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "object2D.h"
#include "billboard.h"

//===========================================================
// ゲージ2Dクラス定義
//===========================================================
class CGage2D : public CObject2D
{
public:

	enum CGAGE2DTYPE
	{
		TYPE_NONE = 0, // なんもない
		TYPE_LIFE,     // 体力
		TYPE_STAMINA,  // スタミナ
		TYPE_MAX
	};

	CGage2D(int nPriority = 6);      // コンストラクタ
	~CGage2D();     // デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fHei, float fWid, CGAGE2DTYPE type);          // 初期化処理
	HRESULT Init(void);          // 初期化処理
	void Uninit(void);           // 終了処理
	void Update(void);           // 更新処理
	void Draw(void);             // 描画処理

	static CGage2D *Create(D3DXVECTOR3 pos, float fHei, float fWid, CGAGE2DTYPE type);      // 生成
	void SetSize(float fHeight, float fWidth);                            // サイズ

	CObject2D *GetObj2D(void) { return m_pLife; }

private:

	void SetCol(void);

	D3DXVECTOR3 m_pos;        // 位置
	D3DXVECTOR3 m_rot;        // 向き
	D3DXCOLOR m_col;          // 色
	float m_fHeight;          // 高さ
	float m_fWidth;           // 幅

	CGAGE2DTYPE m_Type;       // 種類
	CObject2D *m_pLife;   // オブジェクト2Dへのポインタ
	CObject2D *m_pGage;
};

//===========================================================
// ゲージ3Dクラス定義
//===========================================================
class CGage3D : public CBillBoard
{
public:

	enum CGAGE3DTYPE
	{
		TYPE_NONE = 0, // なんもない
		TYPE_STAMINA,  // スタミナ
		TYPE_LIFE,     // ライフ
		TYPE_MAX
	};

	CGage3D(int nPriority = 6);      // コンストラクタ
	~CGage3D();     // デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float fHei, float fWid, CGAGE3DTYPE type);          // 初期化処理
	HRESULT Init(void);          // 初期化処理
	void Uninit(void);           // 終了処理
	void Update(void);           // 更新処理
	void Draw(void);             // 描画処理

	static CGage3D *Create(D3DXVECTOR3 pos, float fHei, float fWid, CGAGE3DTYPE type);      // 生成
	void SetCurrent(D3DXMATRIX Current) { m_pCurrent = Current; }        // 親のマトリックス
	void SetPos(D3DXVECTOR3 *pPos) { m_pPos = pPos; }
	void SetUpHeight(float fUpHei) { m_UpHeight = fUpHei; }

	CBillBoard *GetBill(void) { return m_pLifeGage; }

private:

	void SetCol(void);
	void SetMixPos(void);

	D3DXVECTOR3 *m_pPos;      // 位置
	D3DXVECTOR3 m_rot;        // 向き
	D3DXCOLOR m_col;          // 色
	D3DXMATRIX m_mtxWorld;
	D3DXMATRIX m_pCurrent;
	float m_fHeight;          // 高さ
	float m_fWidth;           // 幅
	float m_UpHeight;

	CGAGE3DTYPE m_Type;          // 種類
	CBillBoard *m_pLifeGage;     // 体力ゲージ
	CBillBoard *m_pFrameGage;    // ゲージの枠
};

#endif