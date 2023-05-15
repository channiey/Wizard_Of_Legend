#pragma once
class CScene
{
public:
	explicit CScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos);
	virtual ~CScene();

public:
	virtual const bool	Init();
	virtual void		Update();
	virtual void		LateUpdate();
	virtual void		Render(HDC _hDC);

public:
	virtual void		Enter() PURE;
	virtual void		Exit() PURE;

public:
	const SCENE_TYPE&	Get_SceneType() const { return m_eType; }
	VEC2&				Get_StartPos() { return m_vStartPos; }

	void				Set_SceneType(const SCENE_TYPE& _eType) { m_eType = _eType; }
	void				Set_StartPos(const VEC2& _vPos) { m_vStartPos = _vPos; }

private:
	void				Appear_Player();

protected:
	SCENE_TYPE			m_eType;
	VEC2				m_vStartPos;
	float				m_fAcc;
	float				m_fPlayerAppearTime;
	bool				m_bPlayerAppear;
};

