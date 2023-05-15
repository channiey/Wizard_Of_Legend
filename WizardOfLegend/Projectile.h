#pragma once
#include "Obj.h"
class CProjectile :
	public CObj
{
public:
	explicit CProjectile(const OBJ_TYPE& _eType);
	explicit CProjectile(const OBJ_TYPE& _eType, CObj* _pOwner);
	virtual ~CProjectile();


public:
	CObj*			Get_Owner() const { return m_pOwner; }

	const bool		Set_Owner(CObj* _pObj) { if (nullptr == _pObj) return false; m_pOwner = _pObj; return true; }
	virtual void	Return() PURE;

	const bool	Create_Particle();

protected:
	virtual void	Move();
	const bool		Out_Of_Screen();

protected:
	CObj*			m_pOwner;
	bool			m_bMove;

	float			m_fAccParticle;
	float			m_fParticleCreateTerm;

	
};

