#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

int setupServer()
{
   int socket_desc;
   struct sockaddr_in server_addr;

   // Create UDP socket:
   socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

   if(socket_desc < 0)
   {
      printf("Error while creating socket\n");
      return -1;
   }
   printf("Socket created successfully\n");

   // Set port and IP:
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(5555);
   server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

   // Bind to the set port and IP:
   if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
   {
      printf("Couldn't bind to the port\n");
      return -1;
   }
   printf("Done with binding\n");
   return socket_desc;
}

void setMotorCommand(int32_t l_wheel_dist, int32_t r_wheel_dist, int32_t speed)
{
   printf("SetMotor LWheelDist %d RWheelDist %d Speed %d\n", l_wheel_dist, r_wheel_dist, speed);
}

int main()
{
   char msg[1024];
   struct sockaddr_in client_addr;
   socklen_t client_struct_length = sizeof(client_addr);
   
   memset(msg, '\0', sizeof(msg));

   const int socket_desc = setupServer();
   
   while(true)
   {
      const ssize_t msg_size = recvfrom(socket_desc, msg, sizeof(msg), 0, (struct sockaddr*)&client_addr, &client_struct_length);
      if(msg_size < 0)
      {
         printf("Couldn't receive\n");
         return -1;
      }

      // Insert null char
      msg[msg_size] = '\0';

      // printf("%s\n", msg);

      // Decode command
      ssize_t i = 0;
      switch(msg[i++])
      {
         case 'm':
         {
            int32_t l, r, s;
   
            memcpy(&l, &msg[i], sizeof(int32_t));
            i += sizeof(int32_t);

            memcpy(&r, &msg[i], sizeof(int32_t));
            i += sizeof(int32_t);

            memcpy(&s, &msg[i], sizeof(int32_t));
            i += sizeof(int32_t);

            setMotorCommand(l, r, s);
            break;
         }

         default:
            break;
      }
   }
   return 0;
}
