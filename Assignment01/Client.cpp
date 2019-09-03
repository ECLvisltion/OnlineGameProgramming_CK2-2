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
	sprintf(filename, myFile);

	// �������� ���� �̸��� ����
	send(sock, filename, 256, 0);

	// ���� ũ�� ���
	fseek(fp, 0, SEEK_END);
	int totalbytes = ftell(fp);

	// ���� ũ�� �������� ������
	send(sock, (char*)& totalbytes, sizeof(totalbytes), 0);

	// ���� ������ ���ۿ� ����� ����
	char buf[bufferSize];
	int numread;
	int numtotal = 0;

	// ���� �����͸� ���� ������ �̵�
	rewind(fp);

	// �ݺ������� ���� ������ ������
	while (true)
	{
		// ������ ������ ���ۿ� ����
		numread = fread(buf, 1, bufferSize, fp);

		if (numread > 0) // ���� �����Ͱ� �����̶� ���� ���
		{
			send(sock, buf, numread, 0);
			numtotal += numread;
		}
		else if (numread == 0 && numtotal == totalbytes) // ������ ��� ������ ���
		{
			cout << "�� " << numtotal << "����Ʈ ���� ������ �Ϸ��߽��ϴ�." << endl;
			break;
		}
	}
	fclose(fp);

	// ���� ���
	closesocket(sock);
	cout << "������ ���� ���� (IP : " << inet_ntoa(servaddr.sin_addr) << ", Port : " << ntohs(servaddr.sin_port) << ')' << endl;

	//���� ����
	WSACleanup();
	return 0;
}