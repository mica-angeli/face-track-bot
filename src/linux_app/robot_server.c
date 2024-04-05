#include <arpa/inet.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <termios.h>
#include <unistd.h>

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
   // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   server_addr.sin_addr.s_addr = inet_addr("192.168.1.107");

   // Bind to the set port and IP:
   if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
   {
      printf("Couldn't bind to the port\n");
      return -1;
   }
   printf("Done with binding\n");
   return socket_desc;
}

int setupSerial()
{
   int fd;

   fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);

   if(fd < 0)
   {
      printf("Error while opening serial port\n");
      return -1;
   }
   printf("Serial connection created successfully\n");

   return fd;
}

void testModeEnable(int serial_desc)
{
   char cmd[1024];
   const size_t s = snprintf(cmd, sizeof(cmd), "TestMode On\n");
   printf("%s", cmd);
   write(serial_desc, cmd, s);
}

void playSound(int serial_desc, uint8_t sound_id)
{
   char cmd[1024];
   const size_t s = snprintf(cmd, sizeof(cmd), "PlaySound SoundID %d\n", sound_id);
   printf("%s", cmd);
   write(serial_desc, cmd, s);
}

void setMotorCommand(int serial_desc, int32_t l_wheel_dist, int32_t r_wheel_dist, int32_t speed)
{
   char cmd[1024];
   const size_t s = snprintf(cmd, sizeof(cmd), "SetMotor LWheelDist %d RWheelDist %d Speed %d\n", l_wheel_dist, r_wheel_dist, speed);
   printf("%s", cmd);
   write(serial_desc, cmd, s);
}

int main()
{
   char msg[1024];
   struct sockaddr_in client_addr;
   socklen_t client_struct_length = sizeof(client_addr);
   
   memset(msg, '\0', sizeof(msg));

   const int socket_desc = setupServer();
   if(socket_desc < 0)
   {
      return 1;
   }

   const int serial_desc = setupSerial();
   if(serial_desc < 0)
   {
      return 1;
   }

   for(int i = 0; i < 3; ++i)
   {
      testModeEnable(serial_desc);
      sleep(2);
   }

   playSound(serial_desc, 0);
   sleep(2);

   // Flush buffer
   recvfrom(socket_desc, msg, sizeof(msg), 0, (struct sockaddr*)&client_addr, &client_struct_length);

   // Invalidate buffer
   msg[0] = '\0';

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

            setMotorCommand(serial_desc, l, r, s);
            break;
         }

         default:
            break;
      }

      // Invalidate buffer
      msg[0] = '\0';
   }
   return 0;
}
