#pragma once
#include "UI.h"

// ���� ������ ���� ����

class CObj;

class CBossStatUI :
	public CUI
{
public:
	CBossStatUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType, CObj* _pOnwer);
	~CBossStatUI();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release();

public:
	const bool			Reset(CObj* _pOnwer);
	virtual void		Set_Active(const bool _bActive) override;

private:
	virtual void		Load_Tex() override;
	void				Calculate_HP();
	void				Start_Gimmick();

private:
	CTexture*			m_pTexFormat;
	CTexture*			m_pHP;

	VEC2				m_vFormatPos;
	VEC2				m_vHPPos;

	float				m_fHPper;  // HP %, �Ʒ� ������ ���ؼ� ���� ����� �����Ѵ�.
	float				m_fHPFullX; // HP�� Ǯ�� �� HP �̹��� ���� ������

	float				m_fHPMag;


	CObj*				m_pOwner;

	bool				m_bStartGimmick;
};

