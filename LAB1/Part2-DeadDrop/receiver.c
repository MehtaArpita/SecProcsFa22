
#include"util.h"
// mman library to be used for hugepage allocations (e.g. mmap or posix_memalign only)
#include <sys/mman.h>
# define BUFF_SIZE 32768


int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	printf("Please press enter.\n");

	char text_buf[2];
	fgets(text_buf, sizeof(text_buf), stdin);

	
	
	uint64_t *buffer = (uint64_t*)malloc(BUFF_SIZE*sizeof(uint64_t));

	int buffer_access_index[8][8]= {0};


  	if(NULL == buffer) {
		  perror("Unable to malloc");
	  	return EXIT_FAILURE;
   	}

	int decoded_message [8];
	uint64_t threshold = 60;
	volatile char temp;
	int t;	
	uint64_t selected_set[8] = {12,25,30,140,200,250,300,400};
 	uint64_t eviction_set[8][8];
  	uint64_t mask = 0x1FF;
  	uint64_t adrs;
  	uint64_t adrs_setindex;
  	int line_count = 0;

 	 for (int i=0; i<8; i++) {
        	  line_count = 0;
       		 for (int j=0; j<BUFF_SIZE; j= j+8){
        	      adrs =(uint64_t) (buffer + j);
             	      adrs_setindex = (adrs>>6) & mask;
             		 if (selected_set[i] == adrs_setindex) {
               			 eviction_set[i][line_count] = adrs;
               			 line_count = line_count + 1;
               			// printf("line count = %d",line_count);
              }
        }
  }

	// prime the selected cache sets
	for (int i = 0; i<10000; i++) {
		for (int set_index = 0; set_index < 8; set_index++) {
			for (int line = 0; line < 8; line ++) {
			        t =(int) buffer_access_index[set_index][line];
				temp = buffer[t];
			}
		}
	}
	
	printf("Receiver now listening.\n");
	//add some delay 
		for (int i = 0; i < 10000000; i++) { 
		}

	uint64_t latency[8][8] = {0};
	bool listening = true;
	int  output = 0;
	int sum = 0;
	while (listening) {

		// Put your covert channel code here
		//Probe the relevant address
		for (int set_index = 0; set_index < 8; set_index++) {
			for (int line = 0; line < 8; line ++) {
				latency[set_index][line] = measure_one_block_access_time(eviction_set[set_index][ line]);	
				//printf("% \n", latency[set_index][line]);		
			}
		}
	printf("probe done");
	int incr = 0;
	// Decode the message 
	for (int set_index = 0; set_index < 8; set_index++) {
	     sum = 0;
             for (int line = 0; line < 8; line ++) {
		if (latency[set_index][line] > threshold) {
                      sum = sum +1;
                 }
	     }
		if (sum > 3) {
			output = output+incr; 
		} else  {
			output = output;
		}
		
		incr = incr*2;
	}
	printf ("%d", output);
	listening = false; 
	}

	printf("Receiver finished.\n");
	return 0;
}
