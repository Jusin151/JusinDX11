#pragma once

#include "VIBuffer.h"

/*
인스턴싱이 필요한 모든 모델들의 부모가 된다.
*/

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instance abstract : public CVIBuffer
{
public:
	typedef struct tagInstanceDesc
	{
		_uint		iNumInstance;
		_float3		vRange;
		_float2		vSize;
		_float3		vCenter;

	}INSTANCE_DESC;
protected:
	CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instance(const CVIBuffer_Instance& Prototype);
	virtual ~CVIBuffer_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual HRESULT Bind_Buffers() override;
	virtual HRESULT Render() override;

	virtual void Drop(_float fTimeDelta);
	virtual void Spread(_float fTimeDelta);


protected:
	ID3D11Buffer*			m_pVBInstance = { nullptr };
	D3D11_BUFFER_DESC		m_VBInstanceDesc = {};
	D3D11_SUBRESOURCE_DATA	m_VBInstanceSubresourceData = {};
	

	_uint					m_iNumIndexPerInstance = {};
	_uint					m_iNumInstance = {};
	_uint					m_iVertexInstanceStride = {};

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END;