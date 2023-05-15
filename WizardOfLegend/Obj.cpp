#include "stdafx.h"
#include "Obj.h"
#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "Animator.h"
#include "Rigidbody.h"

CObj::CObj(const OBJ_TYPE& _eType)
	: m_pTransform(nullptr)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidbody(nullptr)
	, m_bDead(false)
	, m_eObjType(_eType)
	, m_fCurTume(0.f)
	, m_fMaxTime(0.f)
	, m_fSpeed(DF_SPEED)
	, m_fDashSpeed(DF_DASHSPEED)
	, m_fMaxHp(DF_HP)
	, m_fCurHP(DF_HP)
	, m_fAD(DF_AD)
	, m_fKonckBackTime(DF_KNOCKBACK_TIME)
	, m_bInvincible(false)
	, m_bLock(false)
	, m_fRenderMag(1.f)
	, m_eRenderOrder(RENDER_ORDER::GAMEOBJECT)
	, m_bActive(true)
	, m_iGold(0)
{
}


CObj::CObj(const CObj & _rhs)
	: m_pTransform(nullptr)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidbody(nullptr)
	, m_bDead(_rhs.m_bDead)
	, m_eObjType(_rhs.m_eObjType)
	, m_fCurTume(_rhs.m_fCurTume)
	, m_fMaxTime(_rhs.m_fMaxTime)
	, m_fSpeed(_rhs.m_fSpeed)
	, m_fDashSpeed(_rhs.m_fDashSpeed)
	, m_fMaxHp(_rhs.m_fMaxHp)
	, m_fCurHP(_rhs.m_fCurHP)
	, m_fAD(_rhs.m_fAD)
	, m_fKonckBackTime(_rhs.m_fKonckBackTime)
	, m_bInvincible(_rhs.m_bInvincible)
	, m_bLock(_rhs.m_bLock)
	, m_fRenderMag(_rhs.m_fRenderMag)
	, m_eRenderOrder(_rhs.m_eRenderOrder)
	, m_bActive(_rhs.m_bActive)
	, m_iGold(_rhs.m_iGold)
{
	if (_rhs.m_pTransform)
	{
		m_pTransform = new CTransform(*_rhs.m_pTransform);
		m_pTransform->Set_Owner(this);
	}
	if (_rhs.m_pCollider)
	{
		m_pCollider = new CCollider(*_rhs.m_pCollider);
		m_pCollider->Set_Owner(this);
	}
	if (_rhs.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_rhs.m_pAnimator);
		m_pAnimator->Set_Owner(this);
	}
}


CObj::~CObj()
{
	Release();
}

void CObj::Release()
{
	for (auto& iter : m_listComponent)
		Safe_Delete(iter);

	m_listComponent.clear();
}

const bool CObj::Create_Transform()
{
	if (m_pTransform) return false;  

	m_pTransform = new CTransform(this);

	m_listComponent.push_back(m_pTransform); 

	return true; 
}	
const bool CObj::Create_Collider()
{
	if (m_pCollider) return false;  

	m_pCollider = new CCollider(this); 

	m_listComponent.push_back(m_pCollider); 

	return true;
}

const bool CObj::Create_Animator()
{
	if (m_pAnimator) return false;  
	
	m_pAnimator = new CAnimator(this); 
	
	m_listComponent.push_back(m_pAnimator); 
	
	return true; 
}

const bool CObj::Create_Rigidbody()
{
	if (m_pRigidbody) return false;

	m_pRigidbody = new CRigidbody(this);

	m_listComponent.push_back(m_pRigidbody);

	return true; ;
}

void CObj::Component_Render(HDC _hDC)
{
	for (auto& iter : m_listComponent)
	{
		if (nullptr != iter)
			iter->Render(_hDC);
	}
}

void CObj::Component_LateUpdate()
{
	for (auto& iter : m_listComponent)
	{
		if (nullptr != iter)
			iter->LateUpdate();
	}
}

