#pragma once
#include "Scene.h"

class CTexture;

class CEditorScene : public CScene
{
public:
	explicit CEditorScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos);
	virtual ~CEditorScene();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;

public:
	virtual void		Enter() override;
	virtual void		Exit() override;

private:
	void				Set_TileType();				// 생성할 타일의 타입을 결정 -> m_eMode
	void				Key_Input();				// 마우스 인풋에 따른 행동을 결정 (생성, 삭제, 저장, 불러오기)

	void				Render_Text(HDC _hDC);
	void				Render_Background(HDC _hDC);
	void				Render_CurRect(HDC _hDC);	// 현재 내가 지정하고 있는 렉트 영역을 그린다.

private:
	CTexture*			m_pTexBack;
	Graphics			m_Graphics;

	TILE_TYPE			m_eType;

	bool				m_bFirstClick;
	VEC2				m_vLT;
	VEC2				m_vRB;
};

