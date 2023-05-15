#pragma once
#include "Scene.h"
class CResultScene :
	public CScene
{
public:
	CResultScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos);
	virtual ~CResultScene();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;

public:
	virtual void		Enter() override;
	virtual void		Exit() override;

private:
	Graphics			m_Graphics;

	float				m_fPlayTime;

	bool				m_bOn[5];

	bool				m_bAllOn;
	bool				m_bFlash;
};

