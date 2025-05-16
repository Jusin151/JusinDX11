#pragma once

#include "Base.h"

/* 뼈. (aiNode, aiBone, aiNodeAnim) */
/* aiNode를 이용해서 셋팅한 뼈 정보. */

NS_BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const _float4x4* Get_CombinedTransformationMatrix() const {
		return &m_CombinedTransformationMatrix;
	}

public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentBoneIndex);
	void Update_TransformationMatrix(_fmatrix TransformationMatrix);
	void Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);
	_bool Compare_Name(const _char* pName) {
		return !strcmp(m_szName, pName);
	}

private:
	_char					m_szName[MAX_PATH] = {};

	/* (A : 이 뼈 자체의 원점기준 변환정보를 표현한 행렬) */
	_float4x4				m_TransformationMatrix = {};

	/* (A : 이 뼈 자체의 원점기준 변환정보를 표현한 행렬) * 부모행렬. = A: 부모를 기준으로 회전한다. */
	_float4x4				m_CombinedTransformationMatrix = {};	

	_int					m_iParentBoneIndex = { -1 };

public:
	static CBone* Create(const aiNode* pAINode, _int iParentBoneIndex);
	virtual void Free() override;

};

NS_END