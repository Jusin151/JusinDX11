#include "PipeLine.h"

CPipeLine::CPipeLine()
{
}

void CPipeLine::Set_Transform(D3DTS eState, _fmatrix TransformMatrix)
{
	XMStoreFloat4x4(&m_TransformationMatrices[ENUM_CLASS(eState)], TransformMatrix);
}

const _float4x4* CPipeLine::Get_Transform_Float4x4(D3DTS eState) const
{
	return &m_TransformationMatrices[ENUM_CLASS(eState)];
}

_matrix CPipeLine::Get_Transform_Matrix(D3DTS eState) const
{
	return XMLoadFloat4x4(&m_TransformationMatrices[ENUM_CLASS(eState)]);
}

const _float4* CPipeLine::Get_CamPosition() const
{
	return &m_vCamPosition;
}

void CPipeLine::Update()
{
	for (size_t i = 0; i < ENUM_CLASS(D3DTS::END); i++)
	{
		XMStoreFloat4x4(&m_TransformationMatrixInverse[i],
			XMMatrixInverse(nullptr, Get_Transform_Matrix(static_cast<D3DTS>(i))));
	}

	memcpy(&m_vCamPosition, &m_TransformationMatrixInverse[ENUM_CLASS(D3DTS::VIEW)].m[3], sizeof(_float4));
}

CPipeLine* CPipeLine::Create()
{
	return new CPipeLine();
}

void CPipeLine::Free()
{
	__super::Free();
}
