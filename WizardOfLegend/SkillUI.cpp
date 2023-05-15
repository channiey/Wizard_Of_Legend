#include "stdafx.h"
#include "SkillUI.h"
#include "Texture.h"
#include "ResMgr.h"
#include "Player.h"
#include "Skill.h"
#include "ObjMgr.h"

CSkillUI::CSkillUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType)
	: CUI(_eType, _eUiType)
{
}


CSkillUI::~CSkillUI()
{
}

const bool CSkillUI::Init()
{
	Load_Tex();

	return true;
}

void CSkillUI::Update()
{
}

void CSkillUI::LateUpdate()
{
}

void CSkillUI::Render(HDC _hDC)
{
	UINT iWidth = 0, iHeight = 0;

	for (int i = 0; i < (UINT)SKILL_KEY_TYPE::R; ++i)
	{
		CSkill* pSkill = static_cast<CPlayer*>(CObjMgr::Get_Inst()->Get_Player())->Get_Skill((SKILL_KEY_TYPE)i);
		if (nullptr == pSkill) continue;

		// KeyBoard
		iWidth = m_vecSkillUI[i].pKeyTex->Get_Image()->GetWidth();
		iHeight = m_vecSkillUI[i].pKeyTex->Get_Image()->GetHeight();

		m_Graphics.DrawImage(m_vecSkillUI[i].pKeyTex->Get_Image(),
			int(m_vecSkillUI[i].vKeyPos.fX),
			int(m_vecSkillUI[i].vKeyPos.fY),
			int(iWidth * 2),
			int(iHeight * 2));

		// Format
		iWidth = m_vecSkillUI[i].FormatTex->Get_Image()->GetWidth();
		iHeight = m_vecSkillUI[i].FormatTex->Get_Image()->GetHeight();

		m_Graphics.DrawImage(m_vecSkillUI[i].FormatTex->Get_Image(),
			int(m_vecSkillUI[i].vFormatPos.fX),
			int(m_vecSkillUI[i].vFormatPos.fY),
			int(iWidth * 3.5f),
			int(iHeight * 3.5f));


		// SKill
		if (i == 4 & !g_SkillUI) continue;

		iWidth = m_vecSkillUI[i].pSkillTex->Get_Image()->GetWidth();
		iHeight = m_vecSkillUI[i].pSkillTex->Get_Image()->GetHeight();

		m_Graphics.DrawImage(m_vecSkillUI[i].pSkillTex->Get_Image(),
			int(m_vecSkillUI[i].vSkillPos.fX + 5),
			int(m_vecSkillUI[i].vSkillPos.fY + 5),
			int(iWidth * 3.5f - 10),
			int(iHeight * 3.5f - 10));
	

		if (pSkill->Is_Cool())
		{
			// Cool Time
			Rect rc{ int(m_vecSkillUI[i].vSkillPos.fX + 5),
				int(m_vecSkillUI[i].vSkillPos.fY + 5 + (int(int(iHeight * 3.5f - 10) * pSkill->Get_CoolPer()))),
				int(iWidth * 3.5f - 10),
				int(int(iHeight * 3.5f - 10) * (1 - pSkill->Get_CoolPer())) };
				SolidBrush brush(Color(150, 0, 0, 0));
				m_Graphics.FillRectangle(&brush, rc);
		}

	}
	//for (auto& iter : m_vecSkillUI)
	//{
	//	// Key
	//	iWidth = iter.pKeyTex->Get_Image()->GetWidth();
	//	iHeight = iter.pKeyTex->Get_Image()->GetHeight();

	//	m_Graphics.DrawImage(iter.pKeyTex->Get_Image(),
	//		int(iter.vKeyPos.fX),
	//		int(iter.vKeyPos.fY),
	//		int(iWidth * 2),
	//		int(iHeight * 2));

	//	// Format
	//	iWidth = iter.FormatTex->Get_Image()->GetWidth();
	//	iHeight = iter.FormatTex->Get_Image()->GetHeight();

	//	m_Graphics.DrawImage(iter.FormatTex->Get_Image(),
	//		int(iter.vFormatPos.fX),
	//		int(iter.vFormatPos.fY),
	//		int(iWidth * 3.5f),
	//		int(iHeight * 3.5f));


	//	// SKill

	//	iWidth = iter.pSkillTex->Get_Image()->GetWidth();
	//	iHeight = iter.pSkillTex->Get_Image()->GetHeight();

	//	m_Graphics.DrawImage(iter.pSkillTex->Get_Image(),
	//		int(iter.vSkillPos.fX + 5),
	//		int(iter.vSkillPos.fY + 5),
	//		int(iWidth * 3.5f - 10),
	//		int(iHeight * 3.5f - 10));
	//}
}

void CSkillUI::Release()
{
}
 // ÁÂ, ½ºÆäÀÌ½º, ¿ì , q e, r / ÅÇ, ¸Þ´º
