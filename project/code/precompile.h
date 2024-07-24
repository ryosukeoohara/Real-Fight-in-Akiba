//===========================================================
//
// プリコンパイル[precompile.h]
// Author 大原怜将
//
//===========================================================
#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

#define DIRECTINPUT_VERSION (0x0800)               //ビルト時の警告対処用マクロ

//===========================================================
// インクルードファイル
//===========================================================
#include <Windows.h>
#include <Xinput.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "d3dx9.h"
#include "dinput.h"  //入力処理に必要
#include "xaudio2.h" //サウンド処理に必要   

//===========================================================
// ライブラリリンク
//===========================================================
#pragma comment(lib,"d3d9.lib")                    // 描画処理に必要
#pragma comment(lib,"d3dx9.lib")                   // [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")                  // Directコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")                   // システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")                 // 入力処理に必要

#endif