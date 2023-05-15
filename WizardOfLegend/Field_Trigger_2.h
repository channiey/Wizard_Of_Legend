#pragma once
#include "SpawnTrigger.h"

class CItem;

class CField_Trigger_2 :
	public CSpawnTrigger
{
public:
	CField_Trigger_2(const OBJ_TYPE& _eType);
	virtual ~CField_Trigger_2();

public:
	virtual const bool Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render(HDC _hDC) override;


	virtual const bool	Is_EndLevel()override;
private:
	CItem*						m_pDropItem;
};

