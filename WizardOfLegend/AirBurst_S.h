#pragma once
#include "Skill.h"

class CAirBurst_P;
class CObj;

class CAirBurst_S :
	public CSkill
{
public:
	CAirBurst_S(CObj* _pObj, const float& _fCoolTime, const float& _fDamage);
	virtual ~CAirBurst_S();

public:
	virtual void Init() override;
	virtual void  Update() override;
	void Release();

public:
	virtual void Fire() override;

private:
	CAirBurst_P* pProj; 
};

