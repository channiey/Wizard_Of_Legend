#include "stdafx.h"
#include "MapMgr.h"
#include "CameraMgr.h"
#include "InputMgr.h"
#include "Obj.h"
#include "Tile.h"
#include "Transform.h"
#include "EventMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Collider.h"
CMapMgr* CMapMgr::m_pInst = nullptr;

CMapMgr::CMapMgr()
{
}

CMapMgr::~CMapMgr()
{
	Release();
}

void CMapMgr::Render(HDC _hDC)
{
	for (auto& iter : m_vecTile)
		iter->Render(_hDC);
}

void CMapMgr::Release()
{
}

const bool CMapMgr::Save_Tile(const TCHAR* pLoadPath)
{
	HANDLE	hFile = CreateFile(pLoadPath,
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)	
	{
		MessageBox(g_hWnd, _T("save File"), L"Fail", MB_OK);
		return false;
	}

	DWORD dwByte = 0;

	for (auto& iter : m_vecTile)
	{
		WriteFile(hFile, &(static_cast<CTile*>(iter)->Get_TileType()), sizeof(TILE_TYPE), &dwByte, nullptr);
		WriteFile(hFile, &(iter->Get_Transform()->Get_Pos()), sizeof(VEC2), &dwByte, nullptr);
		WriteFile(hFile, &(iter->Get_Transform()->Get_Scale()), sizeof(VEC2), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	return true;
}

const bool CMapMgr::Load_Tile(const TCHAR* pLoadPath)
{
	// 타일을 불러오기 전, 현재 타일 벡터에 저장된 타일들을 전부 날린다.
	if (!m_vecTile.empty())
	{
		for (auto& iter : m_vecTile)
			CEventMgr::Get_Inst()->Delete_Obj(iter);

		m_vecTile.clear();
	}

	HANDLE	hFile = CreateFile(pLoadPath,
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)	
	{
		MessageBox(g_hWnd, _T("Load File"), L"Fail", MB_OK);
		return false;
	}

	TILE_TYPE eType = TILE_TYPE::WALL;
	VEC2 vPos{};
	VEC2 vScale{};
	DWORD dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &eType, sizeof(TILE_TYPE), &dwByte, nullptr);
		ReadFile(hFile, &vPos, sizeof(VEC2), &dwByte, nullptr);
		ReadFile(hFile, &vScale, sizeof(VEC2), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CObj* pTile = new CTile(OBJ_TYPE::TILE);
		pTile->Init();
		pTile->Get_Transform()->Set_Pos(vPos);
		pTile->Get_Transform()->Set_Scale(vScale);
		pTile->Get_Collider()->Set_Pos(vPos);
		pTile->Get_Collider()->Set_Scale(vScale);

		if (TILE_TYPE::TYPEEND == eType)
			eType = TILE_TYPE::WALL;

		static_cast<CTile*>(pTile)->Set_TileType(eType);


		if(CSceneMgr::Get_Inst()->Get_CurScene()->Get_SceneType() == SCENE_TYPE::EDITOR)
			m_vecTile.push_back(pTile);

		CEventMgr::Get_Inst()->Add_Obj(pTile);
	}

	CloseHandle(hFile);

	return true;
}


const bool CMapMgr::Create_RectTile(const VEC2 & _vPos, const VEC2 & _vScale, const TILE_TYPE & _eType)
{
	// Create Tile
	CObj* pTile = new CTile(OBJ_TYPE::TILE);
	pTile->Init();
	pTile->Get_Transform()->Set_Pos(_vPos);
	pTile->Get_Transform()->Set_Scale(_vScale);
	static_cast<CTile*>(pTile)->Set_TileType(_eType);

	m_vecTile.push_back(pTile);
	//CEventMgr::Get_Inst()->Add_Obj(pTile);

	//cout << "렉트 생성! 현재 렉트 개수 : " << m_vecTile.size() << endl;
	return true;
}

const bool CMapMgr::Remove_RectTile(const VEC2 & _vClickPos)
{
	for (vector<CObj*>::iterator iter = m_vecTile.begin(); iter != m_vecTile.end(); ++iter)
	{
		VEC2 vPos = (*iter)->Get_Transform()->Get_Pos();
		VEC2 vScale = (*iter)->Get_Transform()->Get_Scale();

		if (((vPos.fX - vScale.fX * 0.5f < _vClickPos.fX) && (_vClickPos.fX < vPos.fX + vScale.fX * 0.5f))
			&& ((vPos.fY - vScale.fY * 0.5f < _vClickPos.fY) && (_vClickPos.fY) < vPos.fY + vScale.fY * 0.5f))
		{
			m_vecTile.erase(iter);
			//cout << "렉트 삭제! 현재 렉트 개수 : " << m_vecTile.size() << endl;
			return true;
		}
	}
	return false;
}
