#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}


HRESULT CAnimation::Initialize(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones)
{
	/* 이 애니메이션이 컨트롤해야하는 뼈의 갯수 */
	m_iNumChannels = pAIAnimation->mNumChannels;

	m_fTickPerSecond = pAIAnimation->mTicksPerSecond;
	m_fDuration = pAIAnimation->mDuration;

	/* 각 뼈의 정보를 새엇ㅇ한다. */
	for (size_t i = 0; i < m_iNumChannels; i++)
	{		
		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], Bones);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

    return S_OK;
}

void CAnimation::Update_Bones(_float fTimeDelta, const vector<CBone*>& Bones, _bool isLoop)
{
	m_fCurrentTrackPosition += m_fTickPerSecond * fTimeDelta;

	if (m_fCurrentTrackPosition >= m_fDuration)
	{
		m_fCurrentTrackPosition = m_fDuration;

		if (true == isLoop)
			m_fCurrentTrackPosition = 0.f;			
	}

	for (auto& pChannel : m_Channels)
	{
		pChannel->Update_TransformationMatrix(m_fCurrentTrackPosition, Bones);
	}
}

CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, Bones)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	__super::Free();

	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();


}
