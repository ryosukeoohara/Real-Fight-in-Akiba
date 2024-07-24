//===========================================================
//
// 便利関数[utility.cpp]
// Author 大原怜将
//
//===========================================================
#include "utility.h"
#include "object2D.h"

namespace utility
{
	//===========================================================
	// 追尾
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
	// 角度補正
	//===========================================================
	float CorrectAngle(float fAngle)
	{
		if (fAngle > D3DX_PI)
		{
			fAngle = -D3DX_PI;
		}
		else if (fAngle < -D3DX_PI)
		{
			fAngle = D3DX_PI;
		}

		return fAngle;
	}

	//===========================================================
	// 距離測定
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
	// ポリゴンの拡大
	//===========================================================
	void Enlarge(CObject2D* pObj, float fHei, float fWid)
	{
		// 描画させる
		pObj->SetDraw(true);

		// 幅と高さ取得
		float fHeight = pObj->GetHeight();
		float fWidth = pObj->GetWidth();

		fHeight = fHeight + fHei;
		fWidth = fWidth + fWid;

		// サイズ設定
		pObj->SetSize(fWidth, fHeight);
	}

	//===========================================================
	// ポリゴンの縮小
	//===========================================================
	void Shrink(CObject2D* pObj, float fHei, float fWid)
	{
		// 描画させる
		pObj->SetDraw(true);

		// 幅と高さ取得
		float fHeight = pObj->GetHeight();
		float fWidth = pObj->GetWidth();

		fHeight = fHeight - fHei;
		fWidth = fWidth - fWid;

		// サイズ設定
		pObj->SetSize(fWidth, fHeight);
	}

	//===========================================================
	// ポリゴンの色
	//===========================================================
	void Color_A2D(CObject2D* pObj, float fValue)
	{
		// 透明度取得
		float fColor_a = pObj->GetColor().a;

		fColor_a = fColor_a + fValue;

		// 色設定
		pObj->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fColor_a));
	}
}