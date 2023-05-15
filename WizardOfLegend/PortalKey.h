#pragma once
#include "UI.h"
class CPortalKey :
	public CUI
{
public:
	CPortalKey(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType);
	virtual ~CPortalKey();

public:
	virtual const bool	Init() override;
	virtual void		Update() {}
	virtual void		LateUpdate() {}
	virtual void		Render(HDC _hDC) override;
	void				Release() {}

private:
	virtual void		Load_Tex() override;
	
private:
	CTexture*			m_pTex;
	VEC2				m_vImgPos;
};

