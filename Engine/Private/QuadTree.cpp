#include "QuadTree.h"

#include "GameInstance.h"

CQuadTree::CQuadTree()
{
}

HRESULT CQuadTree::Initialize(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	m_iCorners[CORNER_LT] = iLT;
	m_iCorners[CORNER_RT] = iRT;
	m_iCorners[CORNER_RB] = iRB;
	m_iCorners[CORNER_LB] = iLB;

	if (1 == m_iCorners[CORNER_RT] - m_iCorners[CORNER_LT])
		return S_OK;

	m_iCenter = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_RB]) >> 1;

	_uint		iLC, iTC, iRC, iBC;

	iLC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_LB]) >> 1;
	iTC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_RT]) >> 1;
	iRC = (m_iCorners[CORNER_RT] + m_iCorners[CORNER_RB]) >> 1;
	iBC = (m_iCorners[CORNER_LB] + m_iCorners[CORNER_RB]) >> 1;

	m_Children[CORNER_LT] = CQuadTree::Create(m_iCorners[CORNER_LT], iTC, m_iCenter, iLC);
	m_Children[CORNER_RT] = CQuadTree::Create(iTC, m_iCorners[CORNER_RT], iRC, m_iCenter);
	m_Children[CORNER_RB] = CQuadTree::Create(m_iCenter, iRC, m_iCorners[CORNER_RB], iBC);
	m_Children[CORNER_LB] = CQuadTree::Create(iLC, m_iCenter, iBC, m_iCorners[CORNER_LB]);

    return S_OK;
}

void CQuadTree::Culling(CGameInstance* pGameInstance, const _float3* pVertexPositions, _uint* pIndices, _uint* pNumIndices)
{
	if (nullptr == m_Children[CORNER_LT] ||
		true == isDraw(pGameInstance, pVertexPositions))
	{
		_bool		isIn[4] = {
			pGameInstance->isIn_Frustum_LocalSpace(XMLoadFloat3(&pVertexPositions[m_iCorners[CORNER_LT]]), 0.0f),
			pGameInstance->isIn_Frustum_LocalSpace(XMLoadFloat3(&pVertexPositions[m_iCorners[CORNER_RT]]), 0.0f),
			pGameInstance->isIn_Frustum_LocalSpace(XMLoadFloat3(&pVertexPositions[m_iCorners[CORNER_RB]]), 0.0f),
			pGameInstance->isIn_Frustum_LocalSpace(XMLoadFloat3(&pVertexPositions[m_iCorners[CORNER_LB]]), 0.0f),
		};

		if (true == isIn[0] ||
			true == isIn[1] ||
			true == isIn[2])
		{
			pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LT];
			pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RT];
			pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RB];
		}

		if (true == isIn[0] ||
			true == isIn[2] ||
			true == isIn[3])
		{
			pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LT];
			pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RB];
			pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LB];
		}

		return;
	}

	_float		fRange = XMVectorGetX(XMVector3Length(XMLoadFloat3(&pVertexPositions[m_iCorners[CORNER_LT]]) - XMLoadFloat3(&pVertexPositions[m_iCenter])));
	if (true == pGameInstance->isIn_Frustum_LocalSpace(XMLoadFloat3(&pVertexPositions[m_iCenter]), fRange))
	{
		for (size_t i = 0; i < CORNER_END; i++)
		{
			m_Children[i]->Culling(pGameInstance, pVertexPositions, pIndices, pNumIndices);
		}
	}	
}

_bool CQuadTree::isDraw(CGameInstance* pGameInstance, const _float3* pVertexPositions)
{
	_float		fCamDistance = XMVectorGetX(XMVector3Length(XMLoadFloat4(pGameInstance->Get_CamPosition()) - XMLoadFloat3(&pVertexPositions[m_iCenter])));

	if (fCamDistance * 0.2f > m_iCorners[CORNER_RT] - m_iCorners[CORNER_LT])
		return true;

	return false;
}

CQuadTree* CQuadTree::Create(_uint iLT, _uint iRT, _uint iRB, _uint iLB)
{
	CQuadTree* pInstance = new CQuadTree();

	if (FAILED(pInstance->Initialize(iLT, iRT, iRB, iLB)))
	{
		MSG_BOX("Failed to Created : CQuadTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CQuadTree::Free()
{
    __super::Free();

	for (auto& pChild : m_Children)
		Safe_Release(pChild);


}
