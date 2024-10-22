//===========================================================
//
// �G�������Ă����[bullet.cpp]
// Author �匴�叫
//
//===========================================================
#include "manager.h"
#include "player.h"
#include "sound.h"
#include "bullet.h"
#include "enemy.h"
#include "effect.h"
#include "collision.h"
#include "MyEffekseer.h"

//================================================================
// �萔��`
//================================================================
namespace
{
	const float WIDTH = 10.0f;       // ��
	const float HEIGHT = 10.0f;      // ����
	const float SPEED = 7.0f;        // �ړ��X�s�[�h
	const float KNOCK_BACK = 5.0f;   // �v���C���[���m�b�N�o�b�N����l
	const int LIFE = 120;            // �̗�
	const int DAMAGE = 10;           // �U����
	const char* MODEL_PATH = "data\\MODEL\\01_can.x";  // ���f���̃p�X
}

//================================================================
// �ÓI�����o�ϐ��錾
//================================================================
CBullet *CBullet::m_Bullet = NULL;

//================================================================
// �R���X�g���N�^
//================================================================
CBullet::CBullet()
{
	m_nLife = 0;
	m_move = { 0.0f, 0.0f, 0.0f };  //�ړ���
}

//================================================================
// �R���X�g���N�^
//================================================================
CBullet::CBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	SetPosition(pos);
	SetRotition(rot);
	
	m_nLife = 0;
	m_move = { 0.0f, 0.0f, 0.0f };  //�ړ���
}

//================================================================
// �f�X�g���N�^
//================================================================
CBullet::~CBullet()
{

}

//================================================================
// ��������
//================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//�I�u�W�F�N�g2D�̃|�C���^
	CBullet *pBullet = nullptr;

	if (pBullet == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pBullet = new CBullet(pos, rot);

		//����������
		pBullet->Init();
	}
	
	return pBullet;
}

//================================================================
// ����������
//================================================================
HRESULT CBullet::Init(void)
{
	SetModelFileName(MODEL_PATH);

	//����������
	CObjectX::Init();

	//�e�̎���
	m_nLife = LIFE;

	return S_OK;
}

//================================================================
// �I������
//================================================================
void CBullet::Uninit(void)
{
	//�I������
	CObjectX::Uninit();
}

//================================================================
// �X�V����
//================================================================
void CBullet::Update(void)
{
	// �����蔻��̏��擾
	CCollision* pCollision = CCollision::GetInstance();

	// �v���C���[�̏��擾
	CPlayer* pPlayer = CPlayer::GetInstance();

	// �ʒu����
	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 rot = GetRotition();

	// �O��̈ʒu���L�^
	m_posOld = pos;

	// �ʒu���X�V
	pos.x -= sinf(rot.y) * SPEED;
	pos.z -= cosf(rot.y) * SPEED;

	// �X�V����
	CObjectX::Update();

	// ���������炷
	m_nLife--;

	if (pPlayer != nullptr)
	{
		// �v���C���[�Ƃ̓����蔻��
		if (pCollision->Circle(pos, pPlayer->GetPosition(), WIDTH, 50.0f))
		{
			// �������Ȃ���
			m_nLife = 0;

			// �v���C���[�̃_���[�W����
			pPlayer->Damage(DAMAGE, D3DXVECTOR3(sinf(rot.y) * KNOCK_BACK, 0.0f, cosf(rot.y) * KNOCK_BACK));

			// �G�t�F�N�g�̍Đ�
			CManager::GetInstance()->GetMyEffekseer()->Set(CMyEffekseer::TYPE_HIT, ::Effekseer::Vector3D(pos.x, pos.y, pos.z));
		}
	}

	pos.y -= 1.0f;
		
	if (m_nLife <= 0 || pos.y <= 0.0f)
	{// �������Ȃ��Ȃ�����

		// �I������
		Uninit();

		return;
	}

	// �ʒu��ݒ�
	SetPosition(pos);
}

//================================================================
// �`�揈��
//================================================================
void CBullet::Draw(void)
{
	//�`�揈��
	CObjectX::Draw();
}