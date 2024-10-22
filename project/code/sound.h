//===========================================================
//
// サウンド処理 [sound.h]
// Author : 大原　怜将
//
//===========================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//===========================================================
// インクルードファイル
//===========================================================
#include "main.h"

//===========================================================
// サウンド情報の構造体定義
//===========================================================
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
} SOUNDINFO;

//===========================================================
// サウンドクラス定義
//===========================================================
class CSound
{
public:
	CSound();
	~CSound();

	// サウンド一覧
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,
		SOUND_LABEL_BGM_TUTORIAL,
		SOUND_LABEL_BGM_GAME,
		SOUND_LABEL_BGM_RESULT,
		SOUND_LABEL_SE_PUNCH,
		SOUND_LABEL_SE_MOVE,
		SOUND_LABEL_SE_IMPACT,
		SOUND_LABEL_SE_GRASS,
		SOUND_LABEL_SE_FIRE,
		SOUND_LABEL_SE_ATTACK,
		SOUND_LABEL_SE_CAN_SHOOT,
		SOUND_LABEL_SE_KO,
		SOUND_LABEL_SE_ULT_HIT,
		SOUND_LABEL_SE_IRON_ROT_HIT,
		SOUND_LABEL_SE_FANCE_DOWN,
		SOUND_LABEL_SE_ELECTRICBOX_DOWN,
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	// サウンドの情報
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		{ "data/SOUND/BGM/title.wav", -1 },
		{ "data/SOUND/BGM/tutorial.wav", -1 },
		{ "data/SOUND/BGM/game.wav", -1 },
		{ "data/SOUND/BGM/result.wav", -1 },
		{ "data/SOUND/SE/punch.wav", 0 },
		{ "data/SOUND/SE/asioto.wav", 0 },
		{ "data/SOUND/SE/impact.wav", 0 },
		{ "data/SOUND/SE/grass.wav", 0 },
		{ "data/SOUND/SE/fire1.wav", 0 },
		{ "data/SOUND/SE/attack.wav", 0 },
		{ "data/SOUND/SE/can_Shoot.wav", 0 },
		{ "data/SOUND/SE/K.O.wav", 0 },
		{ "data/SOUND/SE/ultHit.wav", 0 },
		{ "data/SOUND/SE/ironrot_hit.wav", 0 },
		{ "data/SOUND/SE/fance_down.wav", 0 },
		{ "data/SOUND/SE/electricbox_down.wav", 0 },
	}; 

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ
};

#endif