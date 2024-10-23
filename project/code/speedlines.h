//===========================================================
//
// �W����[speedlines.h]
// Author �匴�叫
//
//===========================================================
#ifndef _SPEEDLINES_H_             //���̃}�N����`������ĂȂ�������
#define _SPEEDLINES_H_             //2�d�C���N���[�h�h�~�̃}�N����`������

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