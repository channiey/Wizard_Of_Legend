#pragma once
#include "State.h"
class CAttackState2_IceBoss :
	public CState
{
public:
	CAttackState2_IceBoss();
	~CAttackState2_IceBoss();

public:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Exit() override;

private:
	void Fire();
	void Aim();
	void Move();

private:
	float		m_fAimTime;		// 몇 초동안 조준할지
	float		m_fFireTime;	// 발사후 다음 스테이트 전환 전 대기시간
	bool		m_bFired;
	bool		m_bAim;

	bool		m_bPlus1;
	bool		m_bPlus2;
};

// 플레이어 조준 했다가 큰 창 날리기 // IceLance
