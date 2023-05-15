#pragma once

class AI;
class CEnemy;

class CState
{
public:
	CState(const wstring& _strName);
	virtual ~CState();

public:
	AI*				Get_AI() const { return m_pAI; }
	const wstring&	Get_Name() const { return m_strName; }
	CEnemy*			Get_Owner() const;

	const bool		Set_AI(AI* _pAI) { if (nullptr == _pAI) return false; m_pAI = _pAI; return true; }

public:
	virtual void	Enter() PURE;
	virtual void	Update() PURE;
	virtual void	Render(HDC _hDC) {}
	virtual void	Exit() PURE;

protected:
	AI*				m_pAI;
	wstring			m_strName;

	float			m_fDuration;
	float			m_fAcc;
};

