#pragma once

class CObj;

// 에디터에서 생성하는 모든 타일, 맵 오브젝트, 타일, 스폰위치 등을 저장하고 로드한다.

class CMapMgr
{
	SINGLETON(CMapMgr)

private:
	CMapMgr();
	virtual ~CMapMgr();

public:
	void			Render(HDC _hDC);
	void			Release();

public:
	const bool		Save_Tile(const TCHAR* pLoadPath);
	const bool		Load_Tile(const TCHAR* pLoadPath);

public:
	//const bool		Create_Tile(const TILE_TYPE& _eType);
	//const bool		Remove_Obj(); // 지금은 단순 타일이지만 후에는 오브젝트까지 판별해서 삭제할 수 있도록 한다.

	const bool		Create_RectTile(const VEC2& _vPos, const VEC2& _vScale, const TILE_TYPE& _eType);
	const bool		Remove_RectTile(const VEC2& _vClickPos);
	//const bool		Create_All_Tile(const int& _iCX, const int& iCY);

private:
	//CObj*			IsInTile(const VEC2& _vPos);

private:
	vector<CObj*>		m_vecTile;
};

