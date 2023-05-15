#pragma once

class CObj;

// �����Ϳ��� �����ϴ� ��� Ÿ��, �� ������Ʈ, Ÿ��, ������ġ ���� �����ϰ� �ε��Ѵ�.

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
	//const bool		Remove_Obj(); // ������ �ܼ� Ÿ�������� �Ŀ��� ������Ʈ���� �Ǻ��ؼ� ������ �� �ֵ��� �Ѵ�.

	const bool		Create_RectTile(const VEC2& _vPos, const VEC2& _vScale, const TILE_TYPE& _eType);
	const bool		Remove_RectTile(const VEC2& _vClickPos);
	//const bool		Create_All_Tile(const int& _iCX, const int& iCY);

private:
	//CObj*			IsInTile(const VEC2& _vPos);

private:
	vector<CObj*>		m_vecTile;
};

