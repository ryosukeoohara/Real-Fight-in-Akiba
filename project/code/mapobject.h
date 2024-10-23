//===========================================================
//
// マップに配置するモデル[map.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MAPOBJECT_H_             //このマクロ定義がされてなかったら
#define _MAPOBJECT_H_             //2重インクルード防止のマクロ定義をする

#include "objectX.h"

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
class CMapObject : public CObjectX
{
public:

	CMapObject();   // コンストラクタ
	CMapObject(const char* aModelFliename, int nPriority = 3);
	~CMapObject();  // デストラクタ

	HRESULT Init(void);   // 初期化処理    
	void Uninit(void);    // 終了処理
	void Update(void);    // 更新処理
	void Draw(void);      // 描画処理

	static CMapObject* Create(void); // 生成処理

	// 設定系
	void SetNext(CMapObject* pNext) { m_pNext = pNext; }
	void SetPrev(CMapObject* pPrev) { m_pPrev = pPrev; }

	// 取得系
	int GetNum(void) { return m_nNumModel; }
	static CMapObject* GetInstance(void) { return m_pMap; }
	CMapObject* GetNext(void) { return m_pNext; }
	
private:

	static CMapObject* m_pMap;
	int m_nNumModel;
	int m_nNumItem;
	CMapObject* m_pNext;        // 次のオブジェクトへのポインタ
	CMapObject* m_pPrev;        // 前のオブジェクトへのポインタ
	
	bool m_bDeath;              // 死亡フラグ

	bool m_bShut = false;
	bool m_bDown = false;
};

#endif
