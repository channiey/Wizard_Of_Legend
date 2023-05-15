#include "stdafx.h"
#include "ExplodingFireballAnimator.h"
#include "Animation.h"
#include "EventMgr.h"
#include "Projectile.h"
CExplodingFireballAnimator::CExplodingFireballAnimator(CObj* _pOwner)
	: CAnimator(_pOwner)
{
}


CExplodingFireballAnimator::~CExplodingFireballAnimator()
{
}

void CExplodingFireballAnimator::LateUpdate()
{
	if (!m_bActive) return;

	if (nullptr != m_pCurAni)
	{
		m_pCurAni->LateUpdate();

		if (m_bRepeat && m_pCurAni->Is_Finish())
			m_pCurAni->Init_Frame();
		else if (!m_bRepeat && m_pCurAni->Is_Finish())
		{
			if (m_pCurAni->Get_Name() == L"BOOMB")
			{
				static_cast<CProjectile*>(Get_Owner())->Return();
				m_pCurAni->Init_Frame();
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

void CExplodingFireballAnimator::Play(const wstring & _strName, const bool & _bRepeat)
{
	CAnimation* pAni = Find_Ani(_strName);

	if (nullptr == pAni) return;

	m_pCurAni = pAni;
	m_bRepeat = _bRepeat;
}
