#include "stdafx.h"
#include "CyclopsAnimator.h"
#include "Cyclops.h"
#include "Rigidbody.h"
#include "Animation.h"
#include "EventMgr.h"
#include "SoundMgr.h"

CCyclopsAnimator::CCyclopsAnimator(CObj* _pOwner)
	: CAnimator(_pOwner)
{
}


CCyclopsAnimator::~CCyclopsAnimator()
{
}

void CCyclopsAnimator::LateUpdate()
{
	if (!m_bActive) return;

	if (nullptr != m_pCurAni)
	{
		m_pCurAni->LateUpdate();

		if (m_bRepeat && m_pCurAni->Is_Finish())
			m_pCurAni->Init_Frame();
		else if (!m_bRepeat && m_pCurAni->Is_Finish())
		{
			if (m_pCurAni->Get_Name() == L"SPAWN")
			{
				static_cast<CCyclops*>(m_pOwner)->Set_State(CLOSE_ENEMY_STATE::CHASE);
			
				/*CSoundMgr::Get_Inst()->StopSound(SOUND_CYCLOPS);
				CSoundMgr::Get_Inst()->PlaySound(L"EnemyCast.wav", SOUND_CYCLOPS, 1.f);
*/
			
			}
			if (m_pCurAni->Get_Name() == L"HURT")
			{
				static_cast<CCyclops*>(m_pOwner)->Set_State(CLOSE_ENEMY_STATE::CHASE);
				m_pOwner->Get_Rigidbody()->Zero_Velocity();
			}
			else if (m_pCurAni->Get_Name() == L"DEAD")
			{
				m_pOwner->Set_Dead();
				CEventMgr::Get_Inst()->Delete_Obj(m_pOwner);
			}
			else if (m_pCurAni->Get_Name() == L"ATTACK")
			{
				static_cast<CCyclops*>(m_pOwner)->Set_State(CLOSE_ENEMY_STATE::CHASE);
			}
		}
	}
}

void CCyclopsAnimator::Play(const wstring & _strName, const bool & _bRepeat)
{
	CAnimation* pAni = Find_Ani(_strName);

	if (nullptr == pAni) return;

	m_pCurAni = pAni;
	m_bRepeat = _bRepeat;
	 
	if (m_pCurAni->Is_Finish())
		m_pCurAni->Init_Frame();
}
