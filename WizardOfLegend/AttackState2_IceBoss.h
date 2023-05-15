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
	float		m_fAimTime;		// �� �ʵ��� ��������
	float		m_fFireTime;	// �߻��� ���� ������Ʈ ��ȯ �� ���ð�
	bool		m_bFired;
	bool		m_bAim;

	bool		m_bPlus1;
	bool		m_bPlus2;
};

// �÷��̾� ���� �ߴٰ� ū â ������ // IceLance
