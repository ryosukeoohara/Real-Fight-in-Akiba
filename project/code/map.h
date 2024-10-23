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
class CMapObject;

//===========================================================
// 定数定義
//===========================================================
namespace MODEL_FILE
{
	const int MAX_MODEL = 512;  // モデルの最大数
	const int MAX_NAME = 124;   // ファイルパスの最大文字数
}

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
	void Draw(void);

	static CMap *Create(void); // 生成処理
	

	// 設定系

	// 取得系
	CObjectX **GetObjectX(void) { return m_appObjectX; }
	int GetNum(void) { return m_nNumModel; }
	static CMap *GetInstance(void) { return m_pMap; }
	static CMapObject* GetTop(void) { return m_pTop; }  // リストの先頭
	static CObjectX* GetTopObjX(void) { return m_pObjXTop; }

private:

	struct TEX
	{
		char aName[MODEL_FILE::MAX_NAME];
	} ;
	

	void TextLoad(void);
	void MapObjectLoad(void);

	static CMap* m_pMap;

	static CMapObject* m_pTop;  // 先頭のオブジェクトへのポインタ
	static CMapObject* m_pCur;  // 最後尾のオブジェクトへのポインタ
	static CObjectX* m_pObjXTop;  // 先頭のオブジェクトへのポインタ
	static CObjectX* m_pObjXCur;  // 最後尾のオブジェクトへのポインタ

	CObjectX **m_appObjectX;
	int m_nNumModel;
	int m_nNumItem;

	TEX m_aTex[MODEL_FILE::MAX_MODEL];
};



#endif