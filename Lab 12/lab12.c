#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * Connects to a server over TCP and sends it a message.
 * Input: argv[1] is the server's IP address, argv[2] its port, argv[3] the message.
 * Output: returns 0 on success, 1 if a call fails.
 * Assumptions: arguments are valid and a server is listening at that IP and port.
 */
int main(int argc, char *argv[])
{
  struct sockaddr_in serverAddress;
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (clientSocket == -1) {
    printf("Error: socket failed\n");
    return 1;
  }

  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(atoi(argv[2]));
  serverAddress.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
    printf("Error: connect failed\n");
    close(clientSocket);
    return 1;
  }

  if (send(clientSocket, argv[3], strlen(argv[3]), 0) == -1) {
    printf("Error: send failed\n");
    close(clientSocket);
    return 1;
  }

  close(clientSocket);
  return 0;
}
