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

		_bool		isDraw[4] = { true, true, true, true };

		for (size_t i = 0; i < NEIGHBOR_END; i++)
		{
			if(nullptr != m_Neighbors[i])
				isDraw[i] = m_Neighbors[i]->isDraw(pGameInstance, pVertexPositions);
		}

		if (true == isDraw[NEIGHBOR_LEFT] &&
			true == isDraw[NEIGHBOR_TOP] &&
			true == isDraw[NEIGHBOR_RIGHT] &&
			true == isDraw[NEIGHBOR_BOTTOM])
		{
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
		_uint		iLC, iTC, iRC, iBC;

		iLC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_LB]) >> 1;
		iTC = (m_iCorners[CORNER_LT] + m_iCorners[CORNER_RT]) >> 1;
		iRC = (m_iCorners[CORNER_RT] + m_iCorners[CORNER_RB]) >> 1;
		iBC = (m_iCorners[CORNER_LB] + m_iCorners[CORNER_RB]) >> 1;


		if (true == isIn[0] ||
			true == isIn[1] ||
			true == isIn[2])
		{
			if (false == isDraw[NEIGHBOR_TOP])
			{
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LT];
				pIndices[(*pNumIndices)++] = iTC;
				pIndices[(*pNumIndices)++] = m_iCenter;

				pIndices[(*pNumIndices)++] = m_iCenter;
				pIndices[(*pNumIndices)++] = iTC;
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RT];
			}
			else
			{
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LT];
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RT];
				pIndices[(*pNumIndices)++] = m_iCenter;
			}

			if (false == isDraw[NEIGHBOR_RIGHT])
			{
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RT];
				pIndices[(*pNumIndices)++] = iRC;
				pIndices[(*pNumIndices)++] = m_iCenter;

				pIndices[(*pNumIndices)++] = m_iCenter;
				pIndices[(*pNumIndices)++] = iRC;
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RB];
			}
			else
			{
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RT];
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RB];
				pIndices[(*pNumIndices)++] = m_iCenter;
			}
		}

		if (true == isIn[0] ||
			true == isIn[2] ||
			true == isIn[3])
		{
			if (false == isDraw[NEIGHBOR_LEFT])
			{
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LT];
				pIndices[(*pNumIndices)++] = m_iCenter;
				pIndices[(*pNumIndices)++] = iLC;

				pIndices[(*pNumIndices)++] = iLC;
				pIndices[(*pNumIndices)++] = m_iCenter;
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LB];
			}
			else
			{
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LT];
				pIndices[(*pNumIndices)++] = m_iCenter;
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LB];
			}

			if (false == isDraw[NEIGHBOR_BOTTOM])
			{
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LB];
				pIndices[(*pNumIndices)++] = m_iCenter;
				pIndices[(*pNumIndices)++] = iBC;

				pIndices[(*pNumIndices)++] = iBC;
				pIndices[(*pNumIndices)++] = m_iCenter;
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RB];
			}
			else
			{
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_LB];
				pIndices[(*pNumIndices)++] = m_iCenter;
				pIndices[(*pNumIndices)++] = m_iCorners[CORNER_RB];
			}
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

void CQuadTree::Make_Neighbors()
{
	if (nullptr == m_Children[CORNER_LT]->m_Children[CORNER_LT])
		return;

	m_Children[CORNER_LT]->m_Neighbors[NEIGHBOR_RIGHT] = m_Children[CORNER_RT];
	m_Children[CORNER_LT]->m_Neighbors[NEIGHBOR_BOTTOM] = m_Children[CORNER_LB];

	m_Children[CORNER_RT]->m_Neighbors[NEIGHBOR_LEFT] = m_Children[CORNER_LT];
	m_Children[CORNER_RT]->m_Neighbors[NEIGHBOR_BOTTOM] = m_Children[CORNER_RB];

	m_Children[CORNER_RB]->m_Neighbors[NEIGHBOR_LEFT] = m_Children[CORNER_LB];
	m_Children[CORNER_RB]->m_Neighbors[NEIGHBOR_TOP] = m_Children[CORNER_RT];

	m_Children[CORNER_LB]->m_Neighbors[NEIGHBOR_RIGHT] = m_Children[CORNER_RB];
	m_Children[CORNER_LB]->m_Neighbors[NEIGHBOR_TOP] = m_Children[CORNER_LT];

	if (nullptr != m_Neighbors[NEIGHBOR_RIGHT])
	{
		m_Children[CORNER_RT]->m_Neighbors[NEIGHBOR_RIGHT] = m_Neighbors[NEIGHBOR_RIGHT]->m_Children[CORNER_LT];
		m_Children[CORNER_RB]->m_Neighbors[NEIGHBOR_RIGHT] = m_Neighbors[NEIGHBOR_RIGHT]->m_Children[CORNER_LB];
	}

	if (nullptr != m_Neighbors[NEIGHBOR_BOTTOM])
	{
		m_Children[CORNER_LB]->m_Neighbors[NEIGHBOR_BOTTOM] = m_Neighbors[NEIGHBOR_BOTTOM]->m_Children[CORNER_LT];
		m_Children[CORNER_RB]->m_Neighbors[NEIGHBOR_BOTTOM] = m_Neighbors[NEIGHBOR_BOTTOM]->m_Children[CORNER_RT];
	}

	if (nullptr != m_Neighbors[NEIGHBOR_LEFT])
	{
		m_Children[CORNER_LT]->m_Neighbors[NEIGHBOR_LEFT] = m_Neighbors[NEIGHBOR_LEFT]->m_Children[CORNER_RT];
		m_Children[CORNER_LB]->m_Neighbors[NEIGHBOR_LEFT] = m_Neighbors[NEIGHBOR_LEFT]->m_Children[CORNER_RB];
	}

	if (nullptr != m_Neighbors[NEIGHBOR_TOP])
	{
		m_Children[CORNER_LT]->m_Neighbors[NEIGHBOR_TOP] = m_Neighbors[NEIGHBOR_TOP]->m_Children[CORNER_LB];
		m_Children[CORNER_RT]->m_Neighbors[NEIGHBOR_TOP] = m_Neighbors[NEIGHBOR_TOP]->m_Children[CORNER_RB];
	}

	for (size_t i = 0; i < CORNER_END; i++)
	{
		m_Children[i]->Make_Neighbors();
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

	for (auto& pNeighbor : m_Neighbors)
		Safe_Release(pNeighbor);


}
