#pragma once
/*

�̴ֿ��� ���� �����Ѵ��� ��Ȱ��ȭ �ϰ� Ȱ��ȭ �Լ� ȣ���ϵ���

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
	const bool							Display_Warning_UI(); // ���� ���� ��, ���� ǥ�ø� ����.
	const bool							Evt_Active_Boss(); // ��Ȱ��ȭ �Ǿ� �ִ� ������ Ȱ��ȭ ��Ų��.
	const bool							Camera_Move_To_Boss(); // ī�޶� Ÿ���� ������ �̵��Ѵ�.
	const bool							Camera_Move_To_Player(); // ī�޶� Ÿ���� �÷��̾�� �̵��Ѵ�.
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

