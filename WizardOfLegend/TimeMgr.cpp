#include "stdafx.h"
#include "TimeMgr.h"

CTimeMgr* CTimeMgr::m_pInst = nullptr;

CTimeMgr::CTimeMgr()
{

}

CTimeMgr::~CTimeMgr()
{

}

const bool CTimeMgr::Init()
{
	QueryPerformanceCounter(&m_llPrevCount);
	QueryPerformanceFrequency(&m_llFrequency);

	m_dDT = 0.;
	m_dAcc = 0.;
	m_iCallCount = 0;
	m_iFPS = 0;


	m_dTotalTime = 0.f;
	m_dLimit = 0.016f;

	return true;
}

void CTimeMgr::Update()
{
	QueryPerformanceCounter(&m_llCurCount);
	// DT : ���������ӿ��� ���������� ���ű��� �ɸ� �ð�
	// ���� ������ ī���ð� ���� ������ ī���� ���� ���̸� ���� ��, �ð� ������ ��ȯ�ϱ� ���� m_llFrequency�� �������ش�)
	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;
	m_llPrevCount = m_llCurCount;

	++m_iCallCount;
	m_dAcc += m_dDT;
	if (m_dAcc >= 1.) // �����ð��� 1.f�ʰ� �Ǹ�
	{
		m_iFPS = m_iCallCount; // 1.f�� ������ ������ ȣ�� Ƚ��, �� �� 1�ʵ��� ����̳� ���ŵǾ����� => FPS
		m_dAcc = 0.;
		m_iCallCount = 0;
	}
	m_dTotalTime += m_dDT;
}






