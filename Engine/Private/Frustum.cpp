#include "Frustum.h"

#include "GameInstance.h"

CFrustum::CFrustum()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CFrustum::Initialize()
{
	m_vOriginalPoints[0] = _float4(-1.f, 1.f, 0.f, 1.f);
	m_vOriginalPoints[1] = _float4(1.f, 1.f, 0.f, 1.f);
	m_vOriginalPoints[2] = _float4(1.f, -1.f, 0.f, 1.f);
	m_vOriginalPoints[3] = _float4(-1.f, -1.f, 0.f, 1.f);

	m_vOriginalPoints[4] = _float4(-1.f, 1.f, 1.f, 1.f);
	m_vOriginalPoints[5] = _float4(1.f, 1.f, 1.f, 1.f);
	m_vOriginalPoints[6] = _float4(1.f, -1.f, 1.f, 1.f);
	m_vOriginalPoints[7] = _float4(-1.f, -1.f, 1.f, 1.f);

	return S_OK;
}

void CFrustum::Transform_ToWorldSpace()
{
	_matrix		ProjMatrixInverse = m_pGameInstance->Get_Transform_Matrix_Inv(D3DTS::PROJ);
	_matrix		ViewMatrixInverse = m_pGameInstance->Get_Transform_Matrix_Inv(D3DTS::VIEW);

	_vector		vPoints[8];

	for (size_t i = 0; i < 8; i++)
	{
		vPoints[i] = XMVector3TransformCoord(XMLoadFloat4(&m_vOriginalPoints[i]), ProjMatrixInverse);
		XMStoreFloat4(&m_vWorldPoints[i], XMVector3TransformCoord(vPoints[i], ViewMatrixInverse));
	}

	Make_Plane(m_vWorldPoints, m_vWorldPlanes);
	
}

void CFrustum::Transform_ToLocalSpace(_fmatrix WorldMatrix)
{
	_matrix		WorldMatrixInverse = XMMatrixInverse(nullptr, WorldMatrix);
	_float4		vPoints[8];

	for (size_t i = 0; i < 8; i++)
	{
		XMStoreFloat4(&vPoints[i], XMVector3TransformCoord(XMLoadFloat4(&m_vWorldPoints[i]), WorldMatrixInverse));
	}

	Make_Plane(vPoints, m_vLocalPlanes);

}

_bool CFrustum::isIn_WorldSpace(_fvector vWorldPos, _float fRange)
{
	/*ax+ by + cz + d > 0*/

	for (size_t i = 0; i < 6; i++)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vWorldPlanes[i]), vWorldPos)))
			return false;
	}

	return true;
}

_bool CFrustum::isIn_LocalSpace(_fvector vLocalPos, _float fRange)
{
	/*ax+ by + cz + d > 0*/

	for (size_t i = 0; i < 6; i++)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vLocalPlanes[i]), vLocalPos)))
			return false;
	}

	return true;
}

void CFrustum::Make_Plane(const _float4* pPoints, _float4* pPlanes)
{
	XMStoreFloat4(&pPlanes[0], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[1]),
		XMLoadFloat4(&pPoints[5]),
		XMLoadFloat4(&pPoints[6])));

	XMStoreFloat4(&pPlanes[1], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[4]),
		XMLoadFloat4(&pPoints[0]),
		XMLoadFloat4(&pPoints[3])));

	XMStoreFloat4(&pPlanes[2], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[4]),
		XMLoadFloat4(&pPoints[5]),
		XMLoadFloat4(&pPoints[1])));

	XMStoreFloat4(&pPlanes[3], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[3]),
		XMLoadFloat4(&pPoints[2]),
		XMLoadFloat4(&pPoints[6])));

	XMStoreFloat4(&pPlanes[4], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[5]),
		XMLoadFloat4(&pPoints[4]),
		XMLoadFloat4(&pPoints[7])));

	XMStoreFloat4(&pPlanes[5], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[0]),
		XMLoadFloat4(&pPoints[1]),
		XMLoadFloat4(&pPoints[2])));
}

CFrustum* CFrustum::Create()
{
	CFrustum* pInstance = new CFrustum();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CFrustum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFrustum::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
