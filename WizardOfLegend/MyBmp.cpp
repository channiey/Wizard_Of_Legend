#include "stdafx.h"
#include "MyBmp.h"


CMyBmp::CMyBmp()
{
}


CMyBmp::~CMyBmp()
{
}

void CMyBmp::Load_Bmp(const TCHAR * pFilePath)
{
	// 경로에 저장된 파일을 읽어와 비트맵 멤버변수에 저장한다.

	HDC	hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,		// 프로그램의 인스턴스 핸들, 이미지를 프로그램이 아닌 경로로부터 직접 읽어들일 것이어서 NULL 값 전달
		pFilePath,		// 파일의 경로
		IMAGE_BITMAP,  // 어떤 타입을 읽어들일 것인가
		0,				// 가로, 세로 사이즈
		0,				// 파일로 부터 직접 읽어올 것이기 때문에 지정할 필요 없음
		LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : 파일로 부터 이미지 불러오는 옵션, LR_CREATEDIBSECTION : 읽어온 파일을 DIB 형태로 변환

												// SelectObject : GDI 오브젝트를 선택하기 전에 기존에 가지고 있던 오브젝트 핸들을 반환

	m_hOldmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_bitInfo);
}

void CMyBmp::Release(void)
{
	SelectObject(m_hMemDC, m_hOldmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
