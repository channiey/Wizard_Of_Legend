#include "stdafx.h"
#include "WindSlashAnimator.h"
#include "Animation.h"
#include "WindSlash_P.h"

CWindSlashAnimator::CWindSlashAnimator(CObj* _pOwner)
	: CAnimator(_pOwner)
{
}


CWindSlashAnimator::~CWindSlashAnimator()
{
}

void CWindSlashAnimator::LateUpdate()
{
	if (!m_bActive) return;

	if (nullptr != m_pCurAni)
	{
		m_pCurAni->LateUpdate();

		if (m_bRepeat && m_pCurAni->Is_Finish())
			m_pCurAni->Init_Frame();
		else if (!m_bRepeat && m_pCurAni->Is_Finish())
		{
			if (static_cast<CWindSlash_P*>(m_pOwner)->Is_On())
			{
				static_cast<CWindSlash_P*>(m_pOwner)->Return();
				m_pCurAni->Init_Frame();
			}
		}
	}
}

void CWindSlashAnimator::Play(const wstring & _strName, const bool & _bRepeat)
{
	CAnimation* pAni = Find_Ani(_strName);

	if (nullptr == pAni) return;

	m_pCurAni = pAni;
	m_bRepeat = _bRepeat;
}
