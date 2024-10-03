//=============================================================================
//
// 丸影処理 [shadow.h]
// Author : 大原　怜将
//
//=============================================================================
#ifndef _SHADOW_H_             //このマクロ定義がされてなかったら
#define _SHADOW_H_             //2重インクルード防止のマクロ定義をする

#include "object3D.h"

class CShadow : public CObject3D
{
public:
	CShadow();
	~CShadow();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:



};

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int type;
	bool bUse;
} SHADOW;

typedef enum
{
	SHADOWTYPE_PLAYER = 0,
	SHADOWTYPE_BULLET,
	SHADOWTYPE_MAX
}SHADOWTYPE;

//プロトタイプ宣言
void InitShadow(void);          //ポリゴンの初期化処理    
void UninitShadow(void);        //ポリゴンの終了処理
void UpdateShadow(void);        //ポリゴンの更新処理
void DrawShadow(void);          //ポリゴンの描画処理
SHADOW *GetShadow(void);

int SetShadow(void);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
//void SetPositionShadow1(int nIdxShadow1, D3DXVECTOR3 pos);

#endif
