#include "stdafx.h"
#include "CyclopsAttack_P.h"
#include "Transform.h"
#include "Collider.h"

CCyclopsAttack_P::CCyclopsAttack_P(const OBJ_TYPE & _eType, CObj * _pOwner)
	: CProjectile(_eType, _pOwner)
{
}


CCyclopsAttack_P::~CCyclopsAttack_P()
{
}

const bool CCyclopsAttack_P::Init()
{
	Create_Transform();
	Create_Collider();

	m_pTransform->Init();
	m_pTransform->Set_Pos(VEC2_ZERO);
	m_pTransform->Set_Scale(VEC2(150.f, 150.f));

	m_pCollider->Init();
	m_pCollider->Set_Pos(m_pTransform->Get_Pos());
	m_pCollider->Set_Scale(m_pTransform->Get_Scale());

	return true;
}

void CCyclopsAttack_P::Render(HDC _hDC)
{
	Component_Render(_hDC);
}

void CCyclopsAttack_P::Set_On(const bool & _bOn)
{
	m_pCollider->Set_Active(_bOn);
}

const bool CCyclopsAttack_P::Is_On()
{
	return m_pCollider->Is_Active();
}
