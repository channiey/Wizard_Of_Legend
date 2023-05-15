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
	virtual void		Mouse_On();				// ���콺�� UI���� �ö��
	virtual void		Mouse_Down();			// �ش� UI���� ���콺 ���� ��ư ������ ����
	virtual void		Mouse_Up();				// �ش� UI���� ���콺 ���� ��ư ��
	virtual void		Mouse_Clicked();		// �ش� UI���� ���콺 Ŭ��(������ ��)

	const bool			Add_ChildUI(CUI* _pUI)	{ if (nullptr == _pUI) return false; m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this; return true; }
	void				Check_Mouse_On();		// ���콺�� ���� �� ���� �ִ��� ������ üũ

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
	VEC2				m_vFinalPos;			// ���� ���� ��ġ

	UI_TYPE				m_eUiType;				// ī�޶� ������ �޴��� �ȹ޴��� (�������� �ƴ���)
	bool				m_bMouseOn;				// UI ���� ���콺�� �ִ��� ����
	bool				m_bLbtnDown;

	Graphics			m_Graphics;
	friend class CUIMgr;

	CObj*				m_pOnwer;
};

