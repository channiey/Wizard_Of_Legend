#pragma once
#include "UI.h"

// 보스 생성시 같이 생성

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

	float				m_fHPper;  // HP %, 아래 변수에 곱해서 가로 사이즈를 결정한다.
	float				m_fHPFullX; // HP가 풀일 때 HP 이미지 가로 사이즈

	float				m_fHPMag;


	CObj*				m_pOwner;

	bool				m_bStartGimmick;
};

