#pragma once
#include "Projectile.h"
class CCyclopsAttack_P :
	public CProjectile
{
public:
	explicit CCyclopsAttack_P(const OBJ_TYPE & _eType, CObj * _pOwner);
	virtual ~CCyclopsAttack_P();

public:
	virtual const bool Init() override;
	virtual void Update() override {}
	virtual void LateUpdate() override {}
	virtual void Render(HDC _hDC) override;
	virtual void Return() override {}

public:
	void Set_On(const bool& _bOn);
	const bool	Is_On();
};

