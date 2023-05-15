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
	list<CObj*>* pUIlist = CObjMgr::Get_Inst()->Get_ObjList(OBJ_TYPE::UI); // ���� �����ϴ� ��� �θ� UI�� �����´�.

	bool bLBTNtab = CInputMgr::Get_Inst()->Key_Down(VK_LBUTTON); // ���콺 ���� ��ư�� ���� �� ���ȴ���
	bool bLBTNaway = CInputMgr::Get_Inst()->Key_Up(VK_LBUTTON); // ���콺 ���� ��ư�� ���� �� ��������

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
	// 1. �θ� UI ���� ��� �ڽ� UI�� �˻��Ѵ�.

	// 2. Ÿ�� UI�� ��, �� �켱������ ���� ������ �� ���� ������ �ڽ� UI

	return nullptr;
}
