#pragma once
#include "Animator.h"
class CBolbAnimator :
	public CAnimator
{
public:
	CBolbAnimator(CObj* _pOwner);
	virtual ~CBolbAnimator();

public:
	virtual void	LateUpdate() override;
	virtual void	Play(const wstring& _strName, const bool& _bRepeat) override;
};

