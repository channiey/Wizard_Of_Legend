#include "stdafx.h"
#include "IdleState_IceBoss.h"
#include "ObjMgr.h"
#include "EventMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
#include "Enemy.h"
#include "Animator.h"
#include "SoundMgr.h"

CIdleState_IceBoss::CIdleState_IceBoss()
	: CState(L"IDLE")
{
}

CIdleState_IceBoss::~CIdleState_IceBoss()
{
}

void CIdleState_IceBoss::Enter()
{
	m_fDuration = 1.5f;

	Get_Owner()->Get_Animator()->Play(L"IDLE", true);

	CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);

	if (rand() % 2 == 1)
	{
		CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
		CSoundMgr::Get_Inst()->PlaySound(L"IceBossLaugh.wav", SOUND_BOSS, 1.f);
	}
	else
	{
		CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
		CSoundMgr::Get_Inst()->PlaySound(L"IceBossLaughAlt.wav", SOUND_BOSS, 1.f);
	}
}

void CIdleState_IceBoss::Exit()
{
}

void CIdleState_IceBoss::Update()
{
	m_fAcc += DT;

	if (m_fAcc >= m_fDuration)
	{
		CEventMgr::Get_Inst()->Change_AIState(Get_AI(), L"ATTACK1");
		m_fAcc = 0.f;
	}	
}
