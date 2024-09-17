//===========================================================
//
// アイテムの管理 [itemmanager.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _ITEMMANAGER_H_
#define _ITEMMANAGER_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "item.h"

//===========================================================
// アイテムの管理クラス定義
//===========================================================
class CItemManager
{
public:
	CItemManager();              // コンストラクタ
	~CItemManager();             // デストラクタ

	HRESULT Init(void);          // 初期化処理
	void Uninit(void);           // 終了処理
	void Update(void);           // 更新処理

	static CItemManager *Create(const char* filename);   // 生成処理
	void Release(int idx);                               // 指定した番号のアイテムを削除する
	void ReadText(const char *text);                     // テキストファイル読み込み

	 // 設定系
	void SetNum(int num) { m_nNum = num; }               // 総数

	// 取得系
	int GetNum(void) { return m_nNum; }                  // 総数
	static CItemManager* GetInstance(void) { return m_pItemManager; }

private:
	
	int m_nNum;  // 総数

	static const char *m_apTexName[CItem::TYPE_MAX];  // モデルのファイルネーム
	static CItemManager* m_pItemManager;
};

#endif