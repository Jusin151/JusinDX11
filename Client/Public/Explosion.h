#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)

class CExplosion final : public CPartObject
{
public:
	typedef struct tagExploDesc : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
	}EXPLOSION_DESC;

private:
	CExplosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CExplosion(const CExplosion& Prototype);
	virtual ~CExplosion() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*						m_pShaderCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance*		m_pVIBufferCom = { nullptr };
private:
	const _float4x4* m_pSocketMatrix = { nullptr };
	const _uint* m_pParentState = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CExplosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END