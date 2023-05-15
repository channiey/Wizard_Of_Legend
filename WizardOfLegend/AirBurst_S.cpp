#include "stdafx.h"
#include "AirBurst_S.h"
#include "AirBurst_P.h"
#include "EventMgr.h"
#include "Transform.h"
#include "SoundMgr.h"

CAirBurst_S::CAirBurst_S(CObj* _pObj, const float& _fCoolTime, const float& _fDamage)
	: CSkill(_pObj, _fCoolTime, _fDamage)
	, pProj(nullptr)
{
}


CAirBurst_S::~CAirBurst_S()
{
}

void CAirBurst_S::Init()
{
	pProj = new CAirBurst_P(OBJ_TYPE::PROJECTILE);
	pProj->Init();
	pProj->Set_Owner(m_pOwner);
	pProj->Set_AD(m_fDamage);
	pProj->Get_Transform()->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());
	CEventMgr::Get_Inst()->Add_Obj(pProj);
}

void CAirBurst_S::Update()
{
	CSkill::Update();
}

void CAirBurst_S::Release()
{
}

void CAirBurst_S::Fire()
{
	if (m_bCool) return;
	/*CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER_MOVE);
	CSoundMgr::Get_Inst()->PlaySound(L"AirJet.wav", SOUND_PLAYER_MOVE, 0.4f);*/

	pProj->On();
}
