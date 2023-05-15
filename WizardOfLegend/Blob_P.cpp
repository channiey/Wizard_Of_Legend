#include "stdafx.h"
#include "Blob_P.h"
#include "Transform.h"
#include "Collider.h"
#include "Blob.h"
#include "MathLib.h"
CBlob_P::CBlob_P(const OBJ_TYPE & _eType, CObj * _pOwner)
	: CProjectile(_eType, _pOwner)
{
}


CBlob_P::~CBlob_P()
{
}

const bool CBlob_P::Init()
{
	Create_Transform();
	Create_Collider();

	m_pTransform->Init();
	m_pTransform->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());
	m_pTransform->Set_Scale(VEC2{ m_pOwner->Get_Transform()->Get_Scale().fX - 20.f, m_pOwner->Get_Transform()->Get_Scale().fY - 20.f });

	m_pCollider->Init();
	m_pCollider->Set_Pos(m_pTransform->Get_Pos());
	m_pCollider->Set_Scale(m_pTransform->Get_Scale());
	return true;
}

void CBlob_P::Update()
{
	
}

void CBlob_P::LateUpdate()
{
	if (!m_bActive) return;

	m_pTransform->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());
	m_pCollider->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());

	Component_LateUpdate();

	CObj* pTarget = static_cast<CBlob*>(m_pOwner)->Get_Target();
	if (nullptr == pTarget) return;

	float fTheta = CMathLib::Get_Angle(m_pOwner->Get_Transform()->Get_Pos(), pTarget->Get_Transform()->Get_Pos());

	m_pTransform->Set_Theta(fTheta);
	
}

void CBlob_P::Render(HDC _hDC)
{
	if (!m_bActive) return;
	Component_Render(_hDC);
}

void CBlob_P::Return()
{
}

void CBlob_P::Set_On(const bool & _bOn)
{
	m_pTransform->Set_Active(_bOn);
	m_pCollider->Set_Active(_bOn);
}
