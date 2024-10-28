//===========================================================
//
// �G�t�F�N�V�A[MyEffekseer.cpp]
// Author �匴�叫
//
//===========================================================
#include <stdio.h>
#include <string>
#include <windows.h>

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "MyEffekseer.h"
#include "camera.h"

//===========================================================
// �ÓI�����o�ϐ�
//===========================================================
std::list<CEffekseer*> CMyEffekseer::m_List = {};  // ���X�g

//===========================================================
// �萔��`
//===========================================================
namespace
{
	const float DELTA_TIME = 60.0f;
}

namespace MyEffekseer
{
	// �G�t�F�N�g�̖��O
	const char* EFFECT_PATH[CMyEffekseer::TYPE_MAX] =
	{
		"",                                                // �Ȃ���Ȃ�
		"data\\EFFEKSEER\\Effect\\impact.efkefc",          // �Ռ��g
		"data\\EFFEKSEER\\Effect\\hit.efkefc",             // �q�b�g
		"data\\EFFEKSEER\\Effect\\attack_impact.efkefc",   // �U���̗\��
		"data\\EFFEKSEER\\Effect\\taillamp.efkefc",        // �O��
	};

	//============================================
	// �G�t�F�N�g�̐���
	// �p�����[�^�[
	// type  : ��
	// bLopp : ���[�v���邩�̃t���O
	// pos   : �G�t�F�N�g�̈ʒu
	// rot   : �G�t�F�N�g�̌���
	// scale : �G�t�F�N�g�̑傫��
	// 
	// �߂�l
	// �G�t�F�N�g�B���s�����Ƃ���nullptr���Ԃ�
	//============================================
	CEffekseer* EffectCreate(CMyEffekseer::TYPE type, bool bLoop, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
	{
		// �G�t�F�N�V�A�擾
		CMyEffekseer* pMyEffekseer = CManager::GetInstance()->GetMyEffekseer();

		// �g�p����Ă��Ȃ��ꍇ�����𔲂���
		if (pMyEffekseer == nullptr)
			return nullptr;

		// �G�t�F�N�g�𐶐�
		CEffekseer* pEffekseer = pMyEffekseer->CreateEffect(EFFECT_PATH[type],
			Effekseer::Vector3D(pos.x, pos.y, pos.z),
			Effekseer::Vector3D(rot.x, rot.y, rot.z),
			Effekseer::Vector3D(scale.x, scale.y, scale.z),
			bLoop);

		return pEffekseer;
	}
}

//===========================================================
// �R���X�g���N�^
//===========================================================
CMyEffekseer::CMyEffekseer()
{
}

//===========================================================
// �f�X�g���N�^
//===========================================================
CMyEffekseer::~CMyEffekseer()
{
}

//===========================================================
// ��������
//===========================================================
CEffekseer* CMyEffekseer::CreateEffect(const char* FileName, ::Effekseer::Vector3D pos, ::Effekseer::Vector3D rot, ::Effekseer::Vector3D scale, bool bLoop)
{
	CEffekseer* pEffect = new CEffekseer;

	if (pEffect == nullptr)
		return nullptr;

	// char16_t�ɕϊ�
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(FileName);

	// �G�t�F�N�g�̓Ǎ�
	Effekseer::EffectRef effect = Effekseer::Effect::Create(m_EfkManager, string16t.c_str());
	pEffect->SetEffect(effect);

	// �G�t�F�N�g�̍Đ�
	Effekseer::Handle Handle = m_EfkManager->Play(effect, 0, 0, 0);
	pEffect->SetEfkHandle(Handle);

	// �G�t�F�N�g�l�[���ݒ�
	pEffect->SetEfkName(FileName);

	// �ʒu�ݒ�
	pEffect->SetPosition(pos);

	// �����ݒ�
	pEffect->SetRotation(rot);

	// �傫���ݒ�
	pEffect->SetScale(scale);

	// ����������
	pEffect->Init(pos, rot, scale, bLoop);

	m_List.push_back(pEffect);

	// �ʒu�A�����A�傫���ݒ�
	m_EfkManager->SetLocation(Handle, pos);
	m_EfkManager->SetRotation(Handle, { 0.0f, 1.0f, 0.0f }, rot.Y);
	m_EfkManager->SetScale(Handle, scale.X, scale.Y, scale.Z);

	return pEffect;
}

//===========================================================
// ����������
//===========================================================
void CMyEffekseer::Init(void)
{
	// �S�v�f�폜
	m_List.clear();

	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	m_EfkManager = ::Effekseer::Manager::Create(8000);

	// ���_�ʒu���m��
	m_ViewerPosition = ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f);

