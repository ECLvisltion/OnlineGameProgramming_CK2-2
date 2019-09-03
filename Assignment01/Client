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
	sprintf(filename, myFile);

	// 소켓으로 파일 이름을 전송
	send(sock, filename, 256, 0);

	// 파일 크기 얻기
	fseek(fp, 0, SEEK_END);
	int totalbytes = ftell(fp);

	// 파일 크기 소켓으로 보내기
	send(sock, (char*)& totalbytes, sizeof(totalbytes), 0);

	// 파일 데이터 전송에 사용할 변수
	char buf[bufferSize];
	int numread;
	int numtotal = 0;

	// 파일 포인터를 제일 앞으로 이동
	rewind(fp);

	// 반복적으로 파일 데이터 보내기
	while (true)
	{
		// 파일의 내용을 버퍼에 담음
		numread = fread(buf, 1, bufferSize, fp);

		if (numread > 0) // 파일 데이터가 조금이라도 남은 경우
		{
			send(sock, buf, numread, 0);
			numtotal += numread;
		}
		else if (numread == 0 && numtotal == totalbytes) // 파일을 모두 전송한 경우
		{
			cout << "총 " << numtotal << "바이트 파일 전송을 완료했습니다." << endl;
			break;
		}
	}
	fclose(fp);

	// 소켓 폐기
	closesocket(sock);
	cout << "서버에 접속 종료 (IP : " << inet_ntoa(servaddr.sin_addr) << ", Port : " << ntohs(servaddr.sin_port) << ')' << endl;

	//원속 종료
	WSACleanup();
	return 0;
}
