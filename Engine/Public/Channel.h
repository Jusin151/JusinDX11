#pragma once

#include "Base.h"

/* 특정 애니메이션을 구동하기위해서 필요한, 뼈의 시간당 상태정보*/

NS_BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(const aiNodeAnim* aiChannel);

private:
	_char				m_szName[MAX_PATH] = {};
	_uint				m_iNumKeyFrames;
	vector<KEYFRAME>	m_KeyFrames;
	

public:
	static CChannel* Create(const aiNodeAnim* aiChannel);
	virtual void Free() override;

};

NS_END