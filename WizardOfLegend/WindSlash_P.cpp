#include "stdafx.h"
#include "WindSlash_P.h"
#include "TimeMgr.h"
#include "MainGame.h"
#include "Transform.h"
#include "Collider.h"
#include "Texture.h"
#include "ResMgr.h"
#include "Animator.h"
#include "Animation.h"
#include "WindSlashAnimator.h"
#include "ObjPool.h"
CWindSlash_P::CWindSlash_P(const OBJ_TYPE & _eType)
	: CProjectile(_eType)
	, m_bOn(false)
{
}


CWindSlash_P::~CWindSlash_P()
{
}

const bool CWindSlash_P::Init()
{	
	// Component Setting
	Create_Transform();
	Create_Collider();

	m_pTransform->Init();
	//m_pTransform->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());
	m_pTransform->Set_Scale(VEC2(150.f, 150.f));

	m_pCollider->Init();
	//m_pCollider->Set_Pos(m_pTransform->Get_Pos());
	m_pCollider->Set_Scale(m_pTransform->Get_Scale());

	m_pAnimator = new CWindSlashAnimator(this);
	m_pAnimator->Init();
	m_listComponent.push_back(m_pAnimator);
	Init_Animator();

	// Render Setting
	m_fRenderMag = 3.f;

	return false;
}

void CWindSlash_P::Update()
{
	if (!m_bActive) return;
}

void CWindSlash_P::LateUpdate()
{
	if (!m_bActive) return;
	Component_LateUpdate();
}

void CWindSlash_P::Render(HDC _hDC)
{
	if (!m_bActive) return;
	if (!m_bOn) return;

	Component_Render(_hDC);
}

void CWindSlash_P::Release()
{
}

void CWindSlash_P::OnCollision_Enter(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{

}

void CWindSlash_P::OnCollision_Stay(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
}

void CWindSlash_P::OnCollision_Exit(CCollider * _pOtherCol, const float& _fCX, const float& _fCY)
{
	
}

void CWindSlash_P::Return()
{
	CObjPool<CWindSlash_P>::Return_Obj(this);
}


void CWindSlash_P::On()
{
	m_bOn = true;
	m_pCollider->Set_Active(true);
	m_pAnimator->Set_Active(true);

	float	fHoriTerm = 100.f;
	float	fVertTerm = 75.f;

	VEC2 vLook = m_pOwner->Get_Transform()->Get_Look();
	VEC2 vPos = m_pOwner->Get_Transform()->Get_Pos();

	if (fabs(vLook.fX) < fabs(vLook.fY))
		if (0.f < vLook.fY)
			vPos.fY += fVertTerm;
		else
			vPos.fY -= fVertTerm;
	else
		if (0.f < vLook.fX)
			vPos.fX += fHoriTerm;
		else
			vPos.fX -= fHoriTerm;
	

	m_pTransform->Set_Pos(vPos);
	m_pTransform->Set_Look(vLook);

	m_pAnimator->Play(L"SLASH", false);
}

void CWindSlash_P::Off()
{
	//m_bOn = false;
	//m_pCollider->Set_Active(false); // TODO:: 이거 끄는 거랑 프로젝타일들 벽이랑 충돌 안되는 게 무슨 상관이지..?
	//m_pAnimator->Set_Active(false);
}

const bool CWindSlash_P::Init_Animator()
{
	CTexture* pTex;
	CAnimation* pAnimation;
	float fDuration = 0.2f;

	pAnimation = new CAnimation(L"SLASH", m_pAnimator);

	{
		pTex = CResMgr::Get_Inst()->Load_Tex(L"SLASH_DOWN", L"../Data/Sprite/08.Effect/Slash/wind/Wind_Slash_00.png");
		pTex->Get_Image()->RotateFlip(Rotate180FlipX);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"SLASH_UP", L"../Data/Sprite/08.Effect/Slash/wind/Wind_Slash_00.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::UP, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"SLASH_RIGHT", L"../Data/Sprite/08.Effect/Slash/wind/Wind_Slash_00.png");
		pTex->Get_Image()->RotateFlip(Rotate90FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"SLASH_LEFT", L"../Data/Sprite/08.Effect/Slash/wind/Wind_Slash_00.png");
		pTex->Get_Image()->RotateFlip(Rotate270FlipNone);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuration);
	}

	m_pAnimator->PushBack_Ani(pAnimation);

	return true;
}
