#pragma once
class CGameMgr
{
	SINGLETON(CGameMgr)

private:
	CGameMgr();
	virtual ~CGameMgr();

public:
	void				Set_DevMode_Toggle()					{ m_bDevMode = !m_bDevMode; }
	void				Set_DevText_Toggle()					{ m_bDevText = !m_bDevText; }
	void                Set_DevRender_Toggle()					{ m_bDebRender = !m_bDebRender; }

	void				Set_DevMode(const bool& _bMode)			{ m_bDevMode = _bMode; }
	void				Set_DevText(const bool& _bMode)			{ m_bDevText = _bMode; }
	void				Set_DevRender(const bool& _bMode)		{ m_bDebRender = _bMode; }
	void				Set_Finish(const bool _bFinish)			{ m_bFinish = _bFinish; }

	const bool			Is_DevMode() const						{ return m_bDevMode; }
	const bool			Is_Finish() const						{ return m_bFinish; }
	const bool			Is_DevText() const						{ return m_bDevText; }
	const bool			Is_Dev_Render() const					{ return m_bDebRender; }

	void			Set_FPS(const int& _iFPS) { m_iFPS = _iFPS; }
	const int		Get_FPS() const { return m_iFPS; }

	void			Add_PlayerToEnemyDamage(int _iDamage) { m_iAmount_Damage_Inflected += _iDamage;  }
	void			Add_EnemyToPlayerDamage(int _iDamage) { m_iAmount_Damage_Sufferd += _iDamage;  }

	const int&	Get_Player_Damaged() const { return m_iAmount_Damage_Sufferd; }
	const int&	Get_Player_Damage() const { return m_iAmount_Damage_Inflected; }

	void	Plus_Kill() { ++m_iKill; }
	const int Get_Kills() const { return m_iKill; }

private:
	bool				m_bDevMode;
	bool				m_bDevText;
	bool				m_bDebRender;
	bool				m_bFinish;


	int					m_iFPS;



	int					m_iAmount_Damage_Sufferd;
	int					m_iAmount_Damage_Inflected;
	int					m_iKill;

};

