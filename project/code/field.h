//===========================================================
//
// フィールド[field.h]
// Author 大原怜将
//
//===========================================================
#ifndef _FIELD_H_
#define _FIELD_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "objectMesh.h"
#include "object3D.h"

//===========================================================
// フィールドクラス定義
//===========================================================
class CField : public CObject3D
{
public:
	CField();
	~CField();

	HRESULT Init(void);   //　初期化処理    
	void Uninit(void);    //　終了処理
	void Update(void);    //　更新処理
	void Draw(void);      //　描画処理

	static CField *Create(void);  //　生成

private:
	int m_nIdxTexture;  //　テクスチャのインデックス番号

};

#endif // !_FIELD_H_