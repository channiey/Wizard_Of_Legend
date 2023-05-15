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
	const bool			Delete_Obj(CObj* _pObj); // ����(�޸� ����)�� ������Ʈ ó��
	const bool			Remove_Obj(CObj* _pObj); // Ǯ�� ��ȯ�ϴ� ������Ʈ ó��
	const bool			Change_Scene(const SCENE_TYPE& _eType);
	const bool			Change_AIState(AI* _pAI, wstring _pNextStateName);

private:
	void				Add_Event(EVENT& _eve) { m_vecEvent.push_back(_eve); }
	void				Excute_Event(EVENT& _eve);

private:
	vector<EVENT>		m_vecEvent;
	vector<CObj*>		m_vecDeleteObj; // �޸𸮸� ������ ������Ʈ���� ��Ƶд�.
	vector<CObj*>		m_vecRemoveObj; // Ǯ�� ��ȯ�� ������Ʈ���� ��Ƶд�.
};

