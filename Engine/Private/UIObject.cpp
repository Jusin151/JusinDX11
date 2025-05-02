#include "UIObject.h"

CUIObject::CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext }
{
}

CUIObject::CUIObject(const CUIObject& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CUIObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIObject::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fX = pDesc->fX;
	m_fY = pDesc->fY;
	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	D3D11_VIEWPORT			ViewportDesc{};
	_uint					iNumViewports = { 1 };

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.0f, 1.f));

	m_pTransformCom->Scaling(m_fSizeX, m_fSizeY);

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - ViewportDesc.Width * 0.5f, -m_fY + ViewportDesc.Height * 0.5f, 0.f, 1.f));


	return S_OK;
}

void CUIObject::Priority_Update(_float fTimeDelta)
{
}

void CUIObject::Update(_float fTimeDelta)
{
}

void CUIObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIObject::Render()
{

	return S_OK;
}

void CUIObject::Free()
{
	__super::Free();


}
