#pragma once
#include "UI.h"

class CTexture;

class CWarningUI :
	public CUI
{
public:
	CWarningUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType);
	virtual ~CWarningUI();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release() {}

private:
	virtual void		Load_Tex() override;
	void				Check_Self_Delete();
	void				Flash(); // ¿ÃπÃ¡ˆ ±Ù∫˝¿”

public:
	const float&		Get_LifeTime() const { return m_fLifeTime; }
	virtual void		Set_Active(const bool _bActive) override;


private:
	CTexture*			m_pTex;
	VEC2				m_vImgPos;
	float				m_fLifeTime;
	float				m_fAcc;
	float				m_bLife;
	bool				m_bRender; // ±Ù∫˝¿”¿ª ¿ß«ÿº≠ º≥¡§
};

