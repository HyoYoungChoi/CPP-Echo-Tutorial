#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message)	// 오류나면 메세지 출력 후 종료.
{
	cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	cout << "[ TCP Echo Server Ver0.2 ]\n";
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	// Winsock을 초기화 합니다. 2.2버전
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);	// TCP 소켓을 생성합니다.

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);	// 4바이트 정수를 네트워크 바이트 형식으로 l은 long
	serverAddress.sin_port = htons(serverPort); // 2바이트 정수 네트워크 바이트 형식으로			s는 short

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)	// 동시연결 5개로 설정.
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen() \n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) {	// 클라이언트의 메시지를 받아서 그대로 다시 전달합니다.
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[클라이언트 메시지]: " << received << '\n';
		cout << "[메시지 전송]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {	// exit입력시 종료.
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[서버 종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}