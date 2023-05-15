#pragma once

class CObj;
class AI;

class CEventMgr
{
	SINGLETON(CEventMgr)

private:
	CEventMgr();
	~CEventMgr();

public:
	void				Update();
	void				Release();

public:
	const bool			Add_Obj(CObj* _pObj);
	const bool			Delete_Obj(CObj* _pObj); // 삭제(메모리 해제)할 오브젝트 처리
	const bool			Remove_Obj(CObj* _pObj); // 풀에 반환하는 오브젝트 처리
	const bool			Change_Scene(const SCENE_TYPE& _eType);
	const bool			Change_AIState(AI* _pAI, wstring _pNextStateName);

private:
	void				Add_Event(EVENT& _eve) { m_vecEvent.push_back(_eve); }
	void				Excute_Event(EVENT& _eve);

private:
	vector<EVENT>		m_vecEvent;
	vector<CObj*>		m_vecDeleteObj; // 메모리를 해제할 오브젝트들을 모아둔다.
	vector<CObj*>		m_vecRemoveObj; // 풀에 반환될 오브젝트들을 모아둔다.
};

