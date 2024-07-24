//===========================================================
//
// テクスチャ処理 [texture.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//マクロ定義
#define MAX_TEXTURE   (1024)   //テクスチャの最大数

//===========================================================
// テクスチャクラス定義
//===========================================================
class CTexture
{
public:
	CTexture();
	~CTexture();

	//テクスチャの種類
	typedef enum
	{
		TYPE_PLAYER = 0,  //プレイヤー
		TYPE_ENEMY,       //敵
		TYPE_BULLET,      //弾
		TYPE_MAX
	} TYPE;

	struct TEX
	{
		char aName[64] = "a";
		LPDIRECT3DTEXTURE9 m_pTexture;

	} ;

	HRESULT Load(void);
	void UnLoad(void);

	int Regist(const char *pFilename);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

	int GetNumTexture(void) { return m_nNumAll; };

private:
	
	TEX m_aTexture[MAX_TEXTURE];
	static int m_nNumAll;

	
};


#endif // !_TEXTURE_H_

