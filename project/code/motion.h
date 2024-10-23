//===========================================================
//
// モーション処理[motion.h]
// Author 大原怜将
//
//===========================================================
#ifndef _MOTION_H_
#define _MOTION_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "objectX.h"

//===========================================================
// 定数定義
//===========================================================
namespace MOTION_INFO
{
	const int MAX_INFO = 64;  // モーションの最大数
}

//===========================================================
// 前方宣言
//===========================================================
class CCharacter;

//===========================================================
// モーションクラス定義
//===========================================================
class CMotion
{
private:

	//モデル---------------------------------------------------------------
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;

	} KEY;

	//モデル---------------------------------------------------------------
	typedef struct
	{
		KEY aKey[MOTION_INFO::MAX_INFO];  //現在のキー
		int nFrame;   //モーションのフレーム数

	} KEYINFO;

	//モーション---------------------------------------------------------------
	typedef struct
	{
		int nNumKey;    // キーの総数
		int nLoop;      // ループするかどうか
		int nNumFrame;  // フレームの総数
		int nAttackOccurs;  // 攻撃判定発生フレーム
		int nAttackEnd;  // 攻撃判定終了フレーム
		int nAttackDAMAGE;  // 攻撃力
		int nInvincibleStart;  // 完全無敵状態開始
		int nInvincibleEnd;    // 完全無敵終了
		int nCancellFrame;   // キャンセル可能フレーム
		int nCancellFrameEnd;  // キャンセル可能フレーム
		float nKnockBack;      // 吹っ飛び
		bool bLoop;     // ループするかどうか
		bool bFinish;   // 終了したかどうか

		KEYINFO KeySet[MOTION_INFO::MAX_INFO];

	} INFO;

public:
	CMotion();
	~CMotion();

	void Init(void);
	void Uninit(void);
	void Update(void);

	bool IsFinish(void);
	
	// 読み込み系
	void ReadText(const char *TextFilename);            // 外部ファイル読み込み
	
	// 設定系
	void Set(int nType, bool value = true);            // モーション
	void SetInfoBlendON(void);                                 // 情報
	void SetInfoBlendOFF(KEY key);
	void SetModel(CCharacter **ppModel, int nNumModel); // キャラクターのモデル
	void InitPose(int type);

	// 取得系
	int GetType(void);                                  // 種類
	int GetNumFrame(void) { return m_nNowFrame; }       // モーションの総フレーム
	int GetNowFrame(void);                              // 現在のフレーム
	int GetAttackOccurs(void);                          // 攻撃判定が発生するフレーム
	int GetAttackEnd(void);                             // 攻撃判定が終了するフレーム
	int GetAttackDamage(void);                          // その攻撃のダメージ量
	int GetInvincibleStrat(void);                       // 無敵判定が発生するフレーム
	int GetInvincibleEnd(void);                         // 無敵判定が終了するフレーム
	int GetCancellFrame(void);                          // キャンセル可能判定が発生するフレーム
	int GetCancellFrameEnd(void);                       // キャンセル可能判定が終了するフレーム
	int GetKeyFrame(void) { return m_nKeyFrame; }       // モーションのキー数
	float GetKnockBack(void);                           // 吹き飛ぶ値
	INFO GetInfo(void);

private:

	void RotCorrect(float rot);
	void BlendON(void);
	void BlendOFF(void);

	INFO m_aInfo[MOTION_INFO::MAX_INFO];
	KEY m_aOldInfo[MOTION_INFO::MAX_INFO];
	int  m_nNumAll;              //モーションの総数
	int  m_nType;                //モーションの種類
	int m_nTypeold;              //前回のモーションの種類
	bool m_bLoop;                //ループするかどうか
	int  m_nNumKey;              //キーの総数
	int  m_nKey;                 //現在のキー
	int  m_nCounter;             //フレーム数に達したか
	int  m_nCntkeySet;
	int m_nNumFrame;             //モーションのフレーム数
	int m_nNowFrame;
	int m_nKeyFrame = 0;             // 現在のキーの現在のフレーム数
	bool m_bFinish;              //終了したかどうか
	bool m_bBlend;               // モーションブレンドするかどうか
	
	CCharacter **m_ppModel;  //モデルへのダブルポインタ
	int m_nNumModel;         //モデルの総数
	static int m_nObj;       //誰のモーションか
};

#endif