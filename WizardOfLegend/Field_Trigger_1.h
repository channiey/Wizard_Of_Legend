#pragma once
#include "SpawnTrigger.h"
class CField_Trigger_1 :
	public CSpawnTrigger
{
public:
	CField_Trigger_1(const OBJ_TYPE& _eType);
	virtual ~CField_Trigger_1();

public:
	virtual const bool Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;
};

