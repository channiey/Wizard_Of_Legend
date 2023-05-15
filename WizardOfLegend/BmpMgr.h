#pragma once

class CMyBmp;

class CBmpMgr
{
	SINGLETON(CBmpMgr)

private:
	CBmpMgr();
	~CBmpMgr();

public:
	void								Init();
	void								Release(void);

public:
	void								Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC									Find_Image(const TCHAR* pImgKey);
	CMyBmp*								Get_Bmp(const TCHAR* pImgKey);

private:
	map<const TCHAR*, CMyBmp*>		m_mapBit;
};

