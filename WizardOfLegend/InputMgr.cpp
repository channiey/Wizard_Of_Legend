#include "stdafx.h"
#include "InputMgr.h"
#include "MouseUI.h"
#include "EventMgr.h"

CInputMgr* CInputMgr::m_pInst = nullptr;

CInputMgr::CInputMgr()
{
	ZeroMemory(&m_ptMouse, sizeof(m_ptMouse));
	ZeroMemory(&m_bKeyState, sizeof(m_bKeyState));
	ZeroMemory(&m_arrMouseState, sizeof(m_arrMouseState));
	ZeroMemory(&m_arrBlockState, sizeof(m_arrBlockState));
}

CInputMgr::~CInputMgr()
{
	Release();
}

void CInputMgr::Init()
{
	Create_MouseUI(true);
}

void CInputMgr::Release()
{
}

const bool CInputMgr::Key_Down(const int& _iKey)
{
	//HWND hwnd = GetFocus();
	//if (!hwnd) return false;

	//if (m_arrBlockState[(UINT)INPUT_TYPE::KEY]) return false;

	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000)) // 이전에 안 눌리고 이번에 눌림
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))     // 이전에 눌리고 이번에 안눌림
			m_bKeyState[i] = !m_bKeyState[i];
	}

	return false;
}

const bool CInputMgr::Key_Up(const int& _iKey)
{
	//HWND hwnd = GetFocus();
	//if (!hwnd) return false;

	//if (m_arrBlockState[(UINT)INPUT_TYPE::KEY]) return false;

	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000)) // 이전에 눌리고 이번에 안눌림
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000)) // 이전에 안 눌리고 이번에 눌림
			m_bKeyState[i] = !m_bKeyState[i];
	}

	return false;
}

const bool CInputMgr::Key_Hold(const int& _iKey)
{
	HWND hwnd = GetFocus();
	if (!hwnd) return false;

	if (m_arrBlockState[(UINT)INPUT_TYPE::KEY]) return false;

	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;

	return false;
}
const bool CInputMgr::Mouse_Down(const MOUSE_TYPE& _eBtn, const int& _iKey)
{
	HWND hwnd = GetFocus();
	if (!hwnd) return false;

	if (m_arrBlockState[(UINT)INPUT_TYPE::MOUSE]) return false;

	if (!m_arrMouseState[(UINT)_eBtn] && (GetAsyncKeyState((UINT)_iKey) & 0x8000))
	{
		m_arrMouseState[(UINT)_eBtn] = true;
		return true;
	}

	for (int i = 0; i < (UINT)MOUSE_TYPE::TYPEEND; ++i)
	{
		if (m_arrMouseState[(UINT)_eBtn] && !(GetAsyncKeyState(_iKey) & 0x8000))
			m_arrMouseState[(UINT)_eBtn] = false;
	}
	return false;
}

const bool CInputMgr::Mouse_Hold(const MOUSE_TYPE& _eBtn, const int& _iKey)
{
	HWND hwnd = GetFocus();
	if (!hwnd) return false;

	if (m_arrBlockState[(UINT)INPUT_TYPE::KEY]) return false;

	if ((GetAsyncKeyState(_iKey) & 0x8000))
		return true;
	else
		return false;
}

const bool CInputMgr::Mouse_Up(const MOUSE_TYPE& _eBtn, const int& _iKey)
{
	HWND hwnd = GetFocus();
	if (!hwnd) return false;

	if (m_arrBlockState[(UINT)INPUT_TYPE::MOUSE]) return false;

	if (m_arrMouseState[(UINT)_eBtn] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_arrMouseState[(UINT)_eBtn] = false;
		return true;
	}

	for (int i = 0; i < (UINT)MOUSE_TYPE::TYPEEND; ++i)
	{
		if (m_arrMouseState[(UINT)_eBtn] && !(GetAsyncKeyState((UINT)_eBtn) & 0x8000))
			m_arrMouseState[(UINT)_eBtn] = true;
	}
	return false;
}

const POINT CInputMgr::Get_Mouse_Pos()
{
	HWND hwnd = GetFocus();
	if (!hwnd) return POINT{ int(WINCX * 0.5f), int(WINCY * 0.5f)};

	GetCursorPos(&m_ptMouse);
	ScreenToClient(g_hWnd, &m_ptMouse);

	return m_ptMouse;
}

void CInputMgr::Create_MouseUI(const bool & _b)
{
	if (!_b) return;

	ShowCursor(false); // 커서 지우기
	CObj* pMouseUI = new CMouseUI(OBJ_TYPE::UI, UI_TYPE::FIXED);
	pMouseUI->Init();
	CEventMgr::Get_Inst()->Add_Obj(pMouseUI);
}

const bool CInputMgr::Set_Block(const INPUT_TYPE & _etype, const bool & _eBolck)
{
	m_arrBlockState[(UINT)_etype] = _eBolck;

	return true;
}

const bool CInputMgr::Set_Block_All(const bool & _eBlock)
{
	for (auto& iter : m_arrBlockState)
		iter = _eBlock;

	return true;
}



