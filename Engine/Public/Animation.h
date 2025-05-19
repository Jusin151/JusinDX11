#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(const aiAnimation* pAIAnimation);

private:
	/* 이 애니메이션을 표현하기위해서 사용하는 뼈의 갯수 */
	_uint					m_iNumChannels;
	
	vector<class CChannel*>	m_Channels;

public:
	static CAnimation* Create(const aiAnimation* pAIAnimation);
	virtual void Free() override;
};

NS_END