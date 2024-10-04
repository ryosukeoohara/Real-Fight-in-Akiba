//===========================================================
//
// モデル処理[objectX.h]
// Author 大原怜将
//
//===========================================================
#ifndef _OBJECTX_H__             //このマクロ定義がされてなかったら
#define _OBJECTX_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "object.h"

//===========================================================
// 前方宣言
//===========================================================
class CModel;

//===========================================================
//　オブジェクトX(Xファイル)クラス定義
//===========================================================
class CObjectX : public CObject
{
private:
	struct INFO
	{
		D3DXVECTOR3 pos;              // 位置
		D3DXVECTOR3 rot;              // 向き
		D3DXVECTOR3 move;             // 移動
		D3DXCOLOR col;                // 色
		D3DXVECTOR3 vtxMini;          // 最小値
		D3DXVECTOR3 vtxMax;           // 最大値
		D3DXMATRIX mtxWorld;          // ワールドマトリックス
		int nIdxModel;	              // モデルのインデックス番号
		const char *Fliename;         // モデルの名前
	};

	INFO m_Info;                // 情報

public:
	CObjectX();
	CObjectX(const char *aModelFliename, int nPriority = 3);
	~CObjectX();

	HRESULT Init(void);           // 初期化処理    
	void Uninit(void);            // 終了処理
	void Update(void);            // 更新処理
	void Draw(void);              // 描画処理
						          
	void ReadText(void);          //テキストファイル読み込み

	static CObjectX *Create(const char *aModelFliename, int nPriority = 3);  //生成

	void BindModel(INFO pObjX);

	void SetVtx(void);

	// 設定系
	void SetPosition(D3DXVECTOR3 pos) { m_Info.pos = pos; }                     // 位置
	void SetPositionOri(D3DXVECTOR3 posOrigin) { m_posOrigin = posOrigin; }     // 原点の位置
	void SetRotition(D3DXVECTOR3 rot) { m_Info.rot = rot; }                     // 向き
	void SetRotOrigin(D3DXVECTOR3 rotOrigin) { m_rotOrigin = rotOrigin; }       // 向き
	void SetColor(D3DXCOLOR col) { m_Info.col = col; }                          // 色
	void SetIdxModel(int nIdx) { m_Info.nIdxModel = nIdx; }                     // モデルのインデックス番号
	void SetCurrent(D3DXMATRIX *Current) { m_pCurrent = Current; }              // 親のマトリックス
	void SetbEnable(bool bValue = true) { m_bEnable = bValue; }
	void SetbDraw(bool bValue) { m_bDraw = bValue; }
	void SetbShut(bool bValue) { m_bShut = bValue; }
	void SetbDown(bool bValue) { m_bDown = bValue; }
	void SetbHitAttack(bool bValue) { m_bHitAttack = bValue; }
	void SetbFall(bool bValue) { m_bFall = bValue; }
	void SetFallCounter(int n) { m_nFallDownCounter = n; }

	//　取得系
	D3DXVECTOR3 GetPosition(void) { return  m_Info.pos; }        // 位置
	D3DXVECTOR3 GetPositionOri(void) { return m_posOrigin; }     // 原点の位置  
	D3DXVECTOR3 GetRotition(void) { return  m_Info.rot; }        // 向き
	D3DXVECTOR3 GetRotOrigin(void) { return m_rotOrigin; }       // 原点の向き
	D3DXCOLOR GetColor(void) { return  m_Info.col; }             // 色
	D3DXMATRIX *GetMtxWorld(void) { return &m_Info.mtxWorld; }   // マトリックス取得
	D3DXVECTOR3 GetVtxMin(void) { return m_Info.vtxMini; }       // モデルの最小値
	D3DXVECTOR3 GetVtxMax(void) { return m_Info.vtxMax; }        // モデルの最大値
	int GetIdxModel(void) { return m_Info.nIdxModel; }           // モデルのインデックス番号
	const char *GetFileName(void) { return m_Info.Fliename; }    // モデルのファイルネーム
	bool IsEnable(void) { return m_bEnable; }
	int GetFallCounter(void) { return m_nFallDownCounter; }
	bool GetFallDown(void) { return m_bFallDown; }

private:
	
	void Shoot(void);
	void FallDown(void);
	void HitAttack(void);
	void GraduallyFallDown(void);

	D3DXVECTOR3 m_posOrigin;
	D3DXVECTOR3 m_rotOrigin;
	LPD3DXMESH m_pMesh;       //テクスチャへのポインタ
	LPD3DXBUFFER m_pBuffMat;  //頂点バッファへのポインタ
	DWORD m_dwNumMat;         //マテリアルの数
	LPDIRECT3DTEXTURE9 *m_pTexture;     //テクスチャへのポインタ
	D3DXMATRIX *m_pCurrent;                     // 親のマトリックス
	D3DMATERIAL9 m_ShadowMat;
	float m_fFallDownSpeed = 0.0f;
	int m_nShakeTimeCounter = 0;
	int m_nShake = 0;
	int m_nFallDownCounter = 0;
	float ShakeAngle = 0.0f;
	float m_fAngle = 1.0f;

	int *m_nIdxTexture;
	bool m_bEnable;
	bool m_bDraw;
	bool m_bShut = false; 
	bool m_bDown = false;
	bool m_bHitAttack = false;
	bool m_bFall = false;
	bool m_bFallDown = false;
};

#endif