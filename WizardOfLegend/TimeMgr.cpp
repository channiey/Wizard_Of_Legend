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
	// DT : 이전프레임에서 현재프레임 갱신까지 걸린 시간
	// 이전 프레임 카운팅과 현재 프레임 카운팅 값의 차이를 구한 뒤, 시간 단위로 변환하기 위해 m_llFrequency로 나누어준다)
	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;
	m_llPrevCount = m_llCurCount;

	++m_iCallCount;
	m_dAcc += m_dDT;
	if (m_dAcc >= 1.) // 누적시간이 1.f초가 되면
	{
		m_iFPS = m_iCallCount; // 1.f초 동안의 누적된 호출 횟수, 즉 약 1초동안 몇번이나 갱신되었는지 => FPS
		m_dAcc = 0.;
		m_iCallCount = 0;
	}
	m_dTotalTime += m_dDT;
}






