#include "stdafx.h"
#include "Component.h"
#include "MainGame.h"

CComponent::CComponent(CObj* _pOwner)
	: m_pOwner(_pOwner)
	, m_bActive(true)
	, m_eType(COMPONENT_TYPE::TYPEEND)
{
}

CComponent::~CComponent()
{
}
