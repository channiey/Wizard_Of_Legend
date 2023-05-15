#include "stdafx.h"
#include "Projectile.h"
#include "Transform.h"
#include "CameraMgr.h"
#include "TimeMgr.h"
CProjectile::CProjectile(const OBJ_TYPE & _eType)
	: CObj(_eType)
	, m_pOwner(nullptr)
	, m_bMove(true)
	, m_fAccParticle(0.f)
	, m_fParticleCreateTerm(0.05f)
{
}

CProjectile::CProjectile(const OBJ_TYPE & _eType, CObj * _pOwner)
	: CObj(_eType)
	, m_pOwner(_pOwner)
	, m_bMove(true)
	, m_fAccParticle(0.f)
	, m_fParticleCreateTerm(0.03f)
{
}

CProjectile::~CProjectile()
{
}

const bool CProjectile::Create_Particle()
{
	m_fAccParticle += DT;
	if (m_fAccParticle >= m_fParticleCreateTerm)
	{
		m_fAccParticle = 0.f;
		return true;
	}
	return false;
}

void CProjectile::Move()
{
	m_pTransform->Add_Pos(m_pTransform->Get_Look() * m_fSpeed * DT);
}

const bool CProjectile::Out_Of_Screen()
{
	VEC2 vCenterPos = CCameraMgr::Get_Inst()->Get_LookAt();

	VEC2 vProjPos = m_pTransform->Get_Pos();
	VEC2 vProjScale = m_pTransform->Get_Scale();

	if ((vProjPos.fX - (vProjScale.fX * 0.5f) + 300 < vCenterPos.fX - (WINCX * 0.5f) || vCenterPos.fX + (WINCX * 0.5f) + 300 < vProjPos.fX + (vProjScale.fX * 0.5f))
		|| (vProjPos.fY - (vProjScale.fY * 0.5f) + 300 < vCenterPos.fY - (WINCY * 0.5f) || vCenterPos.fY + (WINCY * 0.5f) + 300 < vProjPos.fY + (vProjScale.fY * 0.5f)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

