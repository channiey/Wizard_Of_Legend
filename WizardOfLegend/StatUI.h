#pragma once
#include "UI.h"
class CObj;
class CStatUI :
	public CUI
{
public:
	CStatUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType, CObj* _pOnwer);
	virtual ~CStatUI();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release();

private:
	virtual void		Load_Tex() override;
	void				Calculate_HP();

private:
	CTexture*			m_pTexFormat;
	CTexture*			m_pHP;

	VEC2				m_vFormatPos;
	VEC2				m_vHPPos;

	float				m_fHPper;
	float				m_fHPFull;
	float				m_fHPMag;

	CObj*				m_pOwner;
};

