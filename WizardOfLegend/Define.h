#pragma once


#define START_POS_VILLAGE		VEC2(0, 0)
#define START_POS_FIELD			VEC2(3173, 1000)
#define START_POS_BOSS			VEC2(1100, 1200)





#define PURE						=0

#define WINCX						1280
#define WINCY						720

#define MAPCX						6128
#define MAPCY						3688

#define UNIT						48

#define	VK_MAX						0xff

#define PI							3.1415926535f

#define GRAVITY						VEC2(0.f, 9.8f)


#define DT							CTimeMgr::Get_Inst()->Get_fDT()

#define DF_SCALE					VEC2(50, 50)
#define DF_POS						VEC2(WINCX * 0.5f, WINCY * 0.5f)

#define	DF_GOLD						100

#define DF_SPEED					500.f
#define DF_DASHSPEED				1600.f
#define DF_KNOCKBACK_SPEED			700.f
#define DF_KNOCKBACK_TIME			0.5f
#define DF_HP						100.f
#define DF_AD						5.f
#define DF_MASS						10.f

#define VEC2_UP						VEC2(0.f	,	-1.f)
#define VEC2_DOWN					VEC2(0.f	,	1.f)
#define VEC2_LEFT					VEC2(-1.f	,	0.f)
#define VEC2_RIGHT					VEC2(1.f	,	0.f)
#define VEC2_ZERO					VEC2(0.f	,	0.f)
#define VEC2_ONE					VEC2(1.f	,	1.f)
#define VEC2_SCREEN_CENTER			VEC2(WINCX * 0.5f, WINCY * 0.5f)