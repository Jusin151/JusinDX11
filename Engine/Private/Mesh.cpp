#include "Mesh.h"

#include "Bone.h"
#include "Shader.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CMesh::CMesh(const CMesh& Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CMesh::Initialize_Prototype(MODEL eType, const aiMesh* pAIMesh, const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	strcpy_s(m_szName, pAIMesh->mName.data);
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_iIndexStride = sizeof(_uint);
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	HRESULT hr = eType == MODEL::NONANIM ? Ready_NonAnim_Mesh(pAIMesh, PreTransformMatrix) : Ready_Anim_Mesh(pAIMesh, Bones);

	if (FAILED(hr))
		return E_FAIL;


	D3D11_BUFFER_DESC			IBBufferDesc{};
	IBBufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	IBBufferDesc.StructureByteStride = m_iIndexStride;
	IBBufferDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint	iNumIndices = { 0 };

	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[0];
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[1];
		pIndices[iNumIndices++] = pAIMesh->mFaces[i].mIndices[2];
	}

	D3D11_SUBRESOURCE_DATA		IBInitialData{};
	IBInitialData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBBufferDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);




	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CMesh::Ready_NonAnim_Mesh(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{
	m_iVertexStride = sizeof(VTXMESH);
	D3D11_BUFFER_DESC			VBBufferDesc{};
	VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	VBBufferDesc.StructureByteStride = m_iVertexStride;
	VBBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA		VBInitialData{};

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_pVertexPositions[i] = pVertices[i].vPosition;

	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_Anim_Mesh(const aiMesh* pAIMesh, const vector<class CBone*>& Bones)
{
	m_iVertexStride = sizeof(VTXANIMMESH);
	D3D11_BUFFER_DESC			VBBufferDesc{};
	VBBufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VBBufferDesc.CPUAccessFlags = /*D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE*/0;
	VBBufferDesc.StructureByteStride = m_iVertexStride;
	VBBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA		VBInitialData{};

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
	}

	/* 이 정점이 영향을 받는 뼈의 정보를 저장할 때? */
	/* 뭔말인지 알지? */



	m_iNumBones = pAIMesh->mNumBones;

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		/* 부모ㅗ 자식등의 뼈의 관계성을 표현(x) -> aiNode */
		/* 이 메시에 어떤 정점들게 영향을 줍니다. and 얼마나 영향을 줍니다. */
		aiBone* pAIBone = pAIMesh->mBones[i];

		_float4x4		OffsetMatrix;

		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));


		m_OffsetMatrices.push_back(OffsetMatrix);

		_uint	iBoneIndex = {};

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
			{
				if(true == pBone->Compare_Name(pAIBone->mName.data))
					return true;				

				++iBoneIndex;

				return false;
			});

		m_BoneIndices.push_back(iBoneIndex);	

		/* i번째 뼈가 몇개 정점에게 영향을 주는데?*/
		_uint		iNumWeights = pAIBone->mNumWeights;
		
		for (size_t j = 0; j < iNumWeights; j++)
		{
			/* i번째 뼈가 영향ㅇ르 주는 j번째 정점의 정보 */
			aiVertexWeight	AIWeight = pAIBone->mWeights[j];

			if (0.f == pVertices[AIWeight.mVertexId].vBlendWeights.x)
			{
				/* 이 메시에게 영향을 주는 뼈들 중 i번째 뼈가 이 저젬에게 영향을 주네. */
				pVertices[AIWeight.mVertexId].vBlendIndices.x = i;
				pVertices[AIWeight.mVertexId].vBlendWeights.x = AIWeight.mWeight;
			}

			else if (0.f == pVertices[AIWeight.mVertexId].vBlendWeights.y)
			{
				pVertices[AIWeight.mVertexId].vBlendIndices.y = i;
				pVertices[AIWeight.mVertexId].vBlendWeights.y = AIWeight.mWeight;
			}

			else if (0.f == pVertices[AIWeight.mVertexId].vBlendWeights.z)
			{
				pVertices[AIWeight.mVertexId].vBlendIndices.z = i;
				pVertices[AIWeight.mVertexId].vBlendWeights.z = AIWeight.mWeight;
			}

			else if (0.f == pVertices[AIWeight.mVertexId].vBlendWeights.w)
			{
				pVertices[AIWeight.mVertexId].vBlendIndices.w = i;
				pVertices[AIWeight.mVertexId].vBlendWeights.w = AIWeight.mWeight;
			}
		}
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		_uint	iBoneIndex = {};

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
			{
				if (true == pBone->Compare_Name(m_szName))
					return true;

				++iBoneIndex;

				return false;
			});


		m_BoneIndices.push_back(iBoneIndex);

		_float4x4		OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
		m_OffsetMatrices.push_back(OffsetMatrix);
	}

	for (_uint i = 0; i < m_iNumVertices; ++i)
		m_pVertexPositions[i] = pVertices[i].vPosition;

	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBBufferDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Bind_Bone_Matrices(CShader* pShader, const _char* pConstantName, const vector<class CBone*>& Bones)
{
	ZeroMemory(m_BoneMatrices, sizeof(_float4x4) * g_iMaxNumBones);

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		XMStoreFloat4x4(&m_BoneMatrices[i],
			XMLoadFloat4x4(&m_OffsetMatrices[i]) * 
			XMLoadFloat4x4(Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix()));
	}

	return pShader->Bind_Matrices(pConstantName, m_BoneMatrices, m_iNumBones);	
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL eType, const aiMesh* pAIMesh, const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pAIMesh, Bones, PreTransformMatrix)))
	{
		MSG_BOX("Failed to Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();


}
