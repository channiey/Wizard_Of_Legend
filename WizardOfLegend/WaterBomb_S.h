#pragma once
#include "Skill.h"
class CWaterBomb_S :
	public CSkill
{
public:
	CWaterBomb_S(CObj* _pObj, const float& _fCoolTime, const float& _fDamage);
	virtual ~CWaterBomb_S();

public:
	virtual void	Update()override;

public:
	virtual void	Fire() override;
	void			Start(); // 사용할 개수 및 필요한 변수를 세팅한다

	bool			m_bAttack;

	int				m_iFireCurCount;
	int				m_iFireMaxCount;

	float			m_fAcc;
};

