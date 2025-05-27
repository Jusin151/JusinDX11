#include "ContainerObject.h"

#include "GameInstance.h"
#include "PartObject.h"

CContainerObject::CContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject { pDevice, pContext }
{
}

CContainerObject::CContainerObject(const CContainerObject& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CContainerObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CContainerObject::Initialize(void* pArg)
{
    CONTAINEROBJECT_DESC* pDesc = static_cast<CONTAINEROBJECT_DESC*>(pArg);

    m_iNumPartObjects = pDesc->iNumPartObjects;

    m_PartObjects.resize(m_iNumPartObjects);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    
    return S_OK;
}

void CContainerObject::Priority_Update(_float fTimeDelta)
{
    for (auto& pPartObject : m_PartObjects)
    {
        if(nullptr != pPartObject)
            pPartObject->Priority_Update(fTimeDelta);
    }
}

void CContainerObject::Update(_float fTimeDelta)
{
    for (auto& pPartObject : m_PartObjects)
    {
        if (nullptr != pPartObject)
            pPartObject->Update(fTimeDelta);
    }
}

void CContainerObject::Late_Update(_float fTimeDelta)
{
    for (auto& pPartObject : m_PartObjects)
    {
        if (nullptr != pPartObject)
            pPartObject->Late_Update(fTimeDelta);
    }
}

HRESULT CContainerObject::Render()
{
    return S_OK;
}


HRESULT CContainerObject::Add_PartObject(_uint iPartID, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
    CPartObject*    pPartObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
    if (nullptr == pPartObject)
        return E_FAIL;

    m_PartObjects[iPartID] = pPartObject;

    return S_OK;
}

void CContainerObject::Free()
{
    __super::Free();

    for (auto& pPartObject : m_PartObjects)
        Safe_Release(pPartObject);

    m_PartObjects.clear();


}
