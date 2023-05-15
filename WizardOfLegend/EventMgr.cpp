#include "stdafx.h"
#include "EventMgr.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "AI.h"
#include "State.h"

CEventMgr* CEventMgr::m_pInst = nullptr;

CEventMgr::CEventMgr()
{
}


CEventMgr::~CEventMgr()
{
	Release();
}

void CEventMgr::Update()
{
	for (size_t i = 0; i < m_vecDeleteObj.size(); ++i)
	{
		if (nullptr != m_vecDeleteObj[i] && m_vecDeleteObj[i]->Is_Dead())
			CObjMgr::Get_Inst()->Delete_Obj(m_vecDeleteObj[i]);
	}
	m_vecDeleteObj.clear();

	for (size_t i = 0; i < m_vecRemoveObj.size(); ++i)
	{
		if (nullptr != m_vecRemoveObj[i] && !(m_vecRemoveObj[i]->Is_Active()))
			CObjMgr::Get_Inst()->Remove_Obj(m_vecRemoveObj[i]);
	}
	m_vecRemoveObj.clear();

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
		Excute_Event(m_vecEvent[i]);

	m_vecEvent.clear();
}

void CEventMgr::Release()
{
}

const bool CEventMgr::Add_Obj(CObj * _pObj)
{
	if (nullptr == _pObj) return false;

	EVENT eve;
	ZeroMemory(&eve, sizeof(EVENT));

	//_pObj->Init(); // TODO 고려

	eve.lParam = (DWORD_PTR)_pObj;
	eve._eType = EVENT_TYPE::ADD_OBJ;

	Add_Event(eve);

	return true;
}

const bool CEventMgr::Delete_Obj(CObj * _pObj)
{
	if (nullptr == _pObj) return false;

	EVENT eve;
	ZeroMemory(&eve, sizeof(EVENT));

	_pObj->Set_Dead();
	eve.lParam = (DWORD_PTR)_pObj;
	eve._eType = EVENT_TYPE::DEL_OBJ;

	for (auto& iter : m_vecEvent) // 중복 삭제 방지
	{
		if(iter.lParam == eve.lParam)
			return false;
	}

	Add_Event(eve);

	return true;
}

const bool CEventMgr::Remove_Obj(CObj * _pObj)
{
	if (nullptr == _pObj) return false;

	EVENT eve;
	ZeroMemory(&eve, sizeof(EVENT));

	_pObj->Set_Active(false);
	eve.lParam = (DWORD_PTR)_pObj;
	eve._eType = EVENT_TYPE::RMV_OBJ;

	for (auto& iter : m_vecEvent) // 중복 삭제 방지
	{
		if (iter.lParam == eve.lParam)
			return false;
	}

	Add_Event(eve);

	return true;
}

const bool CEventMgr::Change_Scene(const SCENE_TYPE & _eType)
{
	EVENT eve;
	ZeroMemory(&eve, sizeof(EVENT));

	eve.lParam = (DWORD_PTR)_eType;
	eve._eType = EVENT_TYPE::CHANGE_SCENE;
	Add_Event(eve);

	return true;
}

const bool CEventMgr::Change_AIState(AI* _pAI, wstring _pNextStateName)
{
	if (nullptr == _pAI) return false;

	EVENT eve;
	ZeroMemory(&eve, sizeof(EVENT));

	eve._eType = EVENT_TYPE::CHANGE_AI_STATE;
	eve.lParam = (DWORD_PTR)_pAI;
	eve.wParam = _pNextStateName;
	Add_Event(eve);

	return true;
}



void CEventMgr::Excute_Event(EVENT & _eve)
{
	switch (_eve._eType)
	{
	case EVENT_TYPE::ADD_OBJ:
	{
		// lParam : CObj*
		CObjMgr::Get_Inst()->Add_Obj((CObj*)_eve.lParam);
	}
	break;
	case EVENT_TYPE::CHANGE_SCENE:
	{
		// lParam : SCENE_TYPE(ENUM)
		CSceneMgr::Get_Inst()->Change_Scene((SCENE_TYPE)_eve.lParam);

		/*CObjMgr::Get_Inst()->Release();
		m_vecEvent.clear();*/
	}
	break;
	case EVENT_TYPE::RMV_OBJ:
	{
		// lParam : CObj*
		CObj* pObj = (CObj*)_eve.lParam;
		m_vecRemoveObj.push_back(pObj);
	}
	break;
	case EVENT_TYPE::DEL_OBJ:
	{
		// lParam : CObj*
		CObj* pObj = (CObj*)_eve.lParam;
		m_vecDeleteObj.push_back(pObj);
	}
	break;
	case EVENT_TYPE::CHANGE_AI_STATE:
	{
		// lParam : AI* _pAI
		// wParam : wstring* _pNextStateName
		AI* pAI = (AI*)_eve.lParam;

		pAI->Change_State(_eve.wParam);
	}
	default:
		break;
	}
}
