#include "stdafx.h"
#include "UIMgr.h"
#include "ObjMgr.h"
#include "UI.h"
#include "InputMgr.h"

CUIMgr* CUIMgr::m_pInst = nullptr;

CUIMgr::CUIMgr()
{
}

CUIMgr::~CUIMgr()
{
	Release();
}

void CUIMgr::Update()
{
	list<CObj*>* pUIlist = CObjMgr::Get_Inst()->Get_ObjList(OBJ_TYPE::UI); // 씬에 존재하는 모든 부모 UI를 가져온다.

	bool bLBTNtab = CInputMgr::Get_Inst()->Key_Down(VK_LBUTTON); // 마우스 왼쪽 버튼이 지금 막 눌렸는지
	bool bLBTNaway = CInputMgr::Get_Inst()->Key_Up(VK_LBUTTON); // 마우스 왼쪽 버튼이 지금 막 떼졌는지

	for (auto& iter : *pUIlist)
	{
		CUI* pUI = static_cast<CUI*>(iter);

		if (pUI->Is_Mouse_On())
		{
			pUI->Mouse_On();

			if (bLBTNtab)
			{
				pUI->Mouse_Down();
				pUI->m_bLbtnDown = true;
			}
			else if (bLBTNaway)
			{
				pUI->Mouse_Up();

				if (pUI->m_bLbtnDown)
					pUI->Mouse_Clicked();

				pUI->m_bLbtnDown = false;
			}
		}
		else
		{
			if (bLBTNaway)
				pUI->m_bLbtnDown = false;
		}
	}
}

void CUIMgr::Release()
{
}

CUI * CUIMgr::Get_TargetedUI(CUI * _pParentUI)
{
	// 1. 부모 UI 포함 모든 자식 UI를 검사한다.

	// 2. 타겟 UI들 중, 더 우선순위가 높은 기준은 더 낮은 계층의 자식 UI

	return nullptr;
}
