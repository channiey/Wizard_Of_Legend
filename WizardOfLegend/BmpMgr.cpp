#include "stdafx.h"
#include "BmpMgr.h"
#include "MyBmp.h"

CBmpMgr* CBmpMgr::m_pInst = nullptr;

CBmpMgr::CBmpMgr()
{
}


CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Init()
{
	
}

void CBmpMgr::Release(void)
{
	for_each(m_mapBit.begin(), m_mapBit.end(), DeleteMap());
	m_mapBit.clear();
}

void CBmpMgr::Insert_Bmp(const TCHAR * pFilePath, const TCHAR * pImgKey)
{
	auto		iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
	{
		CMyBmp*		pBmp = new CMyBmp;
		pBmp->Load_Bmp(pFilePath); // ����� ������ �о� ��Ʈ�� ��������� �����Ѵ�.

		m_mapBit.emplace(pImgKey, pBmp); // �ش� Bmp�� �ʿ� Ǫ���Ѵ�.
	}
}

HDC CBmpMgr::Find_Image(const TCHAR * pImgKey)
{
	auto		iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second->Get_MemDC();
}

CMyBmp * CBmpMgr::Get_Bmp(const TCHAR * pImgKey)
{
	auto		iter = find_if(m_mapBit.begin(), m_mapBit.end(), tagFinder(pImgKey));

	if (iter == m_mapBit.end())
		return nullptr;

	return iter->second;
}
