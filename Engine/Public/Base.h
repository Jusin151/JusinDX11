#pragma once

#include "Engine_Defines.h"

/* 모든 클래스들의 부모가 되는 클래스다. */
/* 레퍼런스카운트를 관리하기위한 기능을 부여한다. (AddRef, Release) */
NS_BEGIN(Engine)

class ENGINE_DLL CBase
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* 레퍼런스 카운트를 증가시킨다. */
	unsigned int AddRef();

	/* 레퍼런스 카운트를 감소시킨다. or 삭제한다. */
	unsigned int Release();

protected:
	unsigned int		m_iRefCnt = { };

public:
	virtual void Free();

};

NS_END


