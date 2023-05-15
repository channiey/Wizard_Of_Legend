#pragma once
#include "Animator.h"
class CIcebossAnimator :
	public CAnimator
{
public:
	CIcebossAnimator(CObj* _pOwner);
	virtual ~CIcebossAnimator();

public:
	virtual void	LateUpdate() override;
	virtual void	Play(const wstring& _strName, const bool& _bRepeat) override;
};

