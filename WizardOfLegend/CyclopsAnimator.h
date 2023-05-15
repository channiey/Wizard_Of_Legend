#pragma once
#include "Animator.h"
class CCyclopsAnimator :
	public CAnimator
{
public:
	CCyclopsAnimator(CObj* _pOwner);
	virtual ~CCyclopsAnimator();

public:
	virtual void	LateUpdate() override;
	virtual void	Play(const wstring& _strName, const bool& _bRepeat) override;
};

