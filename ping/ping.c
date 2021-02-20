#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h>
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

struct __attribute__((packed)) ICMPMessage{
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	uint16_t identifier;
	uint16_t sequence;
};

int make_socket(struct ICMPMessage icmp);
void send_socket(int s, struct ICMPMessage icmp, struct sockaddr_in addr, in_addr_t ping_target_ip_addr);
void receve_socket(int s, struct sockaddr_in addr);
uint16_t CalcChecksum(void* buf, size_t start, size_t end);
void error_message(int argc, char** argv);

int main(int argc, char** argv){
	struct ICMPMessage icmp;
	struct sockaddr_in addr;
	in_addr_t ping_target_ip_addr = inet_addr(argv[argc-1]);
	error_message(argc, argv);
	int s = make_socket(icmp);
	send_socket(s, icmp, addr, ping_target_ip_addr);
	receve_socket(s, addr);
}

int make_socket(struct ICMPMessage icmp){
	memset(&icmp,0,sizeof(icmp));
	icmp.type = 8;
	icmp.checksum = CalcChecksum(&icmp, 0, sizeof(icmp));
	int s =  socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	if(s < 0){
		printf("socket() failed\n");
		exit(1);
	}
	return s;
}

void send_socket(int s, struct ICMPMessage icmp, struct sockaddr_in addr, in_addr_t ping_target_ip_addr){
	int n = sendto(s, &icmp, sizeof(icmp), 0, (struct sockaddr*)&addr, sizeof(addr));
	if(n < 1){
		printf("sendto() failed\n");
		exit(1);
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ping_target_ip_addr;
}

void receve_socket(int s, struct sockaddr_in addr){
	uint8_t recv_buf[256];
	socklen_t addr_size;
	int recv_len = recvfrom(s, &recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&addr, &addr_size);
	if(recv_len){
		printf("recvform() failed\n");
		exit(1);
	}
}

uint16_t CalcChecksum(void* buf, size_t start, size_t end) {
  // https://tools.ietf.org/html/rfc1071
  uint8_t* p = buf;
  uint32_t sum = 0;
  for (size_t i = start; i < end; i += 2) {
    sum += ((uint16_t)p[i + 0]) << 8 | p[i + 1];
  }
  while (sum >> 16) {
    sum = (sum & 0xffff) + (sum >> 16);
  }
  sum = ~sum;
  return ((sum >> 8) & 0xFF) | ((sum & 0xFF) << 8);
}

void error_message(int argc, char** argv){
	if(argc != 2){
		printf("Usage :");
		printf(argv[0]);
		printf(" <ip addr>\n");
		exit(EXIT_FAILURE);
	}
}
