#include "stdafx.h"
#include "Transform.h"


CTransform::CTransform(CObj * _pOwner)
	: CComponent(_pOwner)
	, m_fTheta(0.f)
{
	ZeroMemory(&m_vPos, sizeof(VEC2));
	ZeroMemory(&m_vScale, sizeof(VEC2));
	m_vLook = { 0.f, 1.f };
	m_eType = COMPONENT_TYPE::TRANSFORM;
}

CTransform::~CTransform()
{
}
