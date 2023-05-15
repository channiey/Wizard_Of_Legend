#pragma once
class CMainGame
{
	SINGLETON(CMainGame)

private:
	CMainGame();
	virtual ~CMainGame();

public:
	const bool		Init();
	void			Progress();
	void			Release();

public:
	HDC				Get_DC() const								{ return m_DC; }
	HDC				Get_SubDC() const							{ return m_subDC; }
	vector<HFONT>	Get_Font(const FONT_TYPE& _eType) const		{ return m_vecFont[(UINT)_eType]; }

private:
	void			Create_Font();
	void			Delete_Font();
	void			Init_Pool();
	
private:
	HDC				m_DC;
	HDC				m_subDC;
	HBITMAP			m_bitMap;
	RECT			m_RC;

	vector<HFONT>			m_vecFont[(UINT)FONT_TYPE::TYPEEND];
};

