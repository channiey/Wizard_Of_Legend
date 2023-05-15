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
	float		m_fChargeTime;	// �� �ʵ��� ��������
	float		m_fFireTime;	// �߻��� ���� ������Ʈ ��ȯ �� ���ð�
	bool		m_bCharge;
};

// 360�� ���� ������ // IceKatana