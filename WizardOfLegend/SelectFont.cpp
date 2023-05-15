#include "stdafx.h"
#include "SelectFont.h"
#include "MainGame.h"

// HDC, 폰트 타입, 백그라운드 타입(OPAQUE, TRANSPARENT), 글꼴 색상(White, Black)
SelectFont::SelectFont(HDC _hDC, const FONT_TYPE& _eType, const FONT_SIZE& _eSize, const int& _iBackground, const bool _bTextColotBlack)
{
	// 폰트 백그라운드
	SetBkMode(_hDC, _iBackground); 

	// 폰트 색상
	if(_bTextColotBlack)
		SetTextColor(_hDC, RGB(0, 0, 0)); 
	else
		SetTextColor(_hDC, RGB(255, 255, 255)); 

	// 폰트 적용

	vector<HFONT> vecHFont = CMainGame::Get_Inst()->Get_Font(_eType);

	m_hDefaultFont = (HFONT)SelectObject(_hDC, vecHFont[(UINT)_eSize]);
}


SelectFont::~SelectFont()
{
	SelectObject(m_hDC, m_hDefaultFont);
}
