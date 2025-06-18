#include "VIBuffer_Instance.h"

CVIBuffer_Instance::CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer { pDevice, pContext }
{
}

CVIBuffer_Instance::CVIBuffer_Instance(const CVIBuffer_Instance& Prototype)
	: CVIBuffer{ Prototype }
	, m_VBInstanceDesc { Prototype.m_VBInstanceDesc }
	, m_VBInstanceSubresourceData{ Prototype.m_VBInstanceSubresourceData }
	, m_iNumInstance { Prototype.m_iNumInstance }
	, m_iVertexInstanceStride { Prototype.m_iVertexInstanceStride }
	, m_pVertexInstances { Prototype.m_pVertexInstances } 
{
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

void CVIBuffer_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVertexInstances);

	Safe_Release(m_pVBInstance);

}
