#include "stdafx.h"
#include "SnowParticle.h"
#include "Transform.h"
#include "Animator.h"
#include "Animation.h"
#include "ResMgr.h"
#include "Texture.h"
#include "ParticleAnimator.h"
#include "ObjPool.h"
CSnowParticle::CSnowParticle(const OBJ_TYPE& _eType)
	: CParticle(_eType)
{
}


CSnowParticle::~CSnowParticle()
{
}

const bool CSnowParticle::Init()
{
	Create_Transform();
	m_pTransform->Init();


	m_pAnimator = new CParticleAnimator(this);
	m_pAnimator->Init();
	m_pAnimator->Set_Use_Dir(false);

	Init_Animator();
	m_listComponent.push_back(m_pAnimator);
	m_fRenderMag = 5.f;

	return true;
}

void CSnowParticle::Update()
{
	if (!m_bActive) return;
}

void CSnowParticle::LateUpdate()
{
	if (!m_bActive) return;

	Component_LateUpdate();
}

void CSnowParticle::Render(HDC _hDC)
{
	if (!m_bActive) return;

	Component_Render(_hDC);
}

void CSnowParticle::Reset(CObj* _pOwner)
{
	if (nullptr == _pOwner) return;
	m_pOwner = _pOwner;

	m_pTransform->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());
	m_pAnimator->Play(L"MOVE", false);
}

const bool CSnowParticle::Init_Animator()
{
	CTexture* pTex;
	CAnimation* pAnimation;
	float fDuration = 0.2f;
	// IDLE
	{
		fDuration = 0.1f;
	//C:\Users\ChanYoon\Desktop\WizardOfLegend_6\Data\Sprite\09.Particle\Snow_Ice
		pAnimation = new CAnimation(L"MOVE", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_PARTICLE_0", L"../Data/Sprite/09.Particle/Snow_Ice/IcicleBreakForm0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_PARTICLE_1", L"../Data/Sprite/09.Particle/Snow_Ice/IcicleBreakForm1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_PARTICLE_2", L"../Data/Sprite/09.Particle/Snow_Ice/IcicleBreakForm2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"ICE_PARTICLE_3", L"../Data/Sprite/09.Particle/Snow_Ice/IcicleBreakForm3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		


		m_pAnimator->PushBack_Ani(pAnimation);
	}

	return true;
}

void CSnowParticle::Return()
{
	CObjPool<CSnowParticle>::Return_Obj(this);
}
