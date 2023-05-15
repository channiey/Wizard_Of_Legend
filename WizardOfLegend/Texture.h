#pragma once
#include "Res.h"
class CTexture :
	public CRes
{
public:
	CTexture();
	virtual ~CTexture();

public:
	void		Release();
	void		Load(const wstring & _strPath, const bool _bRotateX);

	Image* 		Get_Image()				{ return m_pImg; }

	void		Set_RotationUpDown(const bool& _b) { m_bRotateUpDown = _b; }
	const bool	Is_RotationUpDown() const { return m_bRotateUpDown; }

private:
	Image*    m_pImg;
	bool	m_bRotateUpDown;
};

