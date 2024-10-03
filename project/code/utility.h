//===========================================================
//
// �֗��֐�[utility.h]
// Author �匴�叫
//
//===========================================================
#ifndef _UTILITY_H_
#define _UTILITY_H_

//===========================================================
// �O���錾
//===========================================================
class CObject2D;

//===========================================================
// �֗��֐��Q
//===========================================================
namespace utility
{
	float MoveToPosition(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos, float fMyRotY);  // �ǔ�
	float CorrectAngle(float fAngle);                                               // �p�x�␳
	float Distance(D3DXVECTOR3 MyPos, D3DXVECTOR3 TargetPos);                       // ����
	void Enlarge(CObject2D* pObj, float fHei, float fWid);                          // �|���S�����g��
	void Shrink(CObject2D* pObj, float fHei, float fWid);                           // �|���S����������
	void Color_A2D(CObject2D* pObj, float fValue);                                  // �|���S���̐F
	//void 
}

#endif