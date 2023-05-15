#pragma once

/* ���� ������ ���� */
#define NO_COPY(className)	private :\
							className(const className& obj);\
							className* operator = (className* obj)\
							{return nullptr;}

/* Singleton */
#define SINGLETON(ClassName)	\
private:	\
NO_COPY(ClassName);\
public:\
static ClassName* Get_Inst()\
{\
	if(m_pInst == NULL)\
		m_pInst = new ClassName;\
	return m_pInst;	\
}\
void Destroy_Inst()\
{\
	delete m_pInst;\
	m_pInst = nullptr;\
}\
private:\
static ClassName* m_pInst;


///* API �ܼ� ��� */
//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif


/* �޸� ���� üũ */
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif