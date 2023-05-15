#include "stdafx.h"
#include "WindSlash_S.h"
#include "WindSlash_P.h"
#include "EventMgr.h"
#include "Transform.h"
#include "SoundMgr.h"
#include "ObjPool.h"

CWindSlash_S::CWindSlash_S(CObj* _pObj, const float& _fCoolTime, const float& _fDamage)
	: CSkill(_pObj, _fCoolTime, _fDamage)
	, pProj(nullptr)
{
}


CWindSlash_S::~CWindSlash_S()
{
	Release();
}

void CWindSlash_S::Init()
{
	/*pProj = new CWindSlash_P(OBJ_TYPE::PROJECTILE);
	pProj->Init();
	pProj->Set_Owner(m_pOwner);
	pProj->Set_AD(m_fDamage);
	pProj->Get_Transform()->Set_Pos(m_pOwner->Get_Transform()->Get_Pos());
	CEventMgr::Get_Inst()->Add_Obj(pProj);*/
}

void CWindSlash_S::Update()
{
	CSkill::Update();
}

void CWindSlash_S::Release()
{

}

void CWindSlash_S::Fire()
{
	if (m_bCool) return;
	/*
	CWaterBomb_P* pProj = static_cast<CWaterBomb_P*>(CObjPool<CWaterBomb_P>::Get_Obj());

	pProj->Set_Owner(m_pOwner);

	if (nullptr == pProj) return;

	pProj->Reset(pProj);
	
	
	*/

	CWindSlash_P* pProj = static_cast<CWindSlash_P*>(CObjPool<CWindSlash_P>::Get_Obj());

	if (nullptr == pProj) return;

	pProj->Set_Owner(m_pOwner);

	pProj->On();

	CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
	CSoundMgr::Get_Inst()->PlaySound(L"IceSwordSwing.wav", SOUND_PLAYER, 1.f);

}
