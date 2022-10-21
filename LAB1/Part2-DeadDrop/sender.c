
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>

// TODO: define your own buffer size
#define BUFF_SIZE (1<<21)
#define BUFF_SIZE_1 4096


void send_bit (bool one, uint64_t* address) {
	volatile char temp;
	uint64_t *target = (uint64_t*)malloc(8*sizeof(uint64_t));
	target = address;		
	if (one) {
		temp = target[0];
	}
	else {

	}
	free(target);
}



int main(int argc, char **argv)
{
  // Allocate a buffer using huge page
  // See the handout for details about hugepage management
  void *buf= mmap(NULL, BUFF_SIZE, PROT_READ | PROT_WRITE, MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB, -1, 0);
  
  if (buf == (void*) - 1) {
     perror("mmap() error\n");
     exit(EXIT_FAILURE);
  }

  uint64_t *buffer = (uint64_t*)malloc(BUFF_SIZE_1*sizeof(uint64_t));
  
  if(NULL == buffer) {
	  perror("Unable to malloc");
	  return EXIT_FAILURE;
   }

  uint64_t selected_sets[8] = {12,25,30,140,200,250,300,400};

  uint64_t eviction_set[8][8];
  uint64_t mask = 0x1FF;
  uint64_t adrs;
  uint64_t adrs_setindex;
  int line_count = 0;

  for (int i=0; i<8; i++) {
	  line_count = 0;
	for (int j=0; j<BUFF_SIZE_1; j= j+8){
	      adrs =(uint64_t) (buffer + j);
      	      adrs_setindex = (adrs>>6) & mask;
	      if (selected_sets[i] == adrs_setindex) {
		eviction_set[i][line_count] = adrs;
  		line_count = line_count + 1;
	      }
	}
  }	
  // The first access to a page triggers overhead associated with
  // page allocation, TLB insertion, etc.
  // Thus, we use a dummy write here to trigger page allocation
  // so later access will not suffer from such overhead.
  //*((char *)buf) = 1; // dummy write to trigger page allocation


  // TODO:
  // Put your covert channel setup code here

  printf("Please type a message.\n");

  bool sending = true;
  while (sending) {
      char text_buf[128];
      fgets(text_buf, sizeof(text_buf), stdin);

      // TODO:
      // Put your covert channel code here
      if (strcmp(text_buf,"exit\n") == 0) {
	sending = false;
	}

      char *msg = string_to_binary(text_buf);
      size_t msg_len = strlen(msg);
      for (int index = 0; index< 8; index++) {
	for (int line = 0; line<8; line++) {
		if (msg[index] == '0') {
           		send_bit(false,eviction_set[index%8][line]);
   		}else {
	   		send_bit(true,eviction_set[index%8][line]);
   		}
	}
  	
  }
  free(buffer);
  printf("Sender finished.\n");
  return 0;
}


