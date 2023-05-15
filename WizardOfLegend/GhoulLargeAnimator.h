#pragma once
#include "Animator.h"
class CGhoulLargeAnimator :
	public CAnimator
{
public:
	CGhoulLargeAnimator(CObj* _pOwner);
	~CGhoulLargeAnimator();

public:
	virtual void	LateUpdate() override;
	virtual void	Play(const wstring& _strName, const bool& _bRepeat) override;
};

