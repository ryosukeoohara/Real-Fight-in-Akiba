//===========================================================
//
// �}�b�v�ɔz�u��Ă���O������[mapobject_light.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAPOBJECT_LIGHT_H__             //���̃}�N����`������ĂȂ�������
#define _MAPOBJECT_LIGHT_H__             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// �O���錾
class CLightState;

//===========================================================
// �O���N���X��`
//===========================================================
class CMapObject_Light : public CMapObject
{
public:
	CMapObject_Light();
	CMapObject_Light(const char* aModelFliename, int nPriority = 3);
	~CMapObject_Light();

	HRESULT Init(void);   // ����������    
	void Uninit(void);    // �I������
	void Update(void);    // �X�V����
	void Draw(void);      // �`�揈��

	static CMapObject_Light* Create(const char* aModelFliename, int nPriority = 3);  //����
	void ChangeState(CLightState* pState);  // �X�e�[�g��ύX

	// �擾
	CMapObject_Light* GetTop(void) { return m_pTop; }  // ���X�g�̐擪
	int GetFallDownCount(void) { return m_nFallDownCount; }

	// �ݒ�
	void SetFallDown(int nNum) { m_nFallDownCount = nNum; }
	void SetLightFall(bool nValue) { m_bLightFall = nValue; }

private:

	CLightState* m_pState;  // �X�e�[�g�̃|�C���^

	static CMapObject_Light* m_pTop;  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CMapObject_Light* m_pCur;  // �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_Light* m_pNext;        // ���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_Light* m_pPrev;        // �O�̃I�u�W�F�N�g�ւ̃|�C���^

	bool m_bLightFall = false;

	int m_nFallDownCount = 0;
};

//===========================================================
// �X�e�C�g
//===========================================================
class CLightState
{
public:
	CLightState() {}
	~CLightState() {}

	virtual void Update(CMapObject_Light* pTrashBox) = 0;

private:

};

// �j���[�g����
class CLightNeutral : public CLightState
{
public:
	CLightNeutral();
	~CLightNeutral() {}

	void Update(CMapObject_Light* pLighth) override;

private:

};

// �������
class CLightFall : public CLightState
{
public:
	CLightFall();
	~CLightFall() {}

	void Update(CMapObject_Light* pLighth) override;

private:

	CObjectX* m_pLightR = nullptr;
	CObjectX* m_pLightL = nullptr;

};

// �o�E���h
class CLightBound : public CLightState
{
public:
	CLightBound();
	~CLightBound() {}

	void Update(CMapObject_Light* pLighth) override;

private:

};

// ����ꂽ
class CLightBeaten : public CLightState
{
public:
	CLightBeaten();
	~CLightBeaten() {}

	void Update(CMapObject_Light* pLighth) override;

private:

	int m_nShakeTimeCounter;
	int m_nFallDownCount;
	float m_fShakeAngle;
};


#endif