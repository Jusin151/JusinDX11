#pragma once

#include "VIBuffer_Instance.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer_Instance
{
public:
	typedef struct tagRectInstance : public CVIBuffer_Instance::INSTANCE_DESC
	{
		_float2		vLifeTime;
		_bool		isLoop;

	}RECT_INSTANCE_DESC;
	
private:
	CVIBuffer_Rect_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& Prototype);
	virtual ~CVIBuffer_Rect_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCE_DESC* pDesc);
	virtual HRESULT Initialize(void* pArg);

	virtual HRESULT Bind_Buffers() override;

	virtual void Drop(_float fTimeDelta);


protected:
	ID3D11Buffer*			m_pVBParticle = { nullptr };
	D3D11_BUFFER_DESC		m_VBParticleDesc = {};
	D3D11_SUBRESOURCE_DATA	m_VBParticleSubresourceData = {};
	VTXPARTICLE*			m_pVertexParticles = { nullptr };

	_uint					m_iVertexParticleStride = {};

public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END