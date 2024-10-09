//===========================================================
//
// �}�b�v�ɔz�u��Ă���j�󂳂ꂽ�O������[mapobject_brokenlight.h]
// Author �匴�叫
//
//===========================================================
#ifndef _MAPOBJECT_BROKENLIGHT_H__             //���̃}�N����`������ĂȂ�������
#define _MAPOBJECT_BROKENLIGHT_H__             //2�d�C���N���[�h�h�~�̃}�N����`������

//===========================================================
// �C���N���[�h�t�@�C��
//===========================================================
#include "objectX.h"
#include "mapobject.h"

// �O���錾
class CBrokenLightState;

//===========================================================
// �O���N���X��`
//===========================================================
class CMapObject_BrokenLight : public CMapObject
{
public:
	CMapObject_BrokenLight();
	CMapObject_BrokenLight(const char* aModelFliename, int nPriority = 3);
	~CMapObject_BrokenLight();

	HRESULT Init(void);   // ����������    
	void Uninit(void);    // �I������
	void Update(void);    // �X�V����
	void Draw(void);      // �`�揈��

	static CMapObject_BrokenLight* Create(const char* aModelFliename, int nPriority = 3);  //����
	void ChangeState(CBrokenLightState* pState);  // �X�e�[�g��ύX

	// �擾
	CMapObject_BrokenLight* GetTop(void) { return m_pTop; }  // ���X�g�̐擪
	int GetFallDownCount(void) { return m_nFallDownCount; }

	// �ݒ�
	void SetFallDown(int nNum) { m_nFallDownCount = nNum; }
	void SetLightFall(bool nValue) { m_bLightFall = nValue; }

private:

	CBrokenLightState* m_pState;  // �X�e�[�g�̃|�C���^

	static CMapObject_BrokenLight* m_pTop;  // �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CMapObject_BrokenLight* m_pCur;  // �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_BrokenLight* m_pNext;        // ���̃I�u�W�F�N�g�ւ̃|�C���^
	CMapObject_BrokenLight* m_pPrev;        // �O�̃I�u�W�F�N�g�ւ̃|�C���^

	bool m_bLightFall = false;

	int m_nFallDownCount = 0;
};

//===========================================================
// �X�e�C�g
//===========================================================
class CBrokenLightState
{
public:
	CBrokenLightState() {}
	~CBrokenLightState() {}

	virtual void Update(CMapObject_BrokenLight* pTrashBox) = 0;

private:

};

// �j���[�g����
class CBrokenLightNeutral : public CBrokenLightState
{
public:
	CBrokenLightNeutral();
	~CBrokenLightNeutral() {}

	void Update(CMapObject_BrokenLight* pLighth) override;

private:

};

// �o�E���h
class CBrokenLightFall : public CBrokenLightState
{
public:
	CBrokenLightFall();
	~CBrokenLightFall() {}

	void Update(CMapObject_BrokenLight* pLighth) override;

private:

};

#endif