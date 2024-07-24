//===========================================================
//
//　キャラクター[character.h]
//　Author 大原怜将
//
//===========================================================
#ifndef _CHARACTER_H__             //このマクロ定義がされてなかったら
#define _CHARACTER_H__             //2重インクルード防止のマクロ定義をする

//===========================================================
// インクルードファイル
//===========================================================
#include "main.h"

//===========================================================
// 
//===========================================================
class CCharacter
{
public:
	CCharacter(void);
	CCharacter(const char *aModelFliename);
	~CCharacter();

	HRESULT Init(void);     //モデルの初期化処理    
	void Uninit(void);   //モデルの終了処理
	void Update(void);   //モデルの更新処理
	void Draw(void);     //モデルの描画処理

	void BindTexture(LPDIRECT3DTEXTURE9 *m_Texture);  //テクスチャをバインド
	static CCharacter *Create(const char *aModelFliename);  //生成
	void SetParent(CCharacter *pModel) { m_pParent = pModel; }  //親の設定

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }  //位置設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }     //位置取得   
	void SetPositionOri(D3DXVECTOR3 posOrigin) { m_posOrigin = posOrigin; }  //原点の位置設定
	D3DXVECTOR3 GetPositionOri(void) { return m_posOrigin; }     //原点の位置取得  

	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }       //向き設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }          //向き取得   
	void SetRotOrigin(D3DXVECTOR3 rotOrigin) { m_rotOrigin = rotOrigin; }       //向き設定
	D3DXVECTOR3 GetRotOrigin(void) { return m_rotOrigin; }          //向き取得   

	D3DXMATRIX *GetMtxWorld(void) { return &m_mtxWorld; }     //マトリックス取得
	CCharacter *GetParent(void) { return m_pParent; }       //親モデルのポインタ取得

	D3DXVECTOR3 GetVtxMin(void) { return m_vtxMini; }  //モデルの最小値取得
	D3DXVECTOR3 GetVtxMax(void) { return m_vtxMax; }   //モデルの最大値取得

	void SetCurrent(D3DXMATRIX *Current) { m_pCurrent = Current; }  // 親のマトリックス

private:
	LPDIRECT3DTEXTURE9 *m_pTexture;     //テクスチャへのポインタ
	D3DMATERIAL9 m_ShadowMat;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posOrigin;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_rotOrigin;
	D3DXVECTOR3 m_vtxMini;    //モデルの最小値
	D3DXVECTOR3 m_vtxMax;     //モデルの最大値
	LPD3DXMESH m_pMesh;       //テクスチャへのポインタ
	LPD3DXBUFFER m_pBuffMat;  //頂点バッファへのポインタ
	DWORD m_dwNumMat;         //マテリアルの数
	D3DXMATRIX m_mtxWorld;    //ワールドマトリックス
	D3DXMATRIX *m_pCurrent;
	int m_nNumModel;          //モデル(パーツ)の総数
	const char *m_Fliename;   //モデルのXファイルの名前

	CCharacter *m_pParent;    //親モデルへのポインタ

	int *m_nIdxTexture;
};

#endif