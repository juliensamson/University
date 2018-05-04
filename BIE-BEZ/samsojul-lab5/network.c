/*
    AUTHOR: JULIEN SAMSON

    Connection to the server
    https://www.binarytides.com/server-client-example-c-sockets-linux/

    Display a .pem file on console
    https://stackoverflow.com/questions/9758238/how-to-view-the-contents-of-a-pem-certificate
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <arpa/inet.h>

void Die(char *message) {
  printf("%s\n", message);
  exit(1);
}

int ConnectionToServer(const char host[], int port){
  printf("Establishing connection....\n");
  struct sockaddr_in server;

  //Create socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0)
    Die("Cannot create socket");
  printf("Socket created\n");

  //Prepare the socketaddr structure
  server.sin_addr.s_addr = inet_addr(host); //ip address
  server.sin_family = AF_INET;
  server.sin_port = htons(port); // port

  //Connect to remote server
  if(0 != connect(sock, (struct sockaddr *)&server, sizeof(server)))
    Die("Connection failed");
  printf("Connected\n");

  return sock;
}

int main(void) {
  const char* request = "GET /en/student/forms HTTP/1.1\r\nHost: fit.cvut.cz\r\n\r\n";
  const char* host = "147.32.232.248"; //IP address of "fit.cvut.cz"
  int port = 443;

  int socket = ConnectionToServer(host, port);

  //Initialize OpenSSL library
  SSL_library_init();
  //Create SSL context
  SSL_CTX * ctx = SSL_CTX_new(SSLv23_client_method());
  //Create SSL structure
  SSL * ssl = SSL_new(ctx);
  //Assign open connection
  SSL_set_fd(ssl, socket);
  //Start communication
  int res = SSL_connect(ssl);
  if(SSL_write(ssl, request, strlen(request)) <= 0)
    Die("SSL_write failed");
  printf("SSL_write initialize\n");

  int data = 0;
  const int buff_len = 512;
  char buff[buff_len];
  FILE * page = fopen("index.html", "wb");

  //Downlaod htmlfile from the server
  while((res = SSL_read(ssl, buff, buff_len)))
  {
    if(res <= 0)
      Die("SSL_read failed");

    fwrite(buff, sizeof(char), res, page);
    data += res;
  }
  fclose(page);

  //Certificate
  printf("Save certificate\n");
  FILE * fcertificate = fopen("certificate.pem", "w");
  X509 * cert = SSL_get_peer_certificate(ssl);
  PEM_write_X509(fcertificate, cert);
  fclose(fcertificate);
  printf("Enter: \"openssl x509 -in certificate.pem -text\" to display the certicate\n");

  //Clean up memory
  SSL_shutdown(ssl);
  SSL_free(ssl);
  SSL_CTX_free(ctx);
  //close(socket);

  exit(0);
 }
