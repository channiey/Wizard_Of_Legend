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
	// ��ο� ����� ������ �о�� ��Ʈ�� ��������� �����Ѵ�.

	HDC	hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,		// ���α׷��� �ν��Ͻ� �ڵ�, �̹����� ���α׷��� �ƴ� ��ηκ��� ���� �о���� ���̾ NULL �� ����
		pFilePath,		// ������ ���
		IMAGE_BITMAP,  // � Ÿ���� �о���� ���ΰ�
		0,				// ����, ���� ������
		0,				// ���Ϸ� ���� ���� �о�� ���̱� ������ ������ �ʿ� ����
		LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : ���Ϸ� ���� �̹��� �ҷ����� �ɼ�, LR_CREATEDIBSECTION : �о�� ������ DIB ���·� ��ȯ

												// SelectObject : GDI ������Ʈ�� �����ϱ� ���� ������ ������ �ִ� ������Ʈ �ڵ��� ��ȯ

	m_hOldmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	GetObject(m_hBitmap, sizeof(BITMAP), &m_bitInfo);
}

void CMyBmp::Release(void)
{
	SelectObject(m_hMemDC, m_hOldmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}