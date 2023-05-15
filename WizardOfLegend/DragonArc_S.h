#pragma once
#include "Skill.h"
class CDragonArc_S : public CSkill
{
public:
	CDragonArc_S(CObj* _pObj, const float& _fCoolTime, const float& _fDamage);
	virtual ~CDragonArc_S();

public:
	virtual void Update() override;

public:
	virtual void Fire() override;
};

