#pragma once
#include "Component.h"
class CRigidbody :
	public CComponent
{
public:
	CRigidbody(CObj* _pOwner);
	~CRigidbody();

public:
	virtual void	Init() override;
	virtual void	LateUpdate() override;
	virtual void	Render(HDC _hDC) override;

public:
public:
	const VEC2&		Get_Force		() const						{ return m_vForce; }
	const VEC2&		Get_Velocity	() const						{ return m_vVelocity; }
	const VEC2&		Get_Gravity		() const						{ return m_vGravity; }
	const float&	Get_MaxSpeed	() const						{ return m_fMaxSpeed; }
	const float&	Get_Mass		() const						{ return m_fMass; }

	void			Set_Force		(const VEC2& _vForce)			{ m_vForce = _vForce; }
	void			Set_Velocity	(const VEC2& _vVelocity)		{ m_vVelocity = _vVelocity; }
	void			Set_Gravity		(const VEC2& _vGravity)			{ m_vGravity = _vGravity; }
	void			Set_MaxSpeed	(const float& _fMaxSpeed)		{ m_fMaxSpeed = _fMaxSpeed; }
	void			Set_Mass		(const float& _fMass)			{ m_fMass = _fMass; }

public:
	void			Add_Force		(const VEC2& _vForce)			{ m_vForce += _vForce; }
	void			Add_Velocity	(const VEC2& _vVelocity)		{ m_vVelocity += _vVelocity; }

	void			Zero_Force		()								{ m_vForce.Zero(); }
	void			Zero_Velocity	()								{ m_vVelocity.Zero(); }
	void			Zero_Velocity_X	()								{ m_vVelocity.fX = 0.f; }
	void			Zero_Velocity_Y	()								{ m_vVelocity.fY = 0.f; }

private:
	VEC2			m_vForce;
	VEC2			m_vAcc;
	VEC2			m_vVelocity;
	VEC2			m_vGravity;

	float			m_fMass;
	float			m_fMaxSpeed;
};

