#pragma once
#include "Obj.h"
class CEffect :
	public CObj
{
public:
	explicit CEffect(const OBJ_TYPE& _eType);
	explicit CEffect(const OBJ_TYPE& _eType, CObj* _pOwner);
	virtual ~CEffect();

public:
	const bool Is_On() const { return m_bOn; }
	CObj*				Get_Owner() const { return m_pOwner; }
	const bool			Set_Onwer(CObj* _pObj) { m_pOwner = _pObj; }

public:
	virtual void On(const VEC2& _vPos) PURE;
	virtual void Off(const VEC2& _vPos) PURE;
private:
	bool m_bOn;
	CObj*	m_pOwner;
};