void CSkillUI::Load_Tex()
{
	float fSkillTerm = 60;
	float fKeyTerm = 60;
	int i = 0;

	SKILL_UI UI4;
	UI4.FormatTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_FORMAT", L"../Data/Sprite/99_UI/skill/CooldownUI22.png");
	UI4.pSkillTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_FIREDRAGON", L"../Data/Sprite/99_UI/skill/Skills #1366542.png");
	UI4.pKeyTex = CResMgr::Get_Inst()->Load_Tex(L"KEY_Q", L"../Data/Sprite/99_UI/skill/Q.png");
	UI4.vFormatPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI4.vSkillPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI4.vKeyPos = VEC2(50 + fKeyTerm * i++, WINCY - 135);
	m_vecSkillUI.push_back(UI4);

	SKILL_UI UI2;
	UI2.FormatTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_FORMAT", L"../Data/Sprite/99_UI/skill/CooldownUI22.png");
	UI2.pSkillTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_DASH", L"../Data/Sprite/99_UI/skill/Dash.png");
	UI2.pKeyTex = CResMgr::Get_Inst()->Load_Tex(L"KEY_SPACE", L"../Data/Sprite/99_UI/skill/Space.png");
	UI2.vFormatPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI2.vSkillPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI2.vKeyPos = VEC2(50 + fKeyTerm * i++, WINCY - 135);
	m_vecSkillUI.push_back(UI2);

	SKILL_UI UI1;
	UI1.FormatTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_FORMAT", L"../Data/Sprite/99_UI/skill/CooldownUI22.png");
	UI1.pSkillTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_SLASH", L"../Data/Sprite/99_UI/skill/WindSlashBasic.png");
	UI1.pKeyTex = CResMgr::Get_Inst()->Load_Tex(L"KEY_LBTN", L"../Data/Sprite/99_UI/skill/KeyboardKeys2.png");
	UI1.vFormatPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI1.vSkillPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI1.vKeyPos = VEC2(50 + fKeyTerm * i++, WINCY - 135);
	m_vecSkillUI.push_back(UI1);


	SKILL_UI UI3;
	UI3.FormatTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_FORMAT", L"../Data/Sprite/99_UI/skill/CooldownUI22.png");
	UI3.pSkillTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_ARC", L"../Data/Sprite/99_UI/skill/Skills #136654 (1).png");
	UI3.pKeyTex = CResMgr::Get_Inst()->Load_Tex(L"KEY_RBTN", L"../Data/Sprite/99_UI/skill/KeyboardKeys2 (1).png");
	UI3.vFormatPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI3.vSkillPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI3.vKeyPos = VEC2(50 + fKeyTerm * i++, WINCY - 135);
	m_vecSkillUI.push_back(UI3);


	SKILL_UI UI5;
	UI5.FormatTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_FORMAT", L"../Data/Sprite/99_UI/skill/CooldownUI22.png");
	UI5.pSkillTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_WATER_BOMB", L"../Data/Sprite/99_UI/skill/Skills #13665224.png");
	UI5.pKeyTex = CResMgr::Get_Inst()->Load_Tex(L"KEY_E", L"../Data/Sprite/99_UI/skill/E.png");
	UI5.vFormatPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI5.vSkillPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI5.vKeyPos = VEC2(50 + fKeyTerm * i++, WINCY - 135);
	m_vecSkillUI.push_back(UI5);

	/*SKILL_UI UI6;
	UI6.FormatTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_FORMAT", L"../Data/Sprite/99_UI/skill/CooldownUI22.png");
	UI6.pSkillTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_NONE", L"../Data/Sprite/99_UI/skill/Skills #136654df.png");
	UI6.pKeyTex = CResMgr::Get_Inst()->Load_Tex(L"KEY_R", L"../Data/Sprite/99_UI/skill/R.png");
	UI6.vFormatPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI6.vSkillPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	UI6.vKeyPos = VEC2(50 + fKeyTerm * i++, WINCY - 135);
	m_vecSkillUI.push_back(UI6);*/

	//SKILL_UI UI7;
	//UI7.FormatTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_FORMAT", L"../Data/Sprite/99_UI/skill/CooldownUI22.png");
	//UI7.pSkillTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_NONE", L"../Data/Sprite/99_UI/skill/SkillsNONE.png");
	//UI7.pKeyTex = CResMgr::Get_Inst()->Load_Tex(L"KEY_TAB", L"../Data/Sprite/99_UI/skill/Tab.png");
	//UI7.vFormatPos = VEC2(40 + fSkillTerm * ++i, WINCY - 100);
	//UI7.vSkillPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	//UI7.vKeyPos = VEC2(50 + fKeyTerm * i++, WINCY - 135);
	//m_vecSkillUI.push_back(UI7);

	//SKILL_UI UI8;
	//UI8.FormatTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_FORMAT", L"../Data/Sprite/99_UI/skill/CooldownUI22.png");
	//UI8.pSkillTex = CResMgr::Get_Inst()->Load_Tex(L"SKILL_NONE", L"../Data/Sprite/99_UI/skill/SkillsNONE.png");
	//UI8.pKeyTex = CResMgr::Get_Inst()->Load_Tex(L"KEY_M", L"../Data/Sprite/99_UI/skill/M.png");
	//UI8.vFormatPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	//UI8.vSkillPos = VEC2(40 + fSkillTerm * i, WINCY - 100);
	//UI8.vKeyPos = VEC2(50 + fKeyTerm * i++, WINCY - 135);
	//m_vecSkillUI.push_back(UI8);
}
