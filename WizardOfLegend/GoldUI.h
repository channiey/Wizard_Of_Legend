#pragma once
#include "UI.h"
class CGoldUI :
	public CUI
{
public:
	CGoldUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType);
	virtual ~CGoldUI();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release();

private:
	virtual void		Load_Tex() override;
	void				Receive_Gold();
private:
	CTexture*			m_pTex;
	VEC2				m_vImgPos;
	VEC2				m_vTextPos;

	int					m_iGold;
};

