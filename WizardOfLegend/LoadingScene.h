#pragma once
#include "Scene.h"
class CLoadingScene :
	public CScene
{
public:
	CLoadingScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos);
	virtual ~CLoadingScene();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;

public:
	virtual void Enter() override;
	virtual void Exit() override;

private:
	float				m_fLoadingTime;
	float				m_fAcc;
	Graphics			m_Graphics;
};

