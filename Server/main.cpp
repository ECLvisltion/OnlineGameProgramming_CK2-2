// ��� ���� �߰�
#include <iostream>
#include <cstdlib>
#include <WinSock2.h>
using namespace std;

#define bufferSize 4096 // ���� ũ�� ����

#pragma comment(lib, "Ws2_32.lib") // ���� ����� ���� ���̺귯�� ��ŷ

int dataReceive(SOCKET s, char* buf, int len, int flags); // ������ ���� �Լ�

int main()
{
	int retval; // ������ ũ�⸦ ���� ����

	// ���� ��ü ���� �� �ʱ�ȭ
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0); // ����� ������ ����

	// ���ε� ó�� ���� �ʱ�ȭ
	SOCKADDR_IN servaddr;
	ZeroMemory(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(9000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(listen_sock, (SOCKADDR*)& servaddr, sizeof(servaddr)); // ���ε�

	listen(listen_sock, SOMAXCONN); // ���� �Լ� ����

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[bufferSize];

	while (true) // �ݺ������� Ŭ���̾�Ʈ�κ��� ���� ��û�� ����
	{
		// Ŭ���̾�Ʈ ������ ����
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)& clientaddr, &addrlen);

		// ������ Ŭ���̾�Ʈ ���� ���
		cout << "Ŭ���̾�Ʈ ���� (IP : " << inet_ntoa(clientaddr.sin_addr) << ", Port : " << ntohs(clientaddr.sin_port) << ')' << endl;

		// ���� �̸� �ޱ�
		char filename[256];
		ZeroMemory(filename, 256);
		dataReceive(client_sock, filename, 256, 0);

		// �޴� ���� �̸� ���
		cout << "���� ���� �̸� : " << filename << endl;

		// ���� ũ�� �ޱ�
		int totalbytes;
		dataReceive(client_sock, (char*)& totalbytes, sizeof(totalbytes), 0);

		// ���� ���� ũ�� ���
		cout << "���� ���� �̸� : " << filename << endl;

		// ���� ����
		FILE* fp = fopen(filename, "wb");

		int numtotal = 0;

		while (true) // ���� ������ �ޱ�
		{
			retval = dataReceive(client_sock, buf, bufferSize, 0);

			if (retval == 0) { break; } // �� ���� �����Ͱ� ���� ��
			else
			{
				fwrite(buf, 1, retval, fp);
				numtotal += retval; // ���� ������ ũ�⸸ŭ ������ ������
			}
		}
		fclose(fp);

		// ���� ���
		if (numtotal == totalbytes) { cout << "������ ���������� �޾ҽ��ϴ�." << endl; }
		else { cout << "������ ����� ���� ���߽��ϴ�." << endl; }

		// �ش� Ŭ���̾�Ʈ ������ ��
		closesocket(client_sock);
		cout << "Ŭ���̾�Ʈ ���� (IP : " << inet_ntoa(clientaddr.sin_addr) << ", Port : " << ntohs(clientaddr.sin_port) << ')' << endl;
	}
	// ���� ������ ���
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}

int dataReceive(SOCKET s, char* buf, int len, int flags) // ������ ���� �Լ�
{
	// ��ſ� �ʿ��� ���� ����
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) //�����ִ� �����Ͱ� �ִ� ��� �ݺ������� ����
	{
		received = recv(s, ptr, left, flags);

		if (received == 0) { break; } // ���̻� �����͸� �޾ƿ��� ���ϴ� ���
		left -= received; ptr += received; // �޾ƿ� �����Ͱ� �����ϴ� ���
	}

	return (len - left); // �� ���� �� �ִ� ������ ���̸� ��ȯ(�����͸� �� ���̶� �޾����� 0�� �ƴ� �� ��ȯ)
}