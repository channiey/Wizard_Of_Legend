#pragma once

class CEnemy;
class CState;
class CObj;

class AI
{
public:
	AI(CEnemy* _pOwner);
	virtual ~AI();

public:
	void					Update(); // Enemy의 실질적 업데이트는 AI가 맡아서 처리한다.
	void					Render(HDC _hDC);
	void					Release();

public:
	CEnemy*					Get_Owner() const { return m_pOwner; }
	CState*					Get_State(const wstring& _strName);
	CState*					Get_CurState() const { return m_pCurState; }

	const bool				Set_Owner(CEnemy* _pEnemy) { if (nullptr == _pEnemy) return false; _pEnemy = _pEnemy; return true; }
	const bool				Set_CurState(const wstring& _strName);

	const bool				Add_State(CState* _pState);
	const bool				Change_State(const wstring& _strNextStateName);

private:
	CEnemy*					m_pOwner;
	map<wstring, CState*>	m_mapState;
	CState*					m_pCurState;
};
