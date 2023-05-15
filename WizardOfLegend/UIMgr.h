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
	CUI* Get_TargetedUI(CUI* _pParentUI); // 부모 UI 내에서 실제로 타겟팅 된 자식UI를 찾아서 반환한다.
};

