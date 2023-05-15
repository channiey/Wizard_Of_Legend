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

	const bool							Delete_Obj(CObj* _pObj); // 메모리 해제 및 실제 삭제 수행
	const bool							Remove_Obj(CObj* _pObj); // 풀에 반환하는 오브젝트를 Obj리스트에서만 지우는 용도

private:
	const bool							Out_Of_Screen(CObj* _pObj);


private:
	list<CObj*>							m_ObjList[(UINT)OBJ_TYPE::TYPEEND];
	list<CObj*>							m_RenderSort[(UINT)RENDER_ORDER::TYPEEND];
};

