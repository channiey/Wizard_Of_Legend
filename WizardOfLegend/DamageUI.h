#pragma once
#include "UI.h"
class CDamageUI :
	public CUI
{
public:
	explicit CDamageUI(const OBJ_TYPE& _eType);
	explicit CDamageUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType);
	virtual ~CDamageUI();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release();

public:
	void				Reset(const VEC2& _vPos);
	const int&				Get_Damage() const { return m_iDamage; }

private:
	virtual void		Load_Tex() override {}

private:
	int					m_iDamage;
	float				m_fAcc;
	float				m_fLifeTime;
	float				m_fForce;
};

