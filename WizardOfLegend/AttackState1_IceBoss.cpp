#include "stdafx.h"
#include "AttackState1_IceBoss.h"
#include "TimeMgr.h"
#include "Obj.h"
#include "Enemy.h"
#include "Animator.h"
#include "EventMgr.h"
#include "IceKatana_P.h"
#include "Transform.h"
#include "ObjMgr.h"
#include "ObjPool.h"
#include "IceKatana_P.h"
#include "CameraMgr.h"
#include "SoundMgr.h"

CAttackState1_IceBoss::CAttackState1_IceBoss()
	: CState(L"ATTACK1")
{
}


CAttackState1_IceBoss::~CAttackState1_IceBoss()
{
}

void CAttackState1_IceBoss::Enter()
{
	m_fChargeTime = 1.2f;

	m_fFireTime = 2.1f;

	m_bCharge = false;

	//Get_Owner()->Get_Animator()->Play(L"SLASH_CHARGE", false);
}

void CAttackState1_IceBoss::Update()
{
	m_fAcc += DT;
	if (m_fAcc >= m_fChargeTime)
	{
		//Get_Owner()->Get_Animator()->Play(L"SLASH", false);

		m_fAcc = 0.f;
		m_fChargeTime = 100.f;

		m_bCharge = true;

		Fire();
	}
	else
	{
		Move();
	}
	if (m_bCharge && m_fAcc >= m_fFireTime)
	{
		CEventMgr::Get_Inst()->Change_AIState(Get_AI(), L"ATTACK2");

		m_fAcc = 0.f;

	}
}

void CAttackState1_IceBoss::Exit()
{
}

void CAttackState1_IceBoss::Fire()
{
	//IceSwordSwing
	CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
	CSoundMgr::Get_Inst()->PlaySound(L"IceExplode.wav", SOUND_BOSS, 1.f);
	int iRan = rand() % 2;

	if (0 == iRan)
	{
		for (int i = 0; i < 10; ++i)
		{
			CIceKatana_P* pProj = static_cast<CIceKatana_P*>(CObjPool<CIceKatana_P>::Get_Obj());

			if (nullptr == pProj) return;

			float fRadian = 36 * i * PI / 180.f;

			pProj->Reset(Get_Owner(), Get_Owner()->Get_Transform()->Get_Pos(), DF_AD, fRadian);
		}
	}
	else if (1 == iRan)
	{
		for (int i = 0; i < 8; ++i)
		{
			CIceKatana_P* pProj = static_cast<CIceKatana_P*>(CObjPool<CIceKatana_P>::Get_Obj());

			if (nullptr == pProj) return;

			float fRadian = 45 * i * PI / 180.f;

			pProj->Reset(Get_Owner(), Get_Owner()->Get_Transform()->Get_Pos(), DF_AD, fRadian);
		}
	}
}

void CAttackState1_IceBoss::Move()
{
	VEC2 vOwnerPos = Get_Owner()->Get_Transform()->Get_Pos();
	VEC2 vTargetPos = CObjMgr::Get_Inst()->Get_Player()->Get_Transform()->Get_Pos();

	VEC2 vLook = vTargetPos - vOwnerPos;
	vLook.Normalize();

	
	Get_Owner()->Get_Transform()->Add_Pos(vLook * Get_Owner()->Get_Speed() *DT);
}
