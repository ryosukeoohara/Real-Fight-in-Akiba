//===========================================================
//
// ÖÖ[utility.h]
// Author åŽå«
//
//===========================================================
#ifndef _UTILITY_H_
#define _UTILITY_H_

//===========================================================
// OûéŸ
//===========================================================
class CObject2D;

//===========================================================
// ÖÖQ
//===========================================================
namespace utility
{
	float MoveToPosition(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos, float fMyRotY);  // Çö
	float CorrectAngle(float fAngle);                                               // pxâ³
	float Distance(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos);                       // £
	void Enlarge(CObject2D* pObj, float fHei, float fWid);                          // |Sðgå
	void Shrink(CObject2D* pObj, float fHei, float fWid);                           // |Sð¬³­
	void Color_A2D(CObject2D* pObj, float fValue);                                  // |SÌF
	void ChangeVtx(D3DXVECTOR3* pVtxMax, D3DXVECTOR3* pVtxMin, D3DXVECTOR3 rot);          // 90xñ]ñ]µœÛÌvtxÌÏX
}

#endif