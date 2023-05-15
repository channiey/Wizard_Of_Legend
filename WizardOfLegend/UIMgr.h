#pragma once

class CUI;

class CUIMgr
{
	SINGLETON(CUIMgr)

private:
	CUIMgr();
	virtual ~CUIMgr();

public:
	void		Update();
	void		Release();

private:
	CUI* Get_TargetedUI(CUI* _pParentUI); // �θ� UI ������ ������ Ÿ���� �� �ڽ�UI�� ã�Ƽ� ��ȯ�Ѵ�.
};

