#include "stdafx.h"
#include "PlayerAnimator.h"
#include "Animation.h"
#include "Player.h"
#include "Obj.h"
#include "Rigidbody.h"
#include "SoundMgr.h"

CPlayerAnimator::CPlayerAnimator(CObj* _pOwner)
	: CAnimator(_pOwner)
{
}


CPlayerAnimator::~CPlayerAnimator()
{
}

// IDLE, RUN, DASH, CHARGE, ATTACK, HURT, DEAD

void CPlayerAnimator::LateUpdate()
{
	if (!m_bActive) return;

	if (nullptr != m_pCurAni)
	{
		m_pCurAni->LateUpdate();

		if (m_bRepeat && m_pCurAni->Is_Finish() && L"PORTAL" == m_pCurAni->Get_Name())
		{
			Play(L"IDLE", true);
			static_cast<CPlayer*>(m_pOwner)->Set_PortalAniBool(false);

			/*CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER);
			CSoundMgr::Get_Inst()->PlaySound(L"Teleport.wav", SOUND_PLAYER, 1.f);*/
			
		}
		else if (m_bRepeat && m_pCurAni->Is_Finish())
		{
			m_pCurAni->Init_Frame();
		}
		else if (!m_bRepeat && m_pCurAni->Is_Finish())
		{
			if (m_pOwner->Is_Lock())
				m_pOwner->Set_Lock(false); // 움직임 락 해제
			
			m_pOwner->Get_Rigidbody()->Zero_Velocity(); // 대시 종료시 속도 초기화
			
			if (nullptr != m_pReservedAni)
			{
				m_pCurAni->Init_Frame();
				m_pCurAni = m_pReservedAni;
				m_bRepeat = m_bReservedRepeat;
				m_pReservedAni = nullptr;

				if (m_pCurAni->Get_Name() == L"DASH" || m_pCurAni->Get_Name() == L"ATTACK_A" || m_pCurAni->Get_Name() == L"ATTACK_B" || m_pCurAni->Get_Name() == L"HURT")
				{
					m_pOwner->Set_Lock(true);
				}
			}
		}
	}
}

void CPlayerAnimator::Play(const wstring& _strName, const bool& _bRepeat)
{
	CAnimation* pAni = Find_Ani(_strName);

	if (nullptr == pAni) return;

	if (nullptr == m_pCurAni) // 처음 1회 실행
	{
		m_pCurAni = pAni;
		m_bRepeat = _bRepeat;
	}

	if (m_pCurAni->Get_Name() == L"DASH")
	{
		if (!m_pCurAni->Is_Finish())
		{
			m_pReservedAni = pAni;
			m_bReservedRepeat = _bRepeat;
			return;
		}
	}
	else if (m_pCurAni->Get_Name() == L"ATTACK_A" || m_pCurAni->Get_Name() == L"ATTACK_B")
	{
		if (_strName == L"ATTACK_A" || _strName == L"ATTACK_B")
		{
			m_pCurAni = pAni;
			m_bRepeat = _bRepeat;
			m_pOwner->Get_Rigidbody()->Zero_Velocity();
			return;
		}
		else
		{
			m_pReservedAni = pAni;
			m_bReservedRepeat = _bRepeat;
			return;
		}
	}
	else if (m_pCurAni->Get_Name() == L"HURT")
	{
		if (!m_pCurAni->Is_Finish())
		{
			m_pReservedAni = pAni;
			m_bReservedRepeat = _bRepeat;
			return;
		}
	}
	else if (_strName == L"PORTAL")
	{
		m_pCurAni->Init_Frame();
		m_pCurAni = pAni;
		m_bRepeat = _bRepeat;
		return;
	}

	if (pAni->Get_Name() == L"DASH" || pAni->Get_Name() == L"ATTACK_A" || pAni->Get_Name() == L"ATTACK_B" || pAni->Get_Name() == L"HURT")
	{
		m_pOwner->Set_Lock(true);
	}
	m_pCurAni = pAni;
	m_bRepeat = _bRepeat;

	if (m_pCurAni->Is_Finish())
		m_pCurAni->Init_Frame();
}
