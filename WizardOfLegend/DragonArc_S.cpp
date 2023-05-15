#include "stdafx.h"
#include "DragonArc_S.h"
#include "DragonArc_P.h"
#include "MathLib.h"
#include "Obj.h"
#include "CameraMgr.h"
#include "InputMgr.h"
#include "EventMgr.h"
#include "Transform.h"
#include "ObjPool.h"
#include "SoundMgr.h"

CDragonArc_S::CDragonArc_S(CObj* _pObj, const float& _fCoolTime, const float& _fDamage)
	: CSkill(_pObj, _fCoolTime, _fDamage)
{
}


CDragonArc_S::~CDragonArc_S()
{
}

void CDragonArc_S::Update()
{
	CSkill::Update();
}

void CDragonArc_S::Fire()
{
	if (m_bCool) return;
	if (!m_bActive) return;

	/*CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
	CSoundMgr::Get_Inst()->PlaySound(L"StandardHeavySwing.wav", SOUND_PLAYER, 0.7f);*/


	CDragonArc_P* pPorj = static_cast<CDragonArc_P*>(CObjPool<CDragonArc_P>::Get_Obj());

	if (nullptr == pPorj) return;

	float fTheta = CMathLib::Get_Angle(m_pOwner->Get_Transform()->Get_Pos(), CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y }));
	
	pPorj->Reset(m_pOwner, m_pOwner->Get_Transform()->Get_Pos(), m_fDamage, fTheta);

}
