#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CPartObject abstract : public CGameObject
{
public:
	typedef struct tagPartObjectDesc : CGameObject::GAMEOBJECT_DESC
	{
		const _float4x4* pParentMatrix;
	}PARTOBJECT_DESC;

protected:
	CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartObject(const CPartObject& Prototype);
	virtual ~CPartObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	const _float4x4*		m_pParentMatrix = { nullptr };
	_float4x4				m_CombinedWorldMatrix{};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

NS_END