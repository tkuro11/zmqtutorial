#include <stdio.h>
#include <string.h>
#include <zmq.h>
#include <unistd.h>

void wait_for_hello(void* respond)
{
	zmq_msg_t request;

	zmq_msg_init(&request); 
	zmq_msg_recv(&request, respond, 0);
	zmq_msg_close(&request);
}

void send_world(void* respond)
{
	zmq_msg_t reply;

	zmq_msg_init_size(&reply, strlen("world")); 
	memcpy(zmq_msg_data(&reply), "world", 5);
	zmq_msg_send(&reply, respond, 0);
	zmq_msg_close(&reply);
}

int main(int arc, char const *argv[])
{
	void* context = zmq_ctx_new();
	void* respond = zmq_socket(context, ZMQ_REP);
	zmq_bind(respond, "tcp://*:4040");

	printf("Starting...\n");

	for (;;) {
		wait_for_hello(respond);
		printf("Received: hello\n");
		sleep(1);
		send_world(respond);
		printf("send: world\n");
	}

	zmq_close(respond);
	zmq_ctx_destroy(context);

	return 0;
}
