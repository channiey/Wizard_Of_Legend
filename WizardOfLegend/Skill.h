#pragma once

class CObj;

class CSkill
{
public:
	explicit CSkill(CObj* _pObj, const float& _fCoolTime, const float& _fDamage);
	virtual ~CSkill();

public:
	virtual void	Init() {}
	virtual void	Update();

public:
	virtual void	Fire() PURE;

public:
	void			Set_CoolTime(const float& _fCoolTime) { m_fCoolTime = _fCoolTime; }
	const float		Get_CoolPer() const { return m_fCoolPer; }

	const bool		Is_Cool();

	void			Start_Count();

protected:
	float			m_fCoolTime;
	float			m_fAccTime;
	float			m_fDamage;
	bool			m_bActive;
	CObj*			m_pOwner;





	// CoolTime

	bool			m_bCool; // 현재 쿨타임이 돌고 있는지
	float			m_fCoolAcc; // 스킬 쿨타임을 셀 변수
	float			m_fCoolPer; // 스킬 쿨타임 퍼센테이지

};