	// ���W�n��ݒ肷��B�A�v���P�[�V�����ƈ�v������K�v������B
	m_EfkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// Effekseer�̃��W���[�����Z�b�g�A�b�v����
	SetupEffekseerModules(m_EfkManager);

	// ���e�s���ݒ�
	m_ProjectionMatrix.PerspectiveFovLH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// �J�����s���ݒ�
	m_CameraMatrix.LookAtLH(m_ViewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
}

//===========================================================
// �I������
//===========================================================
void CMyEffekseer::Uninit(void)
{
	// ���ׂĂ̗v�f���폜����
	ReleaseAll();
}

//===========================================================
// �X�V����
//===========================================================
void CMyEffekseer::Update(void)
{
	// �^�C�����Z
	m_nTime++;

	for(auto it = m_List.begin(); it != m_List.end();)
	{
		// �n���h���A�ʒu�A�����A�傫�����擾
		Effekseer::Handle Handle = (*it)->GetHandle();
		Effekseer::Vector3D pos = (*it)->GetPosition();
		Effekseer::Vector3D rot = (*it)->GetRotation();
		Effekseer::Vector3D scale = (*it)->GetScale();

		// �G�t�F�N�g�̍Đ����I������
		if (!m_EfkManager->Exists(Handle))
		{
			// �Đ��̒�~
			m_EfkManager->StopEffect(Handle);

			// ���[�v����t���O�������Ă���
			if ((*it)->IsLoop())
			{
				Effekseer::EffectRef effect = (*it)->GetEffect();

				// �G�t�F�N�g�̍Đ�
				Handle = m_EfkManager->Play(effect, pos);

				// �n���h���̐ݒ�
				(*it)->SetEfkHandle(Handle);

				// �ʒu������A�傫����������x�ݒ�
				m_EfkManager->SetLocation(Handle, pos);
				m_EfkManager->SetRotation(Handle, { 0.0f, 1.0f, 0.0f }, rot.Y);
				m_EfkManager->SetScale(Handle, scale.X, scale.Y, scale.Z);

				// ���̗v�f
				it++;
			}
			else
			{
				// �w�肳�ꂽ�v�f���폜����
				it = m_List.erase(it);
			}
		}
		else
		{
			(*it)->SetPosition(pos);
			(*it)->SetRotation(rot);
			(*it)->SetScale(scale);

			// �ʒu������A�傫����������x�ݒ�
			m_EfkManager->SetLocation(Handle, pos);
			m_EfkManager->SetRotation(Handle, { 0.0f, 1.0f, 0.0f }, rot.Y);
			m_EfkManager->SetScale(Handle, scale.X, scale.Y, scale.Z);

			// ���̗v�f
			it++;
		}
	}

	// ���C���[�p�����[�^�̐ݒ�
	Effekseer::Manager::LayerParameter layerParameter;
	layerParameter.ViewerPosition = m_ViewerPosition;
	m_EfkManager->SetLayerParameter(0, layerParameter);

	// �}�l�[�W���[�̍X�V
	Effekseer::Manager::UpdateParameter updateParameter;
	m_EfkManager->Update(updateParameter);
}

//===========================================================
// �`�揈��
//===========================================================
void CMyEffekseer::Draw(void)
{
	// �����_���[���g�p����Ă��Ȃ��ꍇ�����𔲂���
	if (m_EfkRenderer == nullptr)
		return;

	// �^�C���ݒ�
	m_EfkRenderer->SetTime(m_nTime / DELTA_TIME);

	// ���e�s���ݒ�
	m_EfkRenderer->SetProjectionMatrix(m_ProjectionMatrix);

	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// �J�������g�p����Ă��Ȃ��ꍇ����������
	if (pCamera == nullptr)
		return;

	auto ViewMatrix = pCamera->GetView();
	auto Projection = pCamera->GetProjection();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_ProjectionMatrix.Values[i][j] = Projection.m[i][j];
			m_CameraMatrix.Values[i][j] = ViewMatrix.m[i][j];
		}
	}

	// �J�����s���ݒ�
	m_EfkRenderer->SetCameraMatrix(m_CameraMatrix);

	// �G�t�F�N�g�̕`��J�n����
	m_EfkRenderer->BeginRendering();

	// �G�t�F�N�g�̕`��
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = m_EfkRenderer->GetCameraProjectionMatrix();
	m_EfkManager->Draw(drawParameter);

	// �G�t�F�N�g�̕`��I������
	m_EfkRenderer->EndRendering();
}

