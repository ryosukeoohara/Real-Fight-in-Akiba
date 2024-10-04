//===========================================================
//
// 便利関数[utility.h]
// Author 大原怜将
//
//===========================================================
#ifndef _UTILITY_H_
#define _UTILITY_H_

//===========================================================
// 前方宣言
//===========================================================
class CObject2D;

//===========================================================
// 便利関数群
//===========================================================
namespace utility
{
	float MoveToPosition(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos, float fMyRotY);  // 追尾
	float CorrectAngle(float fAngle);                                               // 角度補正
	float Distance(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos);                       // 距離
	void Enlarge(CObject2D* pObj, float fHei, float fWid);                          // ポリゴンを拡大
	void Shrink(CObject2D* pObj, float fHei, float fWid);                           // ポリゴンを小さく
	void Color_A2D(CObject2D* pObj, float fValue);                                  // ポリゴンの色
	void ChangeVtx(D3DXVECTOR3* pVtxMax, D3DXVECTOR3* pVtxMin, D3DXVECTOR3 rot);          // 90度回転回転した際のvtxの変更
}

#endif