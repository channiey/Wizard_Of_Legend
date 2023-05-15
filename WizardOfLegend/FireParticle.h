#pragma once
#include "Particle.h"
class CFireParticle :
	public CParticle
{
public:
	CFireParticle(const OBJ_TYPE& _eType);
	virtual ~CFireParticle();

public:
	virtual const bool Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

public:
	virtual void Reset(CObj* _pOwner) override;
	virtual const bool	Init_Animator() override;
	virtual void	Return() override;
};

