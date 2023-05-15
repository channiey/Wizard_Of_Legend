#pragma once
#include "Component.h"

class CObj;
class CAnimation;

class CAnimator : public CComponent
{
public:
	CAnimator(CObj* _pOwner);
	virtual ~CAnimator();

public:
	virtual void				Init() override;
	virtual void				LateUpdate() override;
	virtual void				Render(HDC _hDC) override;
	void						Release();

public:
	const bool					PushBack_Ani(CAnimation* _pAni);
	CAnimation*					Find_Ani(const wstring& _strName);
	virtual void				Play(const wstring& _strName, const bool& _bRepeat);

public:
	const bool					Is_Use_Dir() const { return m_bUseDir; }
	void						Set_Use_Dir(const bool _bUse) { m_bUseDir = _bUse; }
protected:
	map<wstring, CAnimation*>	m_mapAni;	// ��� �ִϸ޴ϼ�
	CAnimation*					m_pCurAni;	// ���� ������� �ִϸ��̼�
	CAnimation*					m_pReservedAni; // �÷��� �õ������� ���� ���� �ִϸ��̼��� ������ �ʾ� ©�� �ִϸ��̼�
	bool						m_bReservedRepeat;
	bool						m_bRepeat;  // �ݺ� ��� ����
	bool						m_bUseDir; // �ִϸ��̼ǿ��� Dir ������ ���� �ʴ´ٸ� üũ
};

