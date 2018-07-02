#include <stdio.h>
#include <string.h>
#include <zmq.h>
#include <unistd.h>

void send_hello(void* request)
{
	zmq_msg_t req;

	zmq_msg_init_size(&req, strlen("hello")); 
	memcpy(zmq_msg_data(&req), "hello", 5);
	zmq_msg_send(&req, request, 0);
	zmq_msg_close(&req);
}

void wait_for_world(void* request)
{
	zmq_msg_t reply;

	zmq_msg_init(&reply);
	zmq_msg_recv(&reply, request, 0);
	zmq_msg_close(&reply);
}

int main(int arc, char const *argv[])
{
  void* context = zmq_ctx_new();

  printf("Client starting...\n");

  void* request = zmq_socket(context, ZMQ_REQ);
  zmq_connect(request, "tcp://localhost:4040");

  int count = 0;

  for (;;) {
	  send_hello(request);
	  printf("Sending: world - %d\n", count);
	  wait_for_world(request);
	  printf("Received: hello - %d\n", count);
	  count++;
  }

  zmq_close(request);
  zmq_ctx_destroy(context);

  return 0;
}
