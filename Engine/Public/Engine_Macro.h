﻿#pragma once

#define D3DCOLOR_ARGB(a,r,g,b) \
    ((D3DCOLOR)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)))

#define NS_BEGIN(NAMESPACE) namespace NAMESPACE { 
#define NS_END }

#define ENUM_CLASS(ENUM) static_cast<unsigned int>(ENUM)

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define MSG_BOX(message)								\
::MessageBox(nullptr, TEXT(message), L"error", MB_OK)

#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static ClassName* Get_Instance();					\
	static unsigned int Destroy_Instance();				\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
unsigned int ClassName::Destroy_Instance()				\
{														\
	unsigned int iRefCnt = {};							\
	if(nullptr != m_pInstance)							\
	{													\
		iRefCnt = m_pInstance->Release();				\
		if(0 == iRefCnt)								\
			m_pInstance = nullptr;						\
	}													\
	return iRefCnt;										\
}