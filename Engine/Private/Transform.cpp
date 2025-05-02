#include "Transform.h"

#include "Shader.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent { pDevice, pContext }
{
}

CTransform::CTransform(const CTransform& Prototype)
	: CComponent{ Prototype }
	, m_WorldMatrix { Prototype.m_WorldMatrix }
{
}

_float3 CTransform::Get_Scaled()
{	
	return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE::RIGHT))),
		XMVectorGetX(XMVector3Length(Get_State(STATE::UP))),
		XMVectorGetX(XMVector3Length(Get_State(STATE::LOOK))));		
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;

	return S_OK;
}

void CTransform::Scaling(_float fX, _float fY, _float fZ)
{
	Set_State(STATE::RIGHT, XMVector3Normalize(Get_State(STATE::RIGHT)) * fX);
	Set_State(STATE::UP, XMVector3Normalize(Get_State(STATE::UP)) * fY);
	Set_State(STATE::LOOK, XMVector3Normalize(Get_State(STATE::LOOK)) * fZ);
}

void CTransform::Scaling(const _float3& vScale)
{
	Set_State(STATE::RIGHT, XMVector3Normalize(Get_State(STATE::RIGHT)) * vScale.x);
	Set_State(STATE::UP, XMVector3Normalize(Get_State(STATE::UP)) * vScale.y);
	Set_State(STATE::LOOK, XMVector3Normalize(Get_State(STATE::LOOK)) * vScale.z);
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);
		
	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vRight = Get_State(STATE::RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vRight = Get_State(STATE::RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Target(_fvector vTarget, _float fTimeDelta, _float fMinDistance)
{
	_vector		vMoveDir = vTarget - Get_State(STATE::POSITION);

	if(fMinDistance <= XMVectorGetX(XMVector3Length(vMoveDir)))
		Set_State(STATE::POSITION, Get_State(STATE::POSITION) + XMVector3Normalize(vMoveDir) * m_fSpeedPerSec * fTimeDelta);
}

void CTransform::LookAt(_fvector vAt)
{
	_float3		vScaled = Get_Scaled();

	_vector		vLook = vAt - Get_State(STATE::POSITION);
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE::RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE::UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE::LOOK, XMVector3Normalize(vLook) * vScaled.z);
}

HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);	
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CTransform::Free()
{
	__super::Free();

}
