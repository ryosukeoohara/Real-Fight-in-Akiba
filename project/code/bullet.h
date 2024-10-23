//===========================================================
//
//�|���S�����o�����[bullet.h]
//Author �匴�叫
//
//===========================================================
#ifndef _BULLET_H_             //���̃}�N����`������ĂȂ�������
#define _BULLET_H_             //2�d�C���N���[�h�h�~�̃}�N����`������

#include "object2D.h"
#include "objectX.h"

//�e�N���X
class CBullet : public CObjectX
{
public:

	typedef enum
	{
		TYPE_NONE = 0,  //�����Ȃ�
		TYPE_PLAYER,    //�v���C���[
		TYPE_ENEMY,     //�G
		TYPE_BOM,       //���e
		TYPE_MAX
	} TYPE;

	CBullet();  //�R���X�g���N�^
	CBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot);  //�R���X�g���N�^(�I�[�o�[���[�h)
	~CBullet();  //�f�X�g���N�^

	HRESULT Init(void);   // ����������    
	void Uninit(void);    // �I������
	void Update(void);    // �X�V����
	void Draw(void);      // �`�揈��

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);  //����
	//static HRESULT Load(void);  //�e�N�X�`���̐���(�ǂݍ���)
	//static void UnLoad(void);   //�e�N�X�`���̔j��
	static CBullet *GetBullet(void) { return m_Bullet; };  //�e���擾

	void SetLife(int nLife) { m_nLife = nLife; }  //�����擾
	int GetLife(void) { return m_nLife; }  //�����ݒ�

private:
	//void ControlPlayer(void);  //�v���C���[�̐���
	//static LPDIRECT3DTEXTURE9 m_pTexture;        //�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_move; //�ړ�
	D3DXVECTOR3 m_posOld; //�O��̈ʒu
	int m_nLife;  //����
	int m_nIdxTexture;
	TYPE m_type;  //���
	static CBullet *m_Bullet;

	bool CollisionEnemy(D3DXVECTOR3 pos);  //�G�Ƃ̓����蔻��
};

#endif