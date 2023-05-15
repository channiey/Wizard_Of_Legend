#pragma once
/*

이닛에서 보스 생성한다음 비활성화 하고 활성화 함수 호출하도록

*/

class CIceBoss;
class CWarningUI;
class CObj;


class CBossStageMgr
{
	SINGLETON(CBossStageMgr)

private:
	CBossStageMgr();
	virtual ~CBossStageMgr();

public:
	const bool							Init();
	void								Update();
	void								LateUpdate();
	void								Render(HDC _hDC) {}
	void								Release();

public:
	const bool							Display_Warning_UI(); // 보스 등장 전, 워링 표시를 띄운다.
	const bool							Evt_Active_Boss(); // 비활성화 되어 있던 보스를 활성화 시킨다.
	const bool							Camera_Move_To_Boss(); // 카메라 타겟을 보스로 이동한다.
	const bool							Camera_Move_To_Player(); // 카메라 타겟을 플레이어로 이동한다.
	const bool							Out_Scene();


private:
	CIceBoss*							m_pBoss;
	CWarningUI*							m_pWarningUI;

	float								m_fAcc;
	
	
	float								m_fWarningStartTime;
	bool								m_bWarning;

	bool								m_bSlow;

	bool								m_bOutScene;
	bool								m_bEnd;

};

