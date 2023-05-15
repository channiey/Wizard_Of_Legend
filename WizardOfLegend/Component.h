#pragma once

class CObj;

class CComponent
{
public:
	explicit CComponent(CObj* _pOwner);
	virtual ~CComponent();

public:
	virtual void			Init()					PURE;
	virtual void			LateUpdate()			PURE;
	virtual void			Render(HDC _hDC)		PURE;

public:
	const COMPONENT_TYPE&	Get_Type() const					{ return m_eType; }
	CObj*					Get_Owner()	const					{ return m_pOwner; }

	const bool				Set_Owner(CObj* _pOwner)			{ if (nullptr == _pOwner) return false;  m_pOwner = _pOwner; return true; }
	void					Set_Active(const bool _bActive)		{ m_bActive = _bActive; }

	const bool				Is_Active() const					{ return m_bActive; }

protected:
	CObj*					m_pOwner;
	bool					m_bActive;
	COMPONENT_TYPE			m_eType;
};

