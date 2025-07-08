#pragma once

/* 직교투영, 카메라, 객체만듬, 후처리 쉐이딩, 블러, 그림자, 소프트이펙트, 
픽셀피킹, 모델 로딩, 애니메이팅, 파티클, 인스턴싱, 스프레팅쉐이더 */

/* 특정 헤더파일에 소속되지 않는 전역정보들을 선언해두는 역활 */
/* 기타 다른 정의파일(.h)들을 인클루드하는 역활. */
#include "../Default/framework.h"
#include <process.h>
#include "Client_Enum.h"


namespace Client
{
	const unsigned int			g_iWinSizeX = 1280;
	const unsigned int			g_iWinSizeY = 720;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;


using namespace Client;

//화 : 그림자.
//수 : 블러 절두체컬링.지형컬링.쿼드트리     //11시 출근
//목 : 팀편성관련 설문조사.  
//금 : 발표날짜. 
//월 발표. (팀 편성결과, 분배결과. 파트 분배 어떻게해야하는지 ) 

// 선생님 사랑합니다♥
// ㅡ151기 일동ㅡ 








