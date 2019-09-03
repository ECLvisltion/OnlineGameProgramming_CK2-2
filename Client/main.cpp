#include <iostream>
#include <cstdlib>
#include <WinSock2.h>
using namespace std;

#define bufferSize 4096

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	// 원속 객체 선언 및 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return -1; }

	// 클라이언트 소켓 초기화
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	// 클라이언트 소켓을 서버에 연결
	SOCKADDR_IN servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9000);
	servaddr.sin_addr.s_addr = inet_addr("127, 0, 0, 1");

	// 보낼 파일 설정
	char myFile[256] = "asdf.asdf";

	// 서버에 연결 시도
	connect(sock, (SOCKADDR*)& servaddr, sizeof(servaddr));
	cout << "서버에 접속 성공 (IP : " << inet_ntoa(servaddr.sin_addr) << ", Port : " << ntohs(servaddr.sin_port) << ')' << endl;

	// 파일 열기
	FILE* fp = fopen(myFile, "rb");

	// 파일 이름 변수
	char filename[256];
	ZeroMemory(filename, 256);
	//http://blog.naver.com/war2i7i7/220791180984
}