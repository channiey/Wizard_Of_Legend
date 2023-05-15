#pragma once

class CScene;

class CSceneMgr
{
	SINGLETON(CSceneMgr)

private:
	CSceneMgr();
	virtual ~CSceneMgr();

public:
	const bool		Init();
	void			Update();
	void			LateUpdate();
	void			Render(HDC _hDC);
	void			Release();

public:
	CScene*			Get_CurScene() const { return m_pCurScene; }
	const bool		Change_Scene(const SCENE_TYPE& _eType);

private:
	CScene*			m_arrScene[(UINT)SCENE_TYPE::TYPEEND];
	CScene*			m_pCurScene;
};

