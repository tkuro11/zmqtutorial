#include <stdio.h>
#include <string.h>
#include <zmq.h>
#include <unistd.h>

int main(int arc, char const *argv[])
{
  void* context = zmq_ctx_new();

  printf("Client starting...\n");

  void* request = zmq_socket(context, ZMQ_REQ);
  zmq_connect(request, "tcp://localhost:4040");

  int count = 0;

  for (;;) {
    zmq_msg_t req;
      zmq_msg_init_size(&req, strlen("hello")); 
      memcpy(zmq_msg_data(&req), "world", 5);
    printf("Sending: hello - %d\n", count);
	  zmq_msg_send(&req, request, 0);
      zmq_msg_close(&req);

    zmq_msg_t reply;
	  zmq_msg_init(&reply);
	  zmq_msg_recv(&reply, request, 0);
	printf("Received: hello - %d\n", count);
	  zmq_msg_close(&reply);
	count++;
  }
  zmq_close(request);
  zmq_ctx_destroy(context);

  return 0;
}
