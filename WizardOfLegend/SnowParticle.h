#pragma once
#include "Particle.h"
class CSnowParticle :
	public CParticle
{
public:
	CSnowParticle(const OBJ_TYPE& _eType);
	virtual ~CSnowParticle();

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

