#pragma once
#include "Animator.h"
class CPlayerAnimator : public CAnimator
{
public:
	CPlayerAnimator(CObj* _pOwner);
	virtual ~CPlayerAnimator();

public:
	virtual void	LateUpdate() override;
	virtual void	Play(const wstring& _strName, const bool& _bRepeat) override;

};

