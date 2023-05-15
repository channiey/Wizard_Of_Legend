#include "stdafx.h"
#include "Skill.h"
#include "TimeMgr.h"

CSkill::CSkill(CObj* _pObj, const float& _fCoolTime, const float& _fDamage)
	: m_fAccTime(0.f)
	, m_bActive(true)
	, m_fCoolTime(_fCoolTime)
	, m_fDamage(_fDamage)
	, m_pOwner(_pObj)
	, m_fCoolAcc(0.f)
	, m_fCoolPer(0.f)
	, m_bCool(false)
{
}

CSkill::~CSkill()
{
	
}

void CSkill::Update()
{
	if (!m_bActive) return;

	if (m_bCool)
	{
		m_fCoolAcc += DT;

		m_fCoolPer = m_fCoolAcc / m_fCoolTime;
		
		if (1 < m_fCoolPer)
			m_fCoolPer = 1;

		
		if (m_fCoolAcc > m_fCoolTime)
		{
			m_bCool = false;
			m_fCoolAcc = 0.f;
			m_fCoolPer = 0.f;
		}
	}
}



const bool CSkill::Is_Cool()
{
	//if (m_bCool)
	//	cout << "COOL\n"; 

	return m_bCool;
}

void CSkill::Start_Count()
{
	if (m_bCool) return;

	if (!m_bCool) 
		m_bCool = true;

}
