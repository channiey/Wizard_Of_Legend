#pragma once
#include "Effect.h"
class CPortalEffect :
	public CEffect
{
public:
	CPortalEffect(const OBJ_TYPE& _eType, CObj* _pOwner);
	virtual ~CPortalEffect();

public:
	virtual const bool Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;
	
public:
	virtual const bool	Init_Animator();

public:
	virtual void On(const VEC2& _vPos) override;
	virtual void Off(const VEC2& _vPos) override {}
};

