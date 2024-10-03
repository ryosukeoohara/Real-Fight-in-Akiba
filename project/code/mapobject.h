//===========================================================
//
// マップに配置するモデル[map.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAPOBJECT_H_             //このマクロ定義がされてなかったら
#define _MAPOBJECT_H_             //2重インクルード防止のマクロ定義をする

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
class CMapObject
{
public:

	/*enum TYPE
	{
		TYPE_KAN,
		TYPE_BOX,
		TYPE_KAN,
		TYPE_MAX
	};*/

	CMapObject();   // コンストラクタ
	~CMapObject();  // デストラクタ

	HRESULT Init(void);   // 初期化処理    
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理

	static CMapObject* Create(void); // 生成処理

	// 設定系
	//void SetType(TYPE n) { m_type = n; }

	// 取得系
	CObjectX** GetObjectX(void) { return m_appObjectX; }
	int GetNum(void) { return m_nNumModel; }
	static CMapObject* GetInstance(void) { return m_pMap; }
	
	void SetbShut(bool bValue) { m_bShut = bValue; }

private:

	struct TEX
	{
		char aName[MAX_NAME];
	};

	//TYPE m_type;
	void TextLoad(void);
	void Shoot(void);
	void FallDown(void);

	static CMapObject* m_pMap;
	CObjectX** m_appObjectX;
	int m_nNumModel;
	int m_nNumItem;

	TEX m_aTex[MAX_MODEL];

	bool m_bShut = false;
	bool m_bDown = false;
};

#endif
