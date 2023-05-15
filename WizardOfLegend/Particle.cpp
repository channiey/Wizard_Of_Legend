#include "stdafx.h"
#include "Particle.h"


CParticle::CParticle(const OBJ_TYPE& _eType)
	: CObj(_eType)
	, m_pOwner(nullptr)
{
}


CParticle::~CParticle()
{
}
