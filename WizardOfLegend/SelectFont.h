#pragma once
class SelectFont
{
public:
	SelectFont(HDC _hDC, const FONT_TYPE& _eType, const FONT_SIZE& _eSize, const int& _iBackground = TRANSPARENT, const bool _bTextColotBlack = false);
	~SelectFont();

private:
	HDC		m_hDC;
	HFONT	m_hDefaultFont;
};

