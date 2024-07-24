//===========================================================
//
// マップに配置するモデル[map.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAP_H_             //このマクロ定義がされてなかったら
#define _MAP_H_             //2重インクルード防止のマクロ定義をする

//===========================================================
// 前方宣言
//===========================================================
class CObjectX;

// マクロ定義
#define MAX_MODEL (512)  // モデルの最大数
#define MAX_NAME  (128)  // テクスチャの最大文字数

//===========================================================
// マップクラス定義
//===========================================================
class CMap
{
public:

	CMap();   // コンストラクタ
	~CMap();  // デストラクタ

	HRESULT Init(void);   // 初期化処理    
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理

	static CMap *Create(void); // 生成処理

	// 設定系

	// 取得系
	CObjectX **GetObjectX(void) { return m_appObjectX; }
	int GetNum(void) { return m_nNumModel; }

private:

	struct TEX
	{
		char aName[MAX_NAME];
	} ;
	

	void TextLoad(void);
	CObjectX **m_appObjectX;

	int m_nNumModel;
	int m_nNumItem;

	TEX m_aTex[MAX_MODEL];
};

#endif