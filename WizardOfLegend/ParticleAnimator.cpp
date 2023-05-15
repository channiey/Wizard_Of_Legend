#include "stdafx.h"
#include "ParticleAnimator.h"
#include "Animation.h"
#include "Particle.h"
CParticleAnimator::CParticleAnimator(CObj* _pOwner)
	: CAnimator(_pOwner)
{
}


CParticleAnimator::~CParticleAnimator()
{
}

void CParticleAnimator::LateUpdate()
{
	if (!m_bActive) return;

	if (nullptr != m_pCurAni)
	{
		m_pCurAni->LateUpdate();

		if (m_bRepeat && m_pCurAni->Is_Finish())
			m_pCurAni->Init_Frame();
		else if (!m_bRepeat && m_pCurAni->Is_Finish())
		{
			if (L"MOVE" == m_pCurAni->Get_Name())
			{
				static_cast<CParticle*>(m_pOwner)->Return();
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

void CParticleAnimator::Play(const wstring & _strName, const bool & _bRepeat)
{
	CAnimation* pAni = Find_Ani(_strName);

	if (nullptr == pAni) return;

	m_pCurAni = pAni;
	m_bRepeat = _bRepeat;

	if (m_pCurAni->Is_Finish())
		m_pCurAni->Init_Frame();
}
