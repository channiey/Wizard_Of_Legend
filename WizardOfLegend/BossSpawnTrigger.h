#pragma once
#include "SpawnTrigger.h"

class CBossSpawnTrigger :
	public CSpawnTrigger
{
public:
	explicit CBossSpawnTrigger(const OBJ_TYPE& _eType);
	virtual ~CBossSpawnTrigger();

public:
	virtual const bool Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;

private:
	CObj* pBossUI;
};

