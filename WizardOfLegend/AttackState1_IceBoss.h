#pragma once
#include "State.h"
class CAttackState1_IceBoss :
	public CState
{
public:
	CAttackState1_IceBoss();
	~CAttackState1_IceBoss();

public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Exit() override;

private:
	void Fire();
	void Move();

private:
	float		m_fChargeTime;	// 몇 초동안 충전할지
	float		m_fFireTime;	// 발사후 다음 스테이트 전환 전 대기시간
	bool		m_bCharge;
};

// 360도 얼음 날리기 // IceKatana