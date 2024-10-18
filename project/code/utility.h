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

//======================================
// イージング
//======================================
namespace easing
{
	inline float EaseInSine(float fTime) { return 1 - cosf((fTime * D3DX_PI) / 2); }
	inline float EaseOutSine(float fTime) { return sinf((fTime * D3DX_PI) / 2); }
	inline float EaseInOutSine(float fTime) { return -(cosf(D3DX_PI * fTime) - 1) / 2; }

	inline float EaseInCubic(float fTime) { return powf(fTime, 3.0f); };
	inline float EaseOutCubic(float fTime) { return 1 - powf(1.0f - fTime, 3.0f); };
	inline float EaseInOutCubic(float fTime) { return fTime < 0.5 ? 4 * fTime * fTime * fTime : 1 - powf(-2 * fTime + 2, 3) / 2; }

	inline float EaseInQuint(float fTime) { return fTime * fTime * fTime * fTime * fTime; }
	inline float EaseOutQuint(float fTime) { return 1 - powf(1 - fTime, 5); }
	inline float EaseInOutQuint(float fTime) { return fTime < 0.5 ? 16 * fTime * fTime * fTime * fTime * fTime : 1 - powf(-2 * fTime + 2, 5) / 2; }

	inline float EaseInQric(float fTime) { return 1 - sqrtf(1 - powf(fTime, 2)); }
	inline float EaseOutQric(float fTime) { return sqrtf(1 - powf(fTime - 1, 2)); }
	inline float EaseInOutQric(float fTime)
	{
		return fTime < 0.5
			? (1 - sqrtf(1 - powf(2 * fTime, 2))) / 2
			: (sqrtf(1 - powf(-2 * fTime + 2, 2)) + 1) / 2;
	}

	inline float EaseInElastic(float fTime)
	{
		const float c4 = (2 * D3DX_PI) / 3;

		return fTime == 0
			? 0
			: fTime == 1
			? 1
			: -powf(2, 10 * fTime - 10) * sinf((fTime * 10 - 10.75f) * c4);
	}
	inline float EaseOutElastic(float fTime)
	{
		const float c4 = (2 * D3DX_PI) / 3;

		return fTime == 0
			? 0
			: fTime == 1
			? 1
			: powf(2, -10 * fTime) * sinf((fTime * 10 - 0.75f) * c4) + 1;
	}
	inline float EaseInOutElastic(float fTime)
	{
		const float c5 = (2 * D3DX_PI) / 4.5f;

		return fTime == 0
			? 0
			: fTime == 1
			? 1
			: fTime < 0.5
			? -(powf(2, 20 * fTime - 10) * sinf((20 * fTime - 11.125f) * c5)) / 2
			: (powf(2, -20 * fTime + 10) * sinf((20 * fTime - 11.125f) * c5)) / 2 + 1;
	}

	inline float EaseInQuad(float fTime) { return fTime * fTime; }
	inline float EaseOutQuad(float fTime) { return 1 - (1 - fTime) * (1 - fTime); }
	inline float EaseInOutQuad(float fTime) { return fTime < 0.5 ? 2 * fTime * fTime : 1 - powf(-2 * fTime + 2, 2) / 2; }

	inline float EaseInQuart(float fTime) { return fTime * fTime * fTime * fTime; }
	inline float EaseOutQuart(float fTime) { return 1 - powf(1 - fTime, 4); }
	inline float EaseInOutQuart(float fTime) { return fTime < 0.5 ? 8 * fTime * fTime * fTime * fTime : 1 - powf(-2 * fTime + 2, 4) / 2; }

	inline float EaseInExpo(float fTime) { return fTime == 0 ? 0 : powf(2, 10 * fTime - 10); }
	inline float EaseOutExpo(float fTime) { return fTime == 1 ? 1 : 1 - powf(2, -10 * fTime); }
	inline float EaseInOutExpo(float fTime)
	{
		return fTime == 0
			? 0
			: fTime == 1
			? 1
			: fTime < 0.5 ? powf(2, 20 * fTime - 10) / 2
			: (2 - powf(2, -20 * fTime + 10)) / 2;
	}

	inline float EaseInBack(float fTime)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;

		return c3 * fTime * fTime * fTime - c1 * fTime * fTime;
	}
	inline float EaseOutBack(float fTime)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;

		return 1 + c3 * powf(fTime - 1, 3) + c1 * powf(fTime - 1, 2);
	}
	inline float EaseInOutBack(float fTime)
	{
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;

		return fTime < 0.5f
			? (powf(2 * fTime, 2) * ((c2 + 1) * 2 * fTime - c2)) / 2
			: (powf(2 * fTime - 2, 2) * ((c2 + 1) * (fTime * 2 - 2) + c2) + 2) / 2;
	}

	inline float EaseOutBounce(float fTime)
	{
		const float n1 = 7.5625f;
		const float d1 = 2.75f;

		if (fTime < 1 / d1)
		{
			return n1 * fTime * fTime;
		}
		else if (fTime < 2 / d1)
		{
			return n1 * (fTime -= 1.5f / d1) * fTime + 0.75f;
		}
		else if (fTime < 2.5 / d1)
		{
			return n1 * (fTime -= 2.25f / d1) * fTime + 0.9375f;
		}
		else
		{
			return n1 * (fTime -= 2.625f / d1) * fTime + 0.984375f;
		}
	}
	inline float EaseInBounce(float fTime) { return 1 - EaseOutBounce(1 - fTime); }
	inline float EaseInOutBounce(float fTime)
	{
		return fTime < 0.5f
			? (1 - EaseOutBounce(1 - 2 * fTime)) / 2
			: (1 + EaseOutBounce(2 * fTime - 1)) / 2;
	}
}

#endif