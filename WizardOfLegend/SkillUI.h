#pragma once
#include "UI.h"

class CTexture;

typedef struct mySKillUI
{
	CTexture*			FormatTex = nullptr;
	CTexture*			pSkillTex = nullptr;
	CTexture*			pKeyTex = nullptr;

	VEC2				vFormatPos{};
	VEC2				vSkillPos{};
	VEC2				vKeyPos{};

}SKILL_UI;

class CSkillUI :
	public CUI
{
public:
	CSkillUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType);
	virtual ~CSkillUI();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release();

private:
	virtual void		Load_Tex() override;

	vector<SKILL_UI>    m_vecSkillUI;
};

