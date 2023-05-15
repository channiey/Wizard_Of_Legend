#include "stdafx.h"
#include "ExplodingFireball_S.h"
#include "ExplodingFireball_P.h"
#include "EventMgr.h"
#include "MathLib.h"
#include "Transform.h"
#include "InputMgr.h"
#include "CameraMgr.h"
#include "Animation.h"
#include "Texture.h"
#include "Animator.h"
#include "ObjPool.h"
#include "SoundMgr.h"

CExplodingFireball_S::CExplodingFireball_S(CObj* _pObj, const float& _fCoolTime, const float& _fDamage)
	: CSkill(_pObj, _fCoolTime, _fDamage)
{
}


CExplodingFireball_S::~CExplodingFireball_S()
{
}

void CExplodingFireball_S::Update()
{
	CSkill::Update();
}

void CExplodingFireball_S::Fire()
{
	if (m_bCool) return;
	if (!m_bActive) return;

	/*CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
	CSoundMgr::Get_Inst()->PlaySound(L"StandardHeavySwing.wav", SOUND_PLAYER, 0.7f);*/


	ExplodingFireball_P* pProj = static_cast<ExplodingFireball_P*>(CObjPool<ExplodingFireball_P>::Get_Obj());

	if (nullptr == pProj) return;

	float fTheta = CMathLib::Get_Angle(m_pOwner->Get_Transform()->Get_Pos(), CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y }));
	
	pProj->Reset(m_pOwner, m_pOwner->Get_Transform()->Get_Pos(), m_fDamage, fTheta);
}
