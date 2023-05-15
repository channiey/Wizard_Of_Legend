#pragma once
#include "Skill.h"

class CWindSlash_P;

class CWindSlash_S :
	public CSkill
{
public:
	CWindSlash_S(CObj* _pObj, const float& _fCoolTime, const float& _fDamage);
	virtual ~CWindSlash_S();

public:
	virtual void Init() override;
	virtual void Update() override;
	void Release();

public:
	virtual void Fire() override;

private:
	CWindSlash_P* pProj;
};

