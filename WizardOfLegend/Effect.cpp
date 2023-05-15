#include "stdafx.h"
#include "Effect.h"


CEffect::CEffect(const OBJ_TYPE & _eType)
	: CObj(_eType)
{
}

CEffect::CEffect(const OBJ_TYPE& _eType, CObj* _pOwner)
	: CObj(_eType)
	, m_pOwner(_pOwner)
{
}


CEffect::~CEffect()
{
}
