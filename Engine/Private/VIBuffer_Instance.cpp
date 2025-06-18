#include "VIBuffer_Instance.h"

CVIBuffer_Instance::CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer { pDevice, pContext }
{
}

CVIBuffer_Instance::CVIBuffer_Instance(const CVIBuffer_Instance& Prototype)
	: CVIBuffer{ Prototype }
	, m_pVBInstance { Prototype.m_pVBInstance }
	, m_VBInstanceDesc { Prototype.m_VBInstanceDesc }
	, m_VBInstanceSubresourceData{ Prototype.m_VBInstanceSubresourceData }
	, m_iNumInstance { Prototype.m_iNumInstance }
	, m_iVertexInstanceStride { Prototype.m_iVertexInstanceStride }
	, m_pVertexInstances { Prototype.m_pVertexInstances } 
	, m_iNumIndexPerInstance { Prototype.m_iNumIndexPerInstance }
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Instance::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Instance::Initialize(void* pArg)
{
	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstanceDesc, &m_VBInstanceSubresourceData, &m_pVBInstance)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CVIBuffer_Instance::Bind_Buffers()
{
	ID3D11Buffer* pVertexBuffers[] = {
		   m_pVB,
		   m_pVBInstance,		   
	};

	_uint		iVertexStrides[] = {
		m_iVertexStride,
		m_iVertexInstanceStride

	};

	_uint		iOffsets[] = {
		0, 
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitiveTopology);

	return S_OK;
}

HRESULT CVIBuffer_Instance::Render()
{
	

	m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_iNumInstance, 0, 0, 0);


	return S_OK;
}

void CVIBuffer_Instance::Drop(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXMATRIX* pVertices = static_cast<VTXMATRIX*>(SubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vTranslation.y -= 2.f * fTimeDelta;
	}

	m_pContext->Unmap(m_pVBInstance, 0);

}

void CVIBuffer_Instance::Spread(_float fTimeDelta)
{
}

void CVIBuffer_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVertexInstances);

	Safe_Release(m_pVBInstance);

}
