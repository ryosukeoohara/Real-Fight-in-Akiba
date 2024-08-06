//===========================================================
//
// •Ö—˜ŠÖ”[utility.cpp]
// Author ‘åŒ´—å«
//
//===========================================================
#include "utility.h"
#include "object2D.h"

namespace utility
{
	//===========================================================
	// ’Ç”ö
	//===========================================================
	float MoveToPosition(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos, float fMyRotY)
	{
		D3DXVECTOR3 fDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float fDiffmove = 0.0f, fDestmove = 0.0f;

		fDest = MyPos - TargetPos;

		fDestmove = atan2f(fDest.x, fDest.z);
		fDiffmove = fDestmove - fMyRotY;

		return fDiffmove;
	}

	//===========================================================
	// Šp“x•â³
	//===========================================================
	float CorrectAngle(float fAngle)
	{
		if (fAngle > D3DX_PI)
		{
			fAngle -= D3DX_PI * 2.0f;
		}
		else if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}

		return fAngle;
	}

	//===========================================================
	// ‹——£‘ª’è
	//===========================================================
	float Distance(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos)
	{
		float fLenght = 0.0f;

		fLenght = (MyPos.x + MyPos.z) - (TargetPos.x + TargetPos.z);

		if (fLenght < 0.0f)
		{
			fLenght *= -1.0f;
		}

		return fLenght;
	}

	//===========================================================
	// ƒ|ƒŠƒSƒ“‚ÌŠg‘å
	//===========================================================
	void Enlarge(CObject2D* pObj, float fHei, float fWid)
	{
		// •`‰æ‚³‚¹‚é
		pObj->SetDraw(true);

		// •‚Æ‚‚³Žæ“¾
		float fHeight = pObj->GetHeight();
		float fWidth = pObj->GetWidth();

		fHeight = fHeight + fHei;
		fWidth = fWidth + fWid;

		// ƒTƒCƒYÝ’è
		pObj->SetSize(fWidth, fHeight);
	}

	//===========================================================
	// ƒ|ƒŠƒSƒ“‚Ìk¬
	//===========================================================
	void Shrink(CObject2D* pObj, float fHei, float fWid)
	{
		// •`‰æ‚³‚¹‚é
		pObj->SetDraw(true);

		// •‚Æ‚‚³Žæ“¾
		float fHeight = pObj->GetHeight();
		float fWidth = pObj->GetWidth();

		fHeight = fHeight - fHei;
		fWidth = fWidth - fWid;

		// ƒTƒCƒYÝ’è
		pObj->SetSize(fWidth, fHeight);
	}

	//===========================================================
	// ƒ|ƒŠƒSƒ“‚ÌF
	//===========================================================
	void Color_A2D(CObject2D* pObj, float fValue)
	{
		// “§–¾“xŽæ“¾
		float fColor_a = pObj->GetColor().a;

		fColor_a = fColor_a + fValue;

		// FÝ’è
		pObj->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fColor_a));
	}
}