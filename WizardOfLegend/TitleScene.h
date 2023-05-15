#pragma once
#include "Scene.h"

class CTexture;

class CTitleScene : public CScene
{
public:
	explicit CTitleScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos);
	virtual ~CTitleScene();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;

public:
	virtual void		Enter() override;
	virtual void		Exit() override;
private:
	void				Render_Background(HDC _hDC);
	void				Is_Mouse_On();
private:
	CTexture*			m_pTexLogo;

	Graphics			m_Graphics;

	bool				m_bMenuMode;


	float				m_fAcc;
	float				m_fFlashTime;
	float				m_bRender;

	RECT				m_arrRect[5];
	bool				m_arrOn[5];

	bool m_bStart;
	float	m_fStartAcc;
};

