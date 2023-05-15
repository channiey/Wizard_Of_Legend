#include "stdafx.h"
#include "Texture.h"
#include "MainGame.h"

CTexture::CTexture()
	: m_bRotateUpDown(false)
{
}


CTexture::~CTexture()
{
	Release();
}

void CTexture::Release()
{
}

void CTexture::Load(const wstring & _strPath, const bool _bRotateX)
{
	Image* img = Image::FromFile((TCHAR*)_strPath.c_str(), 0);

	// https://learn.microsoft.com/ko-kr/windows/win32/api/gdiplusimaging/ne-gdiplusimaging-rotatefliptype

	if (_bRotateX)
		img->RotateFlip(Rotate180FlipY);

	m_pImg = img;

	/*m_pImg = new Image((TCHAR*)_strPath.c_str());
	m_pImg->FromFile((TCHAR*)_strPath.c_str(), 0)*/;
}