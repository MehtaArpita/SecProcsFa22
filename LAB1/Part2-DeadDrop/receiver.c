
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	
	
	uint64_t *buffer = (uint64_t*)malloc(BUFF_SIZE_1*sizeof(uint64_t));
	int buffer_access_index [8][8];
  
  	if(NULL == buffer) {
		  perror("Unable to malloc");
	  	return EXIT_FAILURE;
   	}

  	uint64_t selected_sets[8] = {12,25,30,140,200,250,300,400};

  	uint64_t eviction_set[8][8];
  	uint64_t mask = 0x1FF;
  	uint64_t adrs;
  	uint64_t adrs_setindex;
	int decoded_message [8];
  	int line_count = 0;
	int threshold = 100;

	// create eviction set 
  	for (int i=0; i<8; i++) {
		  line_count = 0;
		for (int j=0; j<BUFF_SIZE_1; j= j+8){
	      		adrs =(uint64_t) (buffer + j);
      	      		adrs_setindex = (adrs>>6) & mask;
	      		if (selected_sets[i] == adrs_setindex) {
				eviction_set[i][line_count] = adrs;
  				line_count = line_count + 1;
				buffer_access_index[i][line_count] = j;
	      		}
		}
  	}	
	
	// prime the selected cache sets
	for (i = 0; i<2000; i++) {
		for (int set_index = 0,; set_index < 8; set_index++) {
			for (line = 0; line < 8; line ++) {
				int t = buffer_access_index[(8*set_index)+line_count];
				temp = buffer[t];
			}
		}
	}
	
	printf("Receiver now listening.\n");
	delay(10)
	uint64_t latency[8][8] = {0};
	bool listening = true;
	uint64_t output = 0;
	while (listening) {

		// Put your covert channel code here
		//Probe the relevant address
		for (int set_index = 0,; set_index < 8; set_index++) {
			for (line = 0; line < 8; line ++) {
				latency[set_index][line] = measure_one_block_access_time((uint64_t)eviction_set[set_index][line]);
				if (latency[set_index][line] > threshold) {
					sum = sum +1;
				}				
			}
		}

	}
	
	// Decode the message 
	for (int set_index = 0,; set_index < 8; set_index++) {
		if (sum > 3) {
			output = output + pow (2, set_index);
		} else  {
			output = output;
		}
	}
	printf ("%d", &output);
	listening = false; 
	}

	printf("Receiver finished.\n");
	return 0;
}


