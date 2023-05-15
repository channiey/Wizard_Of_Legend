#include "stdafx.h"
#include "FireParticle.h"
#include "Transform.h"
#include "Animator.h"
#include "Animation.h"
#include "ResMgr.h"
#include "Texture.h"
#include "ParticleAnimator.h"
#include "ObjPool.h"

CFireParticle::CFireParticle(const OBJ_TYPE& _eType)
	: CParticle(_eType)
{
}


CFireParticle::~CFireParticle()
{
}

const bool CFireParticle::Init()
{
	Create_Transform();
	m_pTransform->Init();
	

	m_pAnimator = new CParticleAnimator(this);
	m_pAnimator->Init();
	m_pAnimator->Set_Use_Dir(false);

	Init_Animator();
	m_listComponent.push_back(m_pAnimator);
	m_fRenderMag = 3.f;

	return true;
}

void CFireParticle::Update()
{
	if (!m_bActive) return;
}

void CFireParticle::LateUpdate()
{
	if (!m_bActive) return;

	Component_LateUpdate();

}

void CFireParticle::Render(HDC _hDC)
{
	if (!m_bActive) return;

	Component_Render(_hDC);
}

void CFireParticle::Reset(CObj* _pOwner)
{
	if (nullptr == _pOwner) return;
	m_pOwner = _pOwner;

	m_pTransform->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());
	m_pAnimator->Play(L"MOVE", false);
}

const bool CFireParticle::Init_Animator()
{

	CTexture* pTex;
	CAnimation* pAnimation;
	float fDuration = 0.2f;
	// IDLE
	{
		fDuration = 0.1f;

		pAnimation = new CAnimation(L"MOVE", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_PARTICLE_0", L"../Data/Sprite/09.Particle/Fire/FireParticles_0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_PARTICLE_1", L"../Data/Sprite/09.Particle/Fire/FireParticles_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_PARTICLE_2", L"../Data/Sprite/09.Particle/Fire/FireParticles_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_PARTICLE_3", L"../Data/Sprite/09.Particle/Fire/FireParticles_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_PARTICLE_4", L"../Data/Sprite/09.Particle/Fire/FireParticles_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"FIRE_PARTICLE_5", L"../Data/Sprite/09.Particle/Fire/FireParticles_5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);


		m_pAnimator->PushBack_Ani(pAnimation);
	}

	return false;
}

void CFireParticle::Return()
{
	CObjPool<CFireParticle>::Return_Obj(this);
}
