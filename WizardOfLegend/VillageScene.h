#pragma once
#include "Scene.h"
class CVillageScene :
	public CScene
{
public:
	CVillageScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos);
	~CVillageScene();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;

public:
	virtual void		Enter() override;
	virtual void		Exit() override;

};

