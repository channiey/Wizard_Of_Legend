#pragma once
class CInputMgr
{
	SINGLETON(CInputMgr)

private:
	CInputMgr();
	~CInputMgr();

public:
	void Init();
	void Release();

public:
	const bool		Key_Down(const int& _iKey);
	const bool		Key_Hold(const int& _iKey);
	const bool		Key_Up(const int& _iKey);

	const bool		Mouse_Down(const MOUSE_TYPE& _eBtn, const int& _iKey);
	const bool		Mouse_Hold(const MOUSE_TYPE& _eBtn, const int& _iKey);
	const bool		Mouse_Up(const MOUSE_TYPE& _eBtn, const int& _iKey);

	const bool		Set_Block(const INPUT_TYPE& _etype, const bool& _eBolck);
	const bool		Set_Block_All(const bool& _eBlock);

	const POINT		Get_Mouse_Pos();

	void			Create_MouseUI(const bool& _b);

private:
	POINT			m_ptMouse;
	bool			m_bKeyState[VK_MAX];
	bool			m_arrMouseState[(UINT)MOUSE_TYPE::TYPEEND];
	bool			m_arrBlockState[(UINT)INPUT_TYPE::TYPEEND];
	HWND			m_hWnd;
};

