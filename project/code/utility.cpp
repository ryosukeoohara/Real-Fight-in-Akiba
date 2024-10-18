//===========================================================
//
// 便利関数[utility.cpp]
// Author 大原怜将
//
//===========================================================
#include "utility.h"
#include "object2D.h"
#include <iostream>
#include <cmath>

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
			fAngle -= D3DX_PI * 2.0f;
		}
		else if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}

		return fAngle;
	}

	//===========================================================
	// 距離測定
	//===========================================================
	float Distance(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos)
	{
		float fLenght = 0.0f;

		//fLenght = (MyPos.x + MyPos.z) - (TargetPos.x + TargetPos.z);

		// 点1から点2へのベクトルを計算
		D3DXVECTOR3 diff = TargetPos - MyPos;

		// ベクトルの長さ（距離）を計算
		return D3DXVec3Length(&diff);
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

		if(fColor_a <= 1.0f && fColor_a >= 0.0f)
		   fColor_a = fColor_a + fValue;

		// 色設定
		pObj->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fColor_a));
	}

	//===========================================================
    // 90度回転回転した際のvtxの変更
    //===========================================================
	void ChangeVtx(D3DXVECTOR3* pVtxMax, D3DXVECTOR3* pVtxMin, D3DXVECTOR3 rot)
	{
		float MaxX = 0.0f, MaxY = 0.0f, MaxZ = 0.0f,   MinX = 0.0f, MinY = 0.0f, MinZ = 0.0f;

		if (rot.x >= 1.57f)
		{
			MaxY = pVtxMax->y;
			MinY = pVtxMin->y;

			pVtxMax->y = pVtxMax->x;
			pVtxMin->y = pVtxMin->x;

			pVtxMax->x = MaxY;
			pVtxMin->x = MinY;

			return;
		}

		if (rot.x <= -1.57f)
		{
			MaxY = pVtxMax->y;
			MinY = pVtxMin->y;

			pVtxMax->y = -pVtxMin->x;
			pVtxMin->y = -pVtxMax->x;
			
			pVtxMax->x = -MinY;
			pVtxMin->x = -MaxY;

			return;
		}

		if (rot.y >= 1.57f || rot.y <= -1.57f)
		{
			MaxX = pVtxMax->x;
			MinX = pVtxMin->x;

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