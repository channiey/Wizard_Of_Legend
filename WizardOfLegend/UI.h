#pragma once
#include "Obj.h"
class CTexture;
class CUI :
	public CObj
{
public:
	explicit CUI(const OBJ_TYPE& _eType);
	explicit CUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType);
	virtual ~CUI();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release();

private:
	void				Update_Child();
	void				LateUpdate_Child();
	void				Render_Child(HDC _hDC);

public:
	virtual void		Mouse_On();				// 마우스가 UI위에 올라옴
	virtual void		Mouse_Down();			// 해당 UI에서 마우스 왼쪽 버튼 누르고 있음
	virtual void		Mouse_Up();				// 해당 UI에서 마우스 왼쪽 버튼 뗌
	virtual void		Mouse_Clicked();		// 해당 UI에서 마우스 클릭(눌렀다 뗌)

	const bool			Add_ChildUI(CUI* _pUI)	{ if (nullptr == _pUI) return false; m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this; return true; }
	void				Check_Mouse_On();		// 마우스가 현재 내 위에 있는지 없는지 체크

public:
	const VEC2&			Get_FinalPos() const	{ return m_vFinalPos; }
	CUI*				Get_ParentUI() const	{ return m_pParentUI; }
	CObj*				Get_Owner() const { return m_pOnwer; }
	const bool			Set_Onwer(CObj* _pObj) { if (nullptr == _pObj) return false; m_pOnwer = _pObj; return true; }

	void				Set_FinalPos();
	void				Set_UIType(const UI_TYPE& _eType) { m_eUiType = _eType; }
	void				Set_Owner(CObj* _pObj) { m_pOnwer = _pObj; }

	const bool			Is_Mouse_On() const		{ return m_bMouseOn; }

private:
	virtual void		Load_Tex() PURE;

protected:
	CUI*				m_pParentUI;
	vector <CUI*>		m_vecChildUI;
	VEC2				m_vFinalPos;			// 본인 최종 위치

	UI_TYPE				m_eUiType;				// 카메라 영향을 받는지 안받는지 (고정인지 아닌지)
	bool				m_bMouseOn;				// UI 위에 마우스가 있는지 여부
	bool				m_bLbtnDown;

	Graphics			m_Graphics;
	friend class CUIMgr;

	CObj*				m_pOnwer;
};

