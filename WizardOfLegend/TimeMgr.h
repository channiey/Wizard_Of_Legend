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

	double				m_dDT;			// ������ ������ �ð� ��
	double				m_dAcc;			// 1�� üũ�� ���� ���� �ð�
	UINT				m_iCallCount;	// �ʴ� ȣ�� Ƚ�� üũ
	UINT				m_iFPS;			// �ʴ� ȣ�� Ƚ��

	LARGE_INTEGER		m_llCurCnt;
	LARGE_INTEGER		m_llPrevCnt;

	double				m_dTotalTime;
	double				m_dLimit;
};

