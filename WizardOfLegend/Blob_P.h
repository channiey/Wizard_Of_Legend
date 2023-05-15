#pragma once
#include "Projectile.h"

class CBlob_P :
	public CProjectile
{
public:
	CBlob_P(const OBJ_TYPE & _eType, CObj * _pOwner);
	virtual ~CBlob_P();

public:
	virtual const bool Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;
	virtual void Return() override;

public:
	void Set_On(const bool& _bOn);
};

