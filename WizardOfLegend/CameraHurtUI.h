#pragma once
#include "UI.h"
class CCameraHurtUI :
	public CUI
{
public:
	CCameraHurtUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType);
	~CCameraHurtUI();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release();

private:
	virtual void		Load_Tex() override;

private:
	CTexture*			m_pTex;
};

