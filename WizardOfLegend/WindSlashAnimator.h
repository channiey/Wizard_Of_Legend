#pragma once
#include "Animator.h"
class CWindSlashAnimator :
	public CAnimator
{
public:
	CWindSlashAnimator(CObj* _pOwner);
	virtual ~CWindSlashAnimator();

public:
	virtual void	LateUpdate() override;
	virtual void	Play(const wstring& _strName, const bool& _bRepeat) override;
};

