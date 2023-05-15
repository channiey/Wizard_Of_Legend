#pragma once
class CTimeMgr
{
	SINGLETON(CTimeMgr)

private:
	CTimeMgr();
	virtual ~CTimeMgr();

public:
	const bool		Init();
	void			Update();

public:
	float			Get_fDT() const			{ return (float)m_dDT; }
	double			Get_DT() const			{ return m_dDT; }
	UINT			Get_FPS() const			{ return m_iFPS; }
	double			Get_TotalTime() const { return m_dTotalTime; }
private:

	LARGE_INTEGER		m_llCurCount;
	LARGE_INTEGER		m_llPrevCount;
	LARGE_INTEGER		m_llFrequency;

	double				m_dDT;			// 프레임 사이의 시간 값
	double				m_dAcc;			// 1초 체크를 위한 누적 시간
	UINT				m_iCallCount;	// 초당 호출 횟수 체크
	UINT				m_iFPS;			// 초당 호출 횟수

	LARGE_INTEGER		m_llCurCnt;
	LARGE_INTEGER		m_llPrevCnt;

	double				m_dTotalTime;
	double				m_dLimit;
};

