//===========================================================
//
// •Ö—˜ŠÖ”[utility.h]
// Author ‘åŒ´—å«
//
//===========================================================
#ifndef _UTILITY_H_
#define _UTILITY_H_

//===========================================================
// ‘O•ûéŒ¾
//===========================================================
class CObject2D;

//===========================================================
// •Ö—˜ŠÖ”ŒQ
//===========================================================
namespace utility
{
	float MoveToPosition(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos, float fMyRotY);  // ’Ç”ö
	float CorrectAngle(float fAngle);                                               // Šp“x•â³
	float Distance(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos);                       // ‹——£
	void Enlarge(CObject2D* pObj, float fHei, float fWid);                          // ƒ|ƒŠƒSƒ“‚ğŠg‘å
	void Shrink(CObject2D* pObj, float fHei, float fWid);                           // ƒ|ƒŠƒSƒ“‚ğ¬‚³‚­
	void Color_A2D(CObject2D* pObj, float fValue);                                  // ƒ|ƒŠƒSƒ“‚ÌF
	//void 
}

#endif