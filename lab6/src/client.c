#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "head.h"


struct Server
{
  char ip[255];
  int port;
};


bool ConvertStringToUI64(const char *str, uint64_t *val)
{
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);

  if (errno == ERANGE)
  {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
  {
    return false;
  }

  *val = i;
  return true;
}


int main(int argc, char **argv)
{
  uint64_t k = -1;
  uint64_t mod = -1;
  char servers[255] = {'\0'};

  while (true)
  {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        if(!ConvertStringToUI64(optarg, &k))
          {printf("enter k_num correctly");
          exit(1);
          }
        break;
      case 1:
        if(!ConvertStringToUI64(optarg, &mod))
          {printf("enter mod correctly");
          exit(1);
          }
        break;
      case 2:
        memcpy(servers, optarg, strlen(optarg));

        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  unsigned int server_count = 0;
  FILE* server_reader = fopen(servers,"r"); 

  while(!feof(server_reader))
  {    
    if(fgetc(server_reader)=='\n')
    {
      ++server_count;
    }
  }

  int *ports= malloc(sizeof(int)*server_count); 
  close(server_reader);
  server_reader =fopen(servers,"r");
  struct Server *to = malloc(sizeof(struct Server) * server_count);

  for (int i = 0; i < server_count; i++)  
  {   
      fscanf(server_reader,"%d",&ports[i]);
      memcpy(&to[i].port,&ports[i],sizeof(int));
      memcpy(to[i].ip, "127.0.0.1", sizeof("127.0.0.1"));      
  }

  free(ports);
  int step=k/server_count;
  uint64_t res=1;

  for (int i = 0; i < server_count; i++)
  {
    struct hostent *hostname = gethostbyname(to[i].ip);
    
    if (hostname == NULL)
    {
      fprintf(stderr, "gethostbyname failed with %s\n", to[i].ip);
      exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(to[i].port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr_list[0]);
    int sck = socket(AF_INET, SOCK_STREAM, 0);
  
    if (sck < 0)
    {
      fprintf(stderr, "Socket creation failed!\n");
      exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
      fprintf(stderr, "Connection failed\n");
      exit(1);
    }

      uint64_t begin=1;
      uint64_t end=1;     

    if (i==server_count-1)
    { begin = (i*step)+1;
      end =k;
    }
    else
    {
      begin =(i*step)+1;
      end =(i+1)*step;      
    }  

    char task[sizeof(uint64_t) * 3];
    memcpy(task, &begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    if (send(sck, task, sizeof(task), 0) < 0)
    {
      fprintf(stderr, "Send failed\n");
      exit(1);
    }

    char response[sizeof(uint64_t)];
    
    if (recv(sck, response, sizeof(response), 0) < 0)
    {
      fprintf(stderr, "Recieve failed\n");
      exit(1);
    }   

    uint64_t answer = 0;
    memcpy(&answer, response, sizeof(uint64_t));
    printf("answer from server : %llu\n", answer);
    res=answer*res;
    res=res % mod;    
    if(i==server_count-1)
    printf("result: %llu\n", res);
    close(sck);
  }
  
  free(to);
  return 0;
}
