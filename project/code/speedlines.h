//===========================================================
//
// 集中線[speedlines.h]
// Author 大原怜将
//
//===========================================================
#ifndef _SPEEDLINES_H_             //このマクロ定義がされてなかったら
#define _SPEEDLINES_H_             //2重インクルード防止のマクロ定義をする

#include "object2D.h"

class CSpeedLines : public CObject2D
{
public:
	CSpeedLines();
	~CSpeedLines();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CSpeedLines* Create(void);

private:

	int m_nChangeCount;
	int m_nTexCount;

};


#endif