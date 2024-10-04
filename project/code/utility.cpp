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

		if(fColor_a <= 1.0f && fColor_a >= 0.0f)
		   fColor_a = fColor_a + fValue;

		// FÝ’è
		pObj->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fColor_a));
	}

	//===========================================================
    // 90“x‰ñ“]‰ñ“]‚µ‚½Û‚Ìvtx‚Ì•ÏX
    //===========================================================
	void ChangeVtx(D3DXVECTOR3* pVtxMax, D3DXVECTOR3* pVtxMin, D3DXVECTOR3 rot)
	{
		float MaxX = 0.0f, MaxY = 0.0f, MaxZ = 0.0f,   MinX = 0.0f, MinY = 0.0f, MinZ = 0.0f;

		if (rot.x >= 1.57)
		{
			MaxY = pVtxMax->y;
			MinY = pVtxMin->y;

			pVtxMax->y = pVtxMax->x;
			pVtxMin->y = pVtxMin->x;

			pVtxMax->x = MaxY;
			pVtxMin->x = MinY;

			return;
		}

		if (rot.x <= -1.57)
		{
			MaxY = pVtxMax->y;
			MinY = pVtxMin->y;

			pVtxMax->y = pVtxMax->x;
			pVtxMin->y = pVtxMin->x;
			
			pVtxMax->x = -MinY;
			pVtxMin->x = -MaxY;

			return;
		}

		if (rot.y >= 1.57 || rot.y <= -1.57)
		{
			MaxX = pVtxMax->x;
			MaxZ = pVtxMax->z;
			MinX = pVtxMin->x;
			MinZ = pVtxMin->z;

			pVtxMax->x = pVtxMax->z;
			pVtxMin->x = pVtxMin->z;

			pVtxMax->z = MaxX;
			pVtxMin->z = MinX;

			return;
		}
		
		if (rot.z >= 1.57 || rot.z <= -1.57)
		{
			int n = 0;
		}
	}
}