#include "stdafx.h"
#include "GhoulLargeAnimator.h"
#include "Cyclops.h"
#include "Rigidbody.h"
#include "Animation.h"
#include "EventMgr.h"
#include "GhoulLarge.h"
#include "SoundMgr.h"

CGhoulLargeAnimator::CGhoulLargeAnimator(CObj* _pOwner)
	: CAnimator(_pOwner)

{
}


CGhoulLargeAnimator::~CGhoulLargeAnimator()
{
}

void CGhoulLargeAnimator::LateUpdate()
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
				static_cast<CGhoulLarge*>(m_pOwner)->Set_State(CLOSE_ENEMY_STATE::CHASE);
			}
			if (m_pCurAni->Get_Name() == L"HURT")
			{
				static_cast<CGhoulLarge*>(m_pOwner)->Set_State(CLOSE_ENEMY_STATE::CHASE);
				m_pOwner->Get_Rigidbody()->Zero_Velocity();
			}
			else if (m_pCurAni->Get_Name() == L"DEAD")
			{
				m_pOwner->Set_Dead();
				CEventMgr::Get_Inst()->Delete_Obj(m_pOwner);
			}
			else if (m_pCurAni->Get_Name() == L"ATTACK")
			{
				static_cast<CGhoulLarge*>(m_pOwner)->Set_State(CLOSE_ENEMY_STATE::CHASE);
			}
		}
	}
}

void CGhoulLargeAnimator::Play(const wstring & _strName, const bool & _bRepeat)
{
	CAnimation* pAni = Find_Ani(_strName);

	if (nullptr == pAni) return;

	m_pCurAni = pAni;
	m_bRepeat = _bRepeat;

	if (m_pCurAni->Is_Finish())
		m_pCurAni->Init_Frame();
}