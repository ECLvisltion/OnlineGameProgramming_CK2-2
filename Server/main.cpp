/*
도움을 받은 사이트
C++ 파일 전송 네트워크 프로그램 - 기적의 프로그래머 : http://blog.naver.com/war2i7i7/220791180984 (가장 영향을 많이 받음)

Visual C++ 설명서 : https://docs.microsoft.com/ko-kr/cpp/?view=vs-2019
#pragma warning(disable:4996) - 초록매실원액 : https://puffinknight.tistory.com/69 (사용하지 않음)
[API] winsock2.h 이용시 라이브러리 추가 - 아린R : https://anisyw.tistory.com/13
디스크립터 (descriptor)?? by 해커남 - 해커남 : https://blog.naver.com/ifkiller/70081227708 : (*1)
*/

#include <iostream>
#include <WinSock2.h>
#include <cstdlib>
using namespace std;

#define bufferSize 4096 // 버퍼 크기 정의

// 컴파일을 할 때 Ws2_32.lib 라이브러리 파일과 링크가 되어 있어야 한다.
// Visual Studio에서 프로젝트마다 일일이 링크걸어주는게 귀찮으니 아래처럼 써준다.
#pragma comment(lib, "Ws2_32.lib")

int DataReceive(SOCKET s, char* buf, int len, int flags);

int main()
{
	int retval; // 데이터 크기를 담을 변수

	// 원속 객체 선언 및 초기화
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0); // 연결용 소켓을 생성

	// 바인딩 처리 변수 초기화
	SOCKADDR_IN servaddr;
	ZeroMemory(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listen_sock, (SOCKADDR*)& servaddr, sizeof(servaddr)); // 바인딩

	listen(listen_sock, SOMAXCONN); // 연결 함수 시작

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[bufferSize];

	while (true) // 반복적으로 클라이언트로부터 소켓 요청을 받음
	{
		// 클라이언트 접속을 받음
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)& clientaddr, &addrlen);

		// 접속한 클라이언트 정보 출력
		cout << "클라이언트 접속 (IP : " << inet_ntoa(clientaddr.sin_addr) << ", Port : " << ntohs(clientaddr.sin_port) << ')' << endl;

		// 파일 이름 받기
		char filename[256];
		ZeroMemory(filename, 256);
		DataReceive(client_sock, filename, 256, 0);

		// 받는 파일 이름 출력
		cout << "받을 파일 이름 : " << filename << endl;

		// 파일 크기 받기
		int totalbytes;
		DataReceive(client_sock, (char*)& totalbytes, sizeof(totalbytes), 0);

		// 받을 파일 크기 출력
		cout << "받을 파일 이름 : " << filename << endl;

		// 파일 열기
		FILE* fp = fopen(filename, "wb");

		int numtotal = 0;

		while (true) // 파일 데이터 받기
		{
			retval = DataReceive(client_sock, buf, bufferSize, 0);

			if (retval == 0) { break; } // 더 받을 데이터가 없을 때
			else
			{
				fwrite(buf, 1, retval, fp);
				numtotal += retval; // 받은 데이터 크기만큼 변수에 더해줌
			}
		}
		fclose(fp);

		// 전송 결과
		if (numtotal == totalbytes) { cout << "파일을 성공적으로 받았습니다." << endl; }
		else { cout << "파일을 제대로 받지 못했습니다." << endl; }

		// 해당 클라이언트 소켓을 폐가
		closesocket(client_sock);
		cout << "클라이언트 종료 (IP : " << inet_ntoa(clientaddr.sin_addr) << ", Port : " << ntohs(clientaddr.sin_port) << ')' << endl;
	}
	// 서버 소켓을 폐기
	closesocket(listen_sock);

	// 원속 종료
	WSACleanup();
	return 0;
}

int DataReceive(SOCKET s, char* buf, int len, int flags) // 데이터 수신 함수
{
	// 통신에 필요한 변수들
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) // 남아있는 데이터가 있는 경우 반복 시행.
	{
		received = recv(s, ptr, left, flags);
		/*
		recv(SOCKET s, char *buf, int len, int flags)
			SOCKET s : 연결된 소켓을 식별하기 위한 디스크립터(*1)
			char *buf : 들어오는 데이터를 받는 버퍼에 대한 포인터
			int len : 위의 char *buf의 크기(byte)
			int flags : 이 함수에 영향을 주는 변수. 자세한 것은 아래 링크로.
			https://docs.microsoft.com/ko-kr/windows/win32/api/winsock/nf-winsock-recv#remarks
		*/

		if (received == 0) { break; } // 더 이상 데이터를 받아오지 못하는 경우

		left -= received; ptr += received; // 받아 온 데이터가 존재하는 경우
	}
	// 더 받을 수 있는 데이터 길이를 반환(데이터를 한 번이라도 받았으면 0이 아닌 수를 반환하게 됨.)
	return (len - left);
}
