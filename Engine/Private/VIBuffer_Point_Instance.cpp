#include "VIBuffer_Point_Instance.h"

#include "GameInstance.h"

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer_Instance{ pDevice, pContext }
{
}

CVIBuffer_Point_Instance::CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& Prototype)
	: CVIBuffer_Instance{ Prototype }
	, m_pVertexInstances { Prototype.m_pVertexInstances }
	, m_vPivot { Prototype.m_vPivot }
	, m_pSpeeds{ Prototype.m_pSpeeds }	
	, m_isLoop { Prototype.m_isLoop }
{
	
}

HRESULT CVIBuffer_Point_Instance::Initialize_Prototype(const INSTANCE_DESC* pArg)
{
	const POINT_INSTANCE_DESC* pDesc = static_cast<const POINT_INSTANCE_DESC*>(pArg);

	m_vPivot = pDesc->vPivot;
	m_isLoop = pDesc->isLoop;
	m_iNumIndexPerInstance = 1;
	m_iVertexInstanceStride = sizeof(VTXPOS_PARTICLE_INSTANCE);
	m_iNumInstance = pDesc->iNumInstance;

	m_iNumVertexBuffers = 2;
	m_iNumVertices = 1;
	m_iVertexStride = sizeof(VTXPOS);
	m_iNumIndices = m_iNumIndexPerInstance;

	m_iIndexStride = sizeof(_ushort);
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;


#pragma region VERTEXBUFFER
	D3D11_BUFFER_DESC			VBBufferDesc{};
	VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	VBBufferDesc.StructureByteStride = m_iVertexStride;
	VBBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA		VBInitialData{};

	VTXPOS* pVertices = new VTXPOS[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	pVertices[0].vPosition = _float3(0.0f, 0.0f, 0.f);

	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_pVertexPositions[i] = pVertices[i].vPosition;

	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion 

#pragma region INDEXBUFFER

	D3D11_BUFFER_DESC			IBBufferDesc{};
	IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	IBBufferDesc.StructureByteStride = m_iIndexStride;
	IBBufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);	

	D3D11_SUBRESOURCE_DATA		IBInitialData{};
	IBInitialData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion 

#pragma region INSTANCEBUFFER
	m_VBInstanceDesc.ByteWidth = m_iNumInstance * m_iVertexInstanceStride;
	m_VBInstanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstanceDesc.StructureByteStride = m_iVertexInstanceStride;
	m_VBInstanceDesc.MiscFlags = 0;

	m_pVertexInstances = new VTXPOS_PARTICLE_INSTANCE[m_iNumInstance];
	m_pSpeeds = new _float[m_iNumInstance];

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		m_pSpeeds[i] = m_pGameInstance->Compute_Random(pDesc->vSpeed.x, pDesc->vSpeed.y);
		_float	fSize = m_pGameInstance->Compute_Random(pDesc->vSize.x, pDesc->vSize.y);

		m_pVertexInstances[i].vRight = _float4(fSize, 0.f, 0.f, 0.f);
		m_pVertexInstances[i].vUp = _float4(0.f, fSize, 0.f, 0.f);
		m_pVertexInstances[i].vLook = _float4(0.f, 0.f, fSize, 0.f);

		m_pVertexInstances[i].vTranslation = _float4(
			m_pGameInstance->Compute_Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f),
			m_pGameInstance->Compute_Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f),
			m_pGameInstance->Compute_Random(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f),
			1.f
		);

		m_pVertexInstances[i].vLifeTime = _float2(
			m_pGameInstance->Compute_Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y),
			0.f
		);	
	}
	
	m_VBInstanceSubresourceData.pSysMem = m_pVertexInstances;



#pragma endregion 




	return S_OK;
}

HRESULT CVIBuffer_Point_Instance::Initialize(void* pArg)
{
	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstanceDesc, &m_VBInstanceSubresourceData, &m_pVBInstance)))
		return E_FAIL;

	return S_OK;
}



void CVIBuffer_Point_Instance::Drop(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;

		pVertices[i].vTranslation.y -= m_pSpeeds[i] * fTimeDelta;

		if (true == m_isLoop && 
			pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			pVertices[i].vLifeTime.y = 0.f;
			pVertices[i].vTranslation.y = m_pVertexInstances[i].vTranslation.y;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Point_Instance::Spread(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE	SubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXPOS_PARTICLE_INSTANCE* pVertices = static_cast<VTXPOS_PARTICLE_INSTANCE*>(SubResource.pData);

	_vector vDir = {};

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pVertices[i].vLifeTime.y += fTimeDelta;

		vDir = XMVectorSetW(XMVector3Normalize(XMLoadFloat3(&m_vPivot) - XMLoadFloat4(&m_pVertexInstances[i].vTranslation)), 0.f);

		XMStoreFloat4(&pVertices[i].vTranslation, 
			XMLoadFloat4(&pVertices[i].vTranslation) - (vDir * m_pSpeeds[i] * fTimeDelta));

		if (true == m_isLoop &&
			pVertices[i].vLifeTime.y >= pVertices[i].vLifeTime.x)
		{
			pVertices[i].vLifeTime.y = 0.f;
			pVertices[i].vTranslation = m_pVertexInstances[i].vTranslation;
		}
	}

	m_pContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_Point_Instance* CVIBuffer_Point_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pArg)
{
	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Point_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Point_Instance::Clone(void* pArg)
{
	CVIBuffer_Point_Instance* pInstance = new CVIBuffer_Point_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Point_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Point_Instance::Free()
{
	__super::Free();

	if(false == m_isCloned)
	{
		Safe_Delete_Array(m_pVertexInstances);
		Safe_Delete_Array(m_pSpeeds);
	}
}
