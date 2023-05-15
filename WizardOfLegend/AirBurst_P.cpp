#include "stdafx.h"
#include "AirBurst_P.h"
#include "Animation.h"
#include "Animator.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Obj.h"
#include "Transform.h"

CAirBurst_P::CAirBurst_P(const OBJ_TYPE& _eType)
	: CProjectile(_eType)
	, m_bOn(false)
{
}


CAirBurst_P::~CAirBurst_P()
{
}

const bool CAirBurst_P::Init()
{
	Create_Transform();
	m_pTransform->Init();
	//m_pTransform->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());
	m_pTransform->Set_Scale(VEC2(50.f, 50.f));

	Create_Animator();
	m_pAnimator->Init();
	Init_Animator();

	m_fRenderMag = 3.f;
	return true;
}

void CAirBurst_P::Update()
{
	if (!m_bActive) return;
}

void CAirBurst_P::LateUpdate()
{
	if (!m_bActive) return;
	Component_LateUpdate();
}

void CAirBurst_P::Render(HDC _hDC)
{
	if (!m_bActive) return;
	Component_Render(_hDC);
}

void CAirBurst_P::Release()
{
}

void CAirBurst_P::OnCollision_Enter(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
}

void CAirBurst_P::OnCollision_Stay(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
}

void CAirBurst_P::OnCollision_Exit(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
}

void CAirBurst_P::Return()
{
}

void CAirBurst_P::On()
{
	float	fHoriTerm = 50.f;
	float	fVertTerm = 30.f;

	VEC2 vLook = m_pOwner->Get_Transform()->Get_Look();
	VEC2 vPos = m_pOwner->Get_Transform()->Get_Pos();

	if (fabs(vLook.fX) < fabs(vLook.fY))
		if (0.f < vLook.fY)
			vPos.fY -= fVertTerm;
		else
			vPos.fY += fVertTerm;
	else
		if (0.f < vLook.fX)
			vPos.fX -= fHoriTerm;
		else
			vPos.fX += fHoriTerm;


	m_pTransform->Set_Pos(vPos);
	m_pTransform->Set_Look(vLook);
	/*if (vLook.fX < 0)
	{
		m_pTransform->the
	}
	else
	{

	}*/

	m_pAnimator->Play(L"AIR_BURST", false);
}

const bool CAirBurst_P::Init_Animator()
{
	CTexture* pTex;
	CAnimation* pAnimation;
	float fDuration = 0.05f;

	pAnimation = new CAnimation(L"AIR_BURST", m_pAnimator);

	{
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_RIGHT_1", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_RIGHT_2", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_RIGHT_3", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_RIGHT_4", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_RIGHT_5", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_LEFT_1", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_0.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_LEFT_2", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_1.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_LEFT_3", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_2.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_LEFT_4", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_3.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_LEFT_5", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_4.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuration);


		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_UP_1", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_0.png");
		pTex->Get_Image()->RotateFlip(Rotate90FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_UP_2", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_1.png");
		pTex->Get_Image()->RotateFlip(Rotate90FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_UP_3", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_2.png");
		pTex->Get_Image()->RotateFlip(Rotate90FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_UP_4", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_3.png");
		pTex->Get_Image()->RotateFlip(Rotate90FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_UP_5", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_4.png");
		pTex->Get_Image()->RotateFlip(Rotate90FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_DOWN_1", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_0.png");
		pTex->Get_Image()->RotateFlip(Rotate270FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_DOWN_2", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_1.png");
		pTex->Get_Image()->RotateFlip(Rotate270FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_DOWN_3", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_2.png");
		pTex->Get_Image()->RotateFlip(Rotate270FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_DOWN_4", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_3.png");
		pTex->Get_Image()->RotateFlip(Rotate270FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"AIR_BURST_DOWN_5", L"../Data/Sprite/08.Effect/Dash/DashAirBurst_4.png");
		pTex->Get_Image()->RotateFlip(Rotate270FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);
	}

	m_pAnimator->PushBack_Ani(pAnimation);

	return true;
}

void CAirBurst_P::Off()
{
}
