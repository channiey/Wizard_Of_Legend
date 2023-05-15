#pragma once
#include "Component.h"

class CObj;

class CTransform : public CComponent
{
public:
	CTransform(CObj * _pOwner);
	virtual ~CTransform();

public:
	virtual void Init() override {}
	virtual void LateUpdate() override {}
	virtual void Render(HDC _hDC) override {}

public:
	const VEC2&		Get_Pos() const			{ return m_vPos; }
	const VEC2&		Get_Look() const		{ return m_vLook; }
	const VEC2&		Get_Scale() const		{ return m_vScale; }
	const float&	Get_Theta() const		{ return m_fTheta; }

	void Set_Pos	(const VEC2& _vPos)		{ m_vPos	= _vPos; }
	void Set_Pos_X	(const float& _fX)		{ m_vPos.fX = _fX; }
	void Set_Pos_Y	(const float& _fY)		{ m_vPos.fY = _fY; }
	void Set_Theta(const float& _fTheta)	{ m_fTheta = _fTheta; }

	void Add_Pos	(const VEC2& _vPos)		{ m_vPos	+= _vPos; }
	void Add_Pos_X	(const float& _fX)		{ m_vPos.fX += _fX; }
	void Add_Pos_Y	(const float& _fY)		{ m_vPos.fY += _fY; }

	void Set_Scale	(const VEC2& _vScale)	{ m_vScale	= _vScale; }
	void Set_Look(const VEC2& _vLook)		{ m_vLook = _vLook; }

private:
	VEC2 m_vLook;
	VEC2 m_vPos;
	VEC2 m_vScale;
	float m_fTheta;
};