//===========================================================
// ���W���[���ݒ�
//===========================================================
void CMyEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef efkManager)
{
	// �`��f�o�C�X�̍쐬
	Effekseer::Backend::GraphicsDeviceRef graphicsDevice;
	graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(CManager::GetInstance()->GetRenderer()->GetDevice());

	// �G�t�F�N�g�̃����_���[�̍쐬
	m_EfkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// �`�惂�W���[���̐ݒ�
	efkManager->SetSpriteRenderer(m_EfkRenderer->CreateSpriteRenderer());
	efkManager->SetRibbonRenderer(m_EfkRenderer->CreateRibbonRenderer());
	efkManager->SetRingRenderer(m_EfkRenderer->CreateRingRenderer());
	efkManager->SetTrackRenderer(m_EfkRenderer->CreateTrackRenderer());
	efkManager->SetModelRenderer(m_EfkRenderer->CreateModelRenderer());

	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���
	efkManager->SetTextureLoader(m_EfkRenderer->CreateTextureLoader());
	efkManager->SetModelLoader(m_EfkRenderer->CreateModelLoader());
	efkManager->SetMaterialLoader(m_EfkRenderer->CreateMaterialLoader());
	efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

//===========================================================
// �w�肵���v�f���폜
//===========================================================
void CMyEffekseer::Release(CEffekseer* pEffect)
{
	// �g�p����Ă��Ȃ��ꍇ�����𔲂���
	if (pEffect == nullptr)
		return;

	// �I������
	pEffect->Uninit();

	// �w�肵���v�f���폜����
	m_List.remove(pEffect);
}

//===========================================================
// ���ׂĂ̗v�f���폜
//===========================================================
void CMyEffekseer::ReleaseAll(void)
{
	// ���ׂĂ̗v�f���폜
	m_List.clear();
}

void CMyEffekseer::ListIn(CEffekseer* pEffect)
{

}

//===========================================================================
// �G�t�F�N�g�N���X
//===========================================================================
//===========================================================
// �R���X�g���N�^
//===========================================================
CEffekseer::CEffekseer()
{

}

//===========================================================
// �f�X�g���N�^
//===========================================================
CEffekseer::~CEffekseer()
{

}

//===========================================================
// ����������
//===========================================================
void CEffekseer::Init(Effekseer::Vector3D pos, Effekseer::Vector3D rot, Effekseer::Vector3D scale, bool bLoop)
{
	m_bLoop = bLoop;
}

//===========================================================
// �I������
//===========================================================
void CEffekseer::Uninit(void)
{

}

void CEffekseer::FollowPosition(D3DXVECTOR3 pos)
{
	SetPosition(Effekseer::Vector3D(pos.x, pos.y, pos.z));
}