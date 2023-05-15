#include "stdafx.h"
#include "ResMgr.h"
#include "Texture.h"

CResMgr* CResMgr::m_pInst = nullptr;

CResMgr::CResMgr()
{
}


CResMgr::~CResMgr()
{
	Release();
}

void CResMgr::Release()
{
	Safe_Delete_Map(m_mapTex);
}

CTexture * CResMgr::Load_Tex(const wstring& _strKey, const wstring& _strPath, const bool _bRotateX)
{
	CTexture* pTex = Find_Tex(_strKey);

	if (nullptr == pTex) 
		pTex = new CTexture;

	pTex->Load(_strPath, _bRotateX);

	pTex->Set_Key(_strKey);
	//pTex->Set_Path(_strPath);

	m_mapTex.insert({ _strKey, pTex });

	return pTex;
}

CTexture * CResMgr::Find_Tex(const wstring & _strKey)
{
	map<wstring, CTexture*>::iterator iter = m_mapTex.find(_strKey);

	if (m_mapTex.end() == iter)
		return nullptr;

	return iter->second;
}
