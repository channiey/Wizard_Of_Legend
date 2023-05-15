#include "stdafx.h"
#include "Animator.h"
#include "Animation.h"

CAnimator::CAnimator(CObj* _pOwner)
	: CComponent(_pOwner)
	, m_pCurAni(nullptr)
	, m_pReservedAni(nullptr)
	, m_bRepeat(false)
	, m_bReservedRepeat(false)
	, m_bUseDir(true)
{
	m_eType = COMPONENT_TYPE::ANIMATOR;
}


CAnimator::~CAnimator()
{
	Release();
}

void CAnimator::Init()
{
}

void CAnimator::LateUpdate()
{
	if (!m_bActive) return;

	if (nullptr != m_pCurAni)
	{
		m_pCurAni->LateUpdate();

		if (m_bRepeat && m_pCurAni->Is_Finish())
			m_pCurAni->Init_Frame();
		else if(!m_bRepeat && m_pCurAni->Is_Finish())
		{
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

void CAnimator::Render(HDC _hDC)
{
	if (!m_bActive) return;

	if (nullptr != m_pCurAni)
		m_pCurAni->Render(_hDC);
}

void CAnimator::Release()
{
	Safe_Delete_Map(m_mapAni);
}

const bool CAnimator::PushBack_Ani(CAnimation* _pAni)
{
	if (nullptr == _pAni) return false;

	if (nullptr == Find_Ani(_pAni->Get_Name()))
	{
		m_mapAni.insert({ _pAni->Get_Name(), _pAni });
		return true;
	}

	return false;
}

CAnimation * CAnimator::Find_Ani(const wstring & _strName)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAni.find(_strName);

	if (m_mapAni.end() == iter)
		return nullptr;

	return iter->second;
}

void CAnimator::Play(const wstring& _strName, const bool& _bRepeat)
{
	CAnimation* pAni = Find_Ani(_strName);

	if (nullptr == pAni) return;
	
	m_pCurAni = pAni;
	m_bRepeat = _bRepeat;

	if (m_pCurAni->Is_Finish())
		m_pCurAni->Init_Frame();
}
