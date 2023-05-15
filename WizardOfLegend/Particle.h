#pragma once
#include "Obj.h"
class CParticle :
	public CObj
{
public:
	CParticle(const OBJ_TYPE& _eType);
	virtual ~CParticle();

public:
	virtual void	Reset(CObj* _pOwner) PURE;
	virtual void	Return() PURE;


protected:
	CObj* m_pOwner;
};

