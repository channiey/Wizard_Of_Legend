#pragma once
#include "State.h"
class CAttackState3_IceBoss :
	public CState
{
public:
	CAttackState3_IceBoss();
	~CAttackState3_IceBoss();

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

	bool		m_bPlus;

	vector<VEC2>		m_vStartPos;
};

// 화면에 수직 수평 레이저 그린 다음 라인 따라 얼음창 발사 // IceSword

