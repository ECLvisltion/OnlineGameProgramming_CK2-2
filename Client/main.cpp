#include <iostream>
#include <cstdlib>
#include <WinSock2.h>
using namespace std;

#define bufferSize 4096

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	// ���� ��ü ���� �� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return -1; }

	// Ŭ���̾�Ʈ ���� �ʱ�ȭ
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	// Ŭ���̾�Ʈ ������ ������ ����
	SOCKADDR_IN servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9000);
	servaddr.sin_addr.s_addr = inet_addr("127, 0, 0, 1");

	// ���� ���� ����
	char myFile[256] = "asdf.asdf";

	// ������ ���� �õ�
	connect(sock, (SOCKADDR*)& servaddr, sizeof(servaddr));
	cout << "������ ���� ���� (IP : " << inet_ntoa(servaddr.sin_addr) << ", Port : " << ntohs(servaddr.sin_port) << ')' << endl;

	// ���� ����
	FILE* fp = fopen(myFile, "rb");

	// ���� �̸� ����
	char filename[256];
	ZeroMemory(filename, 256);
	//http://blog.naver.com/war2i7i7/220791180984
}