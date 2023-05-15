#pragma once

class CTexture;

class CResMgr
{
	SINGLETON(CResMgr)

private:
	CResMgr();
	~CResMgr();

public:
	void Release();

private:
	map<wstring, CTexture*> m_mapTex;

public:
	CTexture* Load_Tex(const wstring& _strKey, const wstring& _strPath, const bool _bRotateX = false);
	CTexture* Find_Tex(const wstring& _strKey);
};

