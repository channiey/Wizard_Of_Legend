#include "stdafx.h"
#include "SelectFont.h"
#include "MainGame.h"

// HDC, ��Ʈ Ÿ��, ��׶��� Ÿ��(OPAQUE, TRANSPARENT), �۲� ����(White, Black)
SelectFont::SelectFont(HDC _hDC, const FONT_TYPE& _eType, const FONT_SIZE& _eSize, const int& _iBackground, const bool _bTextColotBlack)
{
	// ��Ʈ ��׶���
	SetBkMode(_hDC, _iBackground); 

	// ��Ʈ ����
	if(_bTextColotBlack)
		SetTextColor(_hDC, RGB(0, 0, 0)); 
	else
		SetTextColor(_hDC, RGB(255, 255, 255)); 

	// ��Ʈ ����

	vector<HFONT> vecHFont = CMainGame::Get_Inst()->Get_Font(_eType);

	m_hDefaultFont = (HFONT)SelectObject(_hDC, vecHFont[(UINT)_eSize]);
}


SelectFont::~SelectFont()
{
	SelectObject(m_hDC, m_hDefaultFont);
}
