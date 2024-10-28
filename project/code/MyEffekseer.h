//===========================================================
//
// �G�t�F�N�V�A[MyEffekseer.h]
// Author �匴�叫
//
//===========================================================
#ifndef _EFFEKSEER_H_     //���̃}�N������`����ĂȂ�������
#define _EFFEKSEER_H_     //2�d�C���N���[�h��h�~�̃}�N�����`����

#define MAX_EFK   (64)

#include <list>

//===========================================================
// �O���錾
//===========================================================v
class CEffekseer;

//===========================================================
// �G�t�F�N�V�A�̃R���g���[���N���X��`
//===========================================================
class CMyEffekseer
{
public:
	CMyEffekseer();   // �R���X�g���N�^
	~CMyEffekseer();  // �f�X�g���N�^

	// ���
	enum TYPE
	{
		TYPE_NONE = 0,  // �Ȃ���Ȃ�
		TYPE_IMPACT,    // �Ռ��g
		TYPE_HIT,       // �q�b�g
		TYPE_ATTACK,    // �U���̗\��
		TYPE_ORBIT,     // �O��
		TYPE_MAX
	};

	void Init(void);       // ����������
	void Uninit(void);     // �I������
	void Update(void);     // �X�V����
	void Draw(void);       // �`�揈��

	// �擾
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer(void) { return m_EfkRenderer; }  // �G�t�F�N�g�����_���[
	::Effekseer::ManagerRef GetEfkManager(void) { return m_EfkManager; }                      // �G�t�F�N�g�}�l�[�W��

	static CMyEffekseer* GetInstance(void) { return m_pInstance; }  // �������g

	// ���W���[��
	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);

	CEffekseer* CreateEffect(const char* FileName, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale, bool bLoop);

	void ListIn(CEffekseer* pEffect);  // ���X�g�ɑ}��

	void Release(CEffekseer* pEffect);  // �w�肵���v�f���폜
	void ReleaseAll(void);              // ���ׂĂ̗v�f���폜

private:

	static std::list<CEffekseer*> m_List;  // �G�t�F�N�g�̃��X�g

	static CMyEffekseer* m_pInstance;  // �������g�̃|�C���^
	EffekseerRendererDX9::RendererRef m_EfkRenderer;
	::Effekseer::Matrix44 m_CameraMatrix;
	::Effekseer::Matrix44 m_ProjectionMatrix;
	::Effekseer::ManagerRef m_EfkManager;
	::Effekseer::Vector3D m_ViewerPosition;
	int m_nNum;
	int32_t m_nTime;
};

//===========================================================
// �G�t�F�N�V�A�N���X��`
//===========================================================
class CEffekseer
{
public:
	CEffekseer();     // �R���X�g���N�^
	~CEffekseer();    // �f�X�g���N�^

	void Init(Effekseer::Vector3D pos, Effekseer::Vector3D rot, Effekseer::Vector3D scale, bool bLoop);                   // ����������
	void Uninit(void);                 // �I������ 

	void FollowPosition(D3DXVECTOR3 pos);

	// �ݒ�
	void SetPosition(Effekseer::Vector3D pos) { m_pos = pos; }             // �ʒu
	void SetRotation(Effekseer::Vector3D rot) { m_rot = rot; }             // ����
	void SetScale(Effekseer::Vector3D scale) { m_scale = scale; }          // �傫��
	void SetEffect(Effekseer::EffectRef effect) { m_effect = effect; }     // �G�t�F�N�g
	void SetEfkHandle(Effekseer::Handle handle) { m_efkHandle = handle; }  // �n���h��
	void SetEfkName(const char* pName) { m_EfkName = pName; }              // �G�t�F�N�g�̖��O
	
	// �擾
	Effekseer::Vector3D GetPosition(void) { return m_pos; }                // �ʒu
	Effekseer::Vector3D GetRotation(void) { return m_rot; }                // ����
	Effekseer::Vector3D GetScale(void) { return m_scale; }                 // �傫��
	Effekseer::EffectRef GetEffect(void) { return m_effect; }              // �G�t�F�N�g
	Effekseer::Handle GetHandle(void) { return m_efkHandle; }              // �n���h��
	const char* GetEfkName(void) { return m_EfkName; }                     // �G�t�F�N�g�̖��O
	bool IsLoop(void) { return m_bLoop; }                                  // ���[�v����t���O

private:

	// �����o�ϐ�
	Effekseer::Vector3D m_pos;       // �ʒu
	Effekseer::Vector3D m_rot;       // ����
	Effekseer::Vector3D m_scale;     // �傫��
	Effekseer::EffectRef m_effect;   // �G�t�F�N�g
	Effekseer::Handle m_efkHandle;   // �n���h��
	const char* m_EfkName;           // ���O
	bool m_bLoop;                    // ���[�v����t���O
	
};

namespace MyEffekseer
{
	CEffekseer* EffectCreate(CMyEffekseer::TYPE type, 
		                     bool bLoop = false,
		                     D3DXVECTOR3 pos = { 0.0f, 0.0f, 0.0f }, 
		                     D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f },
		                     D3DXVECTOR3 scale = { 25.0f, 25.0f, 25.0f } );
}

#endif