#pragma once
#include "Scene.h"

class CTexture;

class CBoss_Scene :
	public CScene
{
public:
	CBoss_Scene(const SCENE_TYPE& _eType, const VEC2& _vStartPos);
	virtual ~CBoss_Scene();

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

private:
	CTexture*			m_pTexBack;
	Graphics			m_Graphics;
};

