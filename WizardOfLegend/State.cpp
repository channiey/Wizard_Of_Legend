#include "stdafx.h"
#include "State.h"
#include "AI.h"
CState::CState(const wstring& _strName)
	: m_pAI(nullptr)
	, m_strName(_strName)
	, m_fDuration(0.f)
	, m_fAcc(0.f)
{
}

CState::~CState()
{
}

CEnemy * CState::Get_Owner() const
{
	return m_pAI->Get_Owner();
}