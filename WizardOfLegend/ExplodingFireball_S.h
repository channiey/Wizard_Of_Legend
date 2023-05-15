#pragma once
#include "Skill.h"
class CExplodingFireball_S : public CSkill
{
public:
	CExplodingFireball_S(CObj* _pObj, const float& _fCoolTime, const float& _fDamage);
	virtual ~CExplodingFireball_S();

public:
	virtual void Update() override;

public:
	virtual void Fire() override;
};

