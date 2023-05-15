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
	void				Set_TileType();				// ������ Ÿ���� Ÿ���� ���� -> m_eMode
	void				Key_Input();				// ���콺 ��ǲ�� ���� �ൿ�� ���� (����, ����, ����, �ҷ�����)

	void				Render_Text(HDC _hDC);
	void				Render_Background(HDC _hDC);
	void				Render_CurRect(HDC _hDC);	// ���� ���� �����ϰ� �ִ� ��Ʈ ������ �׸���.

private:
	CTexture*			m_pTexBack;
	Graphics			m_Graphics;

	TILE_TYPE			m_eType;

	bool				m_bFirstClick;
	VEC2				m_vLT;
	VEC2				m_vRB;
};

