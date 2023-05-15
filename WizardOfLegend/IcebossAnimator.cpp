#include "stdafx.h"
#include "IcebossAnimator.h"
#include "Animation.h"
#include "IceBoss.h"
#include "CameraMgr.h"
#include "EventMgr.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Transform.h"
#include "BossStageMgr.h"
#include "SoundMgr.h"
CIcebossAnimator::CIcebossAnimator(CObj* _pOwner)
	: CAnimator(_pOwner)
{
}


CIcebossAnimator::~CIcebossAnimator()
{
}

void CIcebossAnimator::LateUpdate()
{
	if (!m_bActive) return;

	if (nullptr != m_pCurAni)
	{
		m_pCurAni->LateUpdate();

		if (m_bRepeat && m_pCurAni->Is_Finish())
			m_pCurAni->Init_Frame();
		else if (!m_bRepeat && m_pCurAni->Is_Finish())
		{
			if (L"SPAWN" == m_pCurAni->Get_Name())
			{
				Play(L"IDLE", true);
				static_cast<CIceBoss*>(m_pOwner)->Set_Is_Spawn(true);
				static_cast<CIceBoss*>(m_pOwner)->Set_UI_Active(true);
				CCameraMgr::Get_Inst()->Shake_Camera(1.5f, 5.f);
				m_pOwner->Set_RenderMag(3.5f);

				CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
				CSoundMgr::Get_Inst()->PlaySound(L"IceBossLaugh.wav", SOUND_BOSS, 1.f);

			}
			else if (L"DEAD" == m_pCurAni->Get_Name())
			{

				CObj* pPlayer = CObjMgr::Get_Inst()->Get_Player();

			/*	CCameraMgr::Get_Inst()->Set_LookAt(CCameraMgr::Get_Inst()->Get_RealPos(pPlayer->Get_Transform()->Get_Pos()), 1.f);
				CCameraMgr::Get_Inst()->Set_Target(pPlayer);*/


				CBossStageMgr::Get_Inst()->Out_Scene();
				CEventMgr::Get_Inst()->Delete_Obj(m_pOwner);
			}

			if (nullptr != m_pReservedAni)
			{
				m_pCurAni->Init_Frame();
				m_pCurAni = m_pReservedAni;
				m_bRepeat = m_bReservedRepeat;
				m_pReservedAni = nullptr;
			}
		}
	}
}

void CIcebossAnimator::Play(const wstring & _strName, const bool & _bRepeat)
{
	CAnimation* pAni = Find_Ani(_strName);

	if (nullptr == pAni) return;

	m_pCurAni = pAni;
	m_bRepeat = _bRepeat;

	if (m_pCurAni->Is_Finish())
		m_pCurAni->Init_Frame();
}
