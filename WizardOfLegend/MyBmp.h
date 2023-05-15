#pragma once
class CMyBmp
{
public:
	CMyBmp();
	~CMyBmp();

public:
	HDC			Get_MemDC() { return m_hMemDC; }
	void		Load_Bmp(const TCHAR* pFilePath);
	void		Release(void);

	const int&	Get_Width() const { return m_bitInfo.bmWidth; }
	const int&	Get_Height() const { return m_bitInfo.bmHeight; }
	const VEC2 Get_Scale() const { return VEC2(m_bitInfo.bmWidth, m_bitInfo.bmHeight); }

private:
	HDC			m_hMemDC;
	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldmap;
	BITMAP		m_bitInfo;
};

