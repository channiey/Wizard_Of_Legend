#pragma once
#include "State.h"
class CIdleState_IceBoss :
	public CState
{
public:
	CIdleState_IceBoss();
	virtual ~CIdleState_IceBoss();

public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;

};

