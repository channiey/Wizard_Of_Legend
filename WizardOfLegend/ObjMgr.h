#pragma once

class CObj;

class CObjMgr
{
	SINGLETON(CObjMgr)

private:
	CObjMgr();
	virtual ~CObjMgr();

public:
	const bool							Init();
	void								Update();
	void								LateUpdate();
	void								Render(HDC _hDC);
	void								Release();

public:
	list<CObj*>*						Get_ObjList(const OBJ_TYPE& _eType);

	CObj*								Get_Player();

	const bool							Add_Obj(const OBJ_TYPE& _eType, CObj* const _pObj);
	const bool							Add_Obj(CObj* const _pObj);

	const bool							Delete_Obj(CObj* _pObj); // �޸� ���� �� ���� ���� ����
	const bool							Remove_Obj(CObj* _pObj); // Ǯ�� ��ȯ�ϴ� ������Ʈ�� Obj����Ʈ������ ����� �뵵

private:
	const bool							Out_Of_Screen(CObj* _pObj);


private:
	list<CObj*>							m_ObjList[(UINT)OBJ_TYPE::TYPEEND];
	list<CObj*>							m_RenderSort[(UINT)RENDER_ORDER::TYPEEND];
};

