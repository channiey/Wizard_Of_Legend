#include "stdafx.h"
#include "AI.h"
#include "State.h"
#include "ObjMgr.h"
AI::AI(CEnemy* _pOwner)
	: m_pOwner(_pOwner)
	, m_pCurState(nullptr)
{
}

AI::~AI()
{
	Release();
}

void AI::Update()
{
	if (nullptr != m_pCurState)
		m_pCurState->Update();
}

void AI::Render(HDC _hDC)
{
	if (nullptr != m_pCurState)
		m_pCurState->Render(_hDC);
}

void AI::Release()
{
	Safe_Delete_Map(m_mapState);
}


const bool AI::Set_CurState(const wstring & _strName)
{
	CState* pState = Get_State(_strName);

	if (nullptr == pState) return false;

	m_pCurState = pState;

	return true;
}

const bool AI::Add_State(CState * _pState)
{
	if (nullptr == _pState) return false;

	CState* pState = Get_State(_pState->Get_Name());
	
	if (nullptr != pState) return false;

	m_mapState.insert({ _pState->Get_Name(), _pState });
	_pState->Set_AI(this);

	return true;
}

const bool AI::Change_State(const wstring & _strNextStateName)
{
	CState* pNextState = Get_State(_strNextStateName);

	if (m_pCurState == pNextState)
		return false;

	m_pCurState->Exit();

	m_pCurState = pNextState;

	m_pCurState->Enter();

	return true;
}

CState* AI::Get_State(const wstring & _strName)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_strName);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}
