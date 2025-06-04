#include "Bounding_Sphere.h"

CBounding_Sphere::CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBounding { pDevice, pContext }
{
}

HRESULT CBounding_Sphere::Initialize(const CBounding::BOUNDING_DESC* pDesc)
{
	const SPHERE_DESC* pSphereDesc = static_cast<const SPHERE_DESC*>(pDesc);

	m_pOriginalDesc = new BoundingSphere(pSphereDesc->vCenter, pSphereDesc->fRadius);
	m_pDesc = new BoundingSphere(*m_pOriginalDesc);

	return S_OK;
}

void CBounding_Sphere::Update(_fmatrix WorldMatrix)
{

	m_pOriginalDesc->Transform(*m_pDesc, WorldMatrix);
}

#ifdef _DEBUG

HRESULT CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

#endif


CBounding_Sphere* CBounding_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc)
{
	CBounding_Sphere* pInstance = new CBounding_Sphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX("Failed to Created : CBounding_Sphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBounding_Sphere::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalDesc);
	Safe_Delete(m_pDesc);
}
