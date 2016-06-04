#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* variables to change depending on the contents of the lsf logdir */
/********************************************************************/
#define NUMBEROFTOTALJOBS 105000 						//total number of jobs since system initialization
char *LOGFILE = "/usr/share/lsf/work/cluster1/logdir/lsb.events"; 		//the log directory
#define LOGFILENUMBER 108							//total number of log files since system initialization
#define MAXHOSTNAMELENGTH 21							//the maximum length for the name of a node
#define BUFFERSIZE 8192								//buffer size to read log files line by line
#define NUMBEROFNODES 65							//total number of nodes in the system including head
char *RESULTFILE = "/usr/share/lsf/work/cluster1/logdir/lsb.acct";		//information about finished jobs
/********************************************************************/

/* declare arrays to hold the results */
/**************************************/
int job_clean_times_by_ids[NUMBEROFTOTALJOBS];
int job_new_times_by_ids[NUMBEROFTOTALJOBS];
int job_start_times_by_ids[NUMBEROFTOTALJOBS];
int job_start_accept_times_by_ids[NUMBEROFTOTALJOBS];
int job_execute_times_by_ids[NUMBEROFTOTALJOBS];
int job_status_times_by_ids[NUMBEROFTOTALJOBS];
int job_new_user_ids_by_ids[NUMBEROFTOTALJOBS];
char job_new_submit_from_host_by_ids[NUMBEROFTOTALJOBS][MAXHOSTNAMELENGTH];
char hosts_to_work_on[NUMBEROFTOTALJOBS][NUMBEROFNODES][MAXHOSTNAMELENGTH];
int job_move_times_by_ids[NUMBEROFTOTALJOBS];
int job_move_user_ids_by_ids[NUMBEROFTOTALJOBS];
int job_signal_times_by_ids[NUMBEROFTOTALJOBS];
int job_signal_user_ids_by_ids[NUMBEROFTOTALJOBS];
int job_requeue_times_by_ids[NUMBEROFTOTALJOBS];
int job_finish_times_by_ids[NUMBEROFTOTALJOBS];
int job_finish_user_ids_by_ids[NUMBEROFTOTALJOBS];
int job_finish_submission_time_by_ids[NUMBEROFTOTALJOBS];
int job_finish_dispatch_time_by_ids[NUMBEROFTOTALJOBS];
char job_finish_from_host_by_ids[NUMBEROFTOTALJOBS][MAXHOSTNAMELENGTH];
/**************************************/

/* function prototypes */
/***********************/
int string_to_int(char *string);
int find_num_of_digits(int a_number);
int array_of_string_contains(char *string, char array[][MAXHOSTNAMELENGTH], int num_strings_in_array);
/***********************/

/* main function */
/*****************/
int main()
{
	/* file open variables */
	FILE *file;
	int counter = 0;
	int counter2 = 0;
	int counter3 = 0;
	
	char str1[strlen(LOGFILE) + 1 + find_num_of_digits(LOGFILENUMBER)];
    	char str2[find_num_of_digits(LOGFILENUMBER)];
	
	for(counter2 = 0; counter2<=LOGFILENUMBER; counter2++) {
		if(counter2)
		{
			for(counter = 0; counter < find_num_of_digits(counter2); counter ++)
    			{
    				str2[find_num_of_digits(counter2)-counter-1] = ((counter2 % (int)pow(10,(counter+1))) / (int)pow(10,counter)) + 48;
    			}
    		}

    		strcpy(str1,LOGFILE);
    		if(counter2)
    		{	
    			
    			strcat(str1, ".");
    			strcat(str1, str2);
    		}
    		
		file = fopen(str1, "r");
		
		/* variables to assign times for different operations */
		int looking_for = 0;
		int clean_time = 0;
		int new_time = 0;
		int start_time = 0;
		int start_accept_time = 0;
		int execute_time = 0;
		int status_time = 0;
		int new_user_id = 0;
		char new_from_host[MAXHOSTNAMELENGTH];
		int num_hosts_assigned = 0;
		int move_time = 0;
		int move_user_id = 0;
		int signal_time = 0;
		int signal_user_id = 0;
		int requeue_time = 0;
	
		int index = 0;
		int case_number = 0;
	
		if (file != NULL)
		{
			char line[BUFFERSIZE];
		
			while ( fgets ( line, sizeof line, file ) != NULL )
			{
				char* token = strtok(line, " ");
				
				if (!strcmp(token, "\"JOB_CLEAN\""))
				{
					case_number = 1;
				}
				else if (!strcmp(token, "\"JOB_NEW\""))
				{
					case_number = 2;
				}
				else if (!strcmp(token, "\"JOB_START\""))
				{
					case_number = 3;
				}
				else if (!strcmp(token, "\"JOB_START_ACCEPT\""))
				{
					case_number = 4;
				}
				else if (!strcmp(token, "\"JOB_EXECUTE\""))
				{
					case_number = 5;
				}
				else if (!strcmp(token, "\"JOB_STATUS\""))
				{
					case_number = 6;
				}
				else if (!strcmp(token, "\"JOB_MOVE\""))
				{
					case_number = 7;
				}
				else if (!strcmp(token, "\"JOB_SIGNAL\""))
				{
					case_number = 8;
				}
				else if (!strcmp(token, "\"JOB_REQUEUE\""))
				{
					case_number = 9;
				}				
				else
				{
					case_number = -1;
				}
			
				switch (case_number)
				{
					case 1:
						/* JOB_CLEAN */
						looking_for = 1;
						while (token) {
							token = strtok(NULL, " ");
	    						if (looking_for)
	    						{
	    							if(counter == looking_for)
	    							{
	    								clean_time = string_to_int(token);
	    							}
								if(counter == looking_for + 1)
								{
									index = string_to_int(token);
								}
	    						}
	    						counter++;
						}
						job_clean_times_by_ids[index] = clean_time;
						break;
					case 2:
						/* JOB_NEW */
						looking_for = 1;
						while (token) {
							token = strtok(NULL, " ");
	    						if (looking_for)
	    						{
	    							if(counter == looking_for)
	    							{
	    								new_time = string_to_int(token);
	    							}
								if(counter == looking_for + 1)
								{
									index = string_to_int(token);
								}
								if(counter == looking_for + 2)
								{
									new_user_id = string_to_int(token);
								}
								if(counter == looking_for + 28)
								{
									strcpy(new_from_host, token);
								}
	    						}
	    						counter++;
						}
						job_new_times_by_ids[index] = new_time;
						job_new_user_ids_by_ids[index] = new_user_id;
						strcpy(job_new_submit_from_host_by_ids[index], new_from_host);
						break;
					case 3:
						/* JOB_START */
						looking_for = 1;
						while (token) {
							token = strtok(NULL, " ");
	    						if (looking_for)
	    						{
	    							if(counter == looking_for)
	    							{
	    								start_time = string_to_int(token);
	    							}
								if(counter == looking_for + 1)
								{
									index = string_to_int(token);
								}
								if(counter == looking_for + 6)
								{
									num_hosts_assigned = string_to_int(token);
								}
								if(counter > (looking_for + 6) && counter <= (looking_for + 6 + num_hosts_assigned))
								{
									if(!array_of_string_contains(token, hosts_to_work_on[index], NUMBEROFNODES))
									{
										strcpy(hosts_to_work_on[index][counter3], token);
										counter3++;	
									}
								}
	    						}
	    						counter++;
						}
						
						job_start_times_by_ids[index] = start_time;
						break;
					case 4:
						/* JOB_START_ACCEPT */
						looking_for = 1;
						while (token) {
							token = strtok(NULL, " ");
	    						if (looking_for)
	    						{
	    							if(counter == looking_for)
	    							{
	    								start_accept_time = string_to_int(token);
	    							}
								if(counter == looking_for + 1)
								{
									index = string_to_int(token);
								}
	    						}
	    						counter++;
						}
						job_start_accept_times_by_ids[index] = start_accept_time;
						break;
					case 5:
						/* JOB_EXECUTE */
						looking_for = 1;
						while (token) {
							token = strtok(NULL, " ");
	    						if (looking_for)
	    						{
	    							if(counter == looking_for)
	    							{
	    								execute_time = string_to_int(token);
	    							}
								if(counter == looking_for + 1)
								{
									index = string_to_int(token);
								}
	    						}
	    						counter++;
						}
						job_execute_times_by_ids[index] = execute_time;
						break;
					case 6:
						/* JOB_STATUS */
						looking_for = 1;
						while (token) {
							token = strtok(NULL, " ");
	    						if (looking_for)
	    						{
	    							if(counter == looking_for)
	    							{
	    								status_time = string_to_int(token);
	    							}
								if(counter == looking_for + 1)
								{
									index = string_to_int(token);
								}
	    						}
	    						counter++;
						}
						job_status_times_by_ids[index] = status_time;
						break;
					case 7:
						/* JOB_MOVE */
						looking_for = 1;
						while (token) {
							token = strtok(NULL, " ");
	    						if (looking_for)
	    						{
	    							if(counter == looking_for)
	    							{
	    								move_time = string_to_int(token);
	    							}
	    							if(counter == looking_for + 1)
	    							{
	    								move_user_id = string_to_int(token);
	    							}
								if(counter == looking_for + 2)
								{
									index = string_to_int(token);
								}
	    						}
	    						counter++;
						}
						job_move_times_by_ids[index] = move_time;
						job_move_user_ids_by_ids[index] = move_user_id;
						break;
					case 8:
						/* JOB_SIGNAL */
						looking_for = 1;
						while (token) {
							token = strtok(NULL, " ");
	    						if (looking_for)
	    						{
	    							if(counter == looking_for)
	    							{
	    								signal_time = string_to_int(token);
	    							}
								if(counter == looking_for + 1)
								{
									index = string_to_int(token);
								}
	    							if(counter == looking_for + 2)
	    							{
	    								signal_user_id = string_to_int(token);
	    							}
	    						}
	    						counter++;
						}
						job_signal_times_by_ids[index] = signal_time;
						job_signal_user_ids_by_ids[index] = signal_user_id;
						break;
					case 9:
						/* JOB_REQUEUE */
						looking_for = 1;
						while (token) {
							token = strtok(NULL, " ");
	    						if (looking_for)
	    						{
	    							if(counter == looking_for)
	    							{
	    								requeue_time = string_to_int(token);
	    							}
								if(counter == looking_for + 1)
								{
									index = string_to_int(token);
								}
	    						}
	    						counter++;
						}
						job_requeue_times_by_ids[index] = requeue_time;
						break;				
					default:
						break;
				}
				
				looking_for = 0;
				counter = 0;
				counter3 = 0;
			}		
		}
		
		fclose(file);
	}

	/* variables to use to parse information from lsb.acct */
	int finish_time = 0;
	int finish_index = 0;
	int finish_user_id = 0;
	int finish_submission_time = 0;
	int finish_dispatch_time = 0;
	char *finish_from_host;
	file = fopen(RESULTFILE, "r");
		
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			char* token = strtok(line, " ");
			counter = 0;
			
			if (strcmp(token, "\"JOB_FINISH\""))
			{
				printf("Please increase the buffer size\n");
				printf("First token of lsb.acct is supposed to be JOB_FINISH all the time\n");
			}
			
			while (token) {
				token = strtok(NULL, " ");
				if (counter)
	    			{
	    				switch (counter)
	    				{
	    					case 1:
	    						finish_time = string_to_int(token);
	    						break;
	    					case 2:
	    						finish_index = string_to_int(token);
	    						break;
	    					case 3:
	    						finish_user_id = string_to_int(token);
	    						break;
	    					case 6:
	    						finish_submission_time = string_to_int(token);
	    						break;
	    					case 9:
	    						finish_dispatch_time = string_to_int(token);
	    						break;
	    					case 15:
	    						strcpy(finish_from_host, token);
	    						break;
	    					default:
	    						break;
	    				}
	    			}
	    			counter++;
			}
			
			job_finish_times_by_ids[finish_index] = finish_time;
			job_finish_user_ids_by_ids[finish_index] = finish_user_id;
			job_finish_submission_time_by_ids[finish_index] = finish_submission_time;
			job_finish_dispatch_time_by_ids[finish_index] = finish_dispatch_time;
			strcpy(job_finish_from_host_by_ids[finish_index], finish_from_host);
		}
	}
	
	fclose(file);
	
	file = fopen("/home/cek10006/job_clean_times_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_clean_times_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_clean_times_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_new_times_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_new_times_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_new_times_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file); 
	
	file = fopen("/home/cek10006/job_start_times_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_start_times_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_start_times_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file); 	
	
	file = fopen("/home/cek10006/job_start_accept_times_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_start_accept_times_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_start_accept_times_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);	
	
	file = fopen("/home/cek10006/job_execute_times_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_execute_times_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_execute_times_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_status_times_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_status_times_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_status_times_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_new_user_ids_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_new_user_ids_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_new_user_ids_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_new_submit_from_host_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
    		fprintf(file, job_new_submit_from_host_by_ids[counter]);
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/hosts_to_work_on", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		for(counter2 = 0; counter2<NUMBEROFNODES; counter2++) {
			if(strlen(hosts_to_work_on[counter][counter2])){ 
    				fprintf(file, hosts_to_work_on[counter][counter2]);
    			}
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_move_times_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_move_times_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_move_times_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_move_user_ids_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_move_user_ids_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_move_user_ids_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_signal_times_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_signal_times_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_signal_times_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);	

	file = fopen("/home/cek10006/job_signal_user_ids_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_signal_user_ids_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_signal_user_ids_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);

	file = fopen("/home/cek10006/job_requeue_times_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_requeue_times_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_requeue_times_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_finish_times_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_finish_times_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_finish_times_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_finish_user_ids_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_finish_user_ids_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_finish_user_ids_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_finish_submission_time_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_finish_submission_time_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_finish_submission_time_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);	

	file = fopen("/home/cek10006/job_finish_dispatch_time_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
		char c;
    		for(counter2 = find_num_of_digits(job_finish_dispatch_time_by_ids[counter]) - 1; counter2 >= 0; counter2 --)
    		{
    			c = ((job_finish_dispatch_time_by_ids[counter] % (int)pow(10,(counter2+1))) / (int)pow(10,counter2)) + 48;
    			fputc(c, file);
    		}
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);
	
	file = fopen("/home/cek10006/job_finish_from_host_by_ids", "w");
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++) {
    		fprintf(file, job_finish_from_host_by_ids[counter]);
    		if(counter != NUMBEROFTOTALJOBS - 1)
    			fputc('\n', file);
	}
	fclose(file);

	return 0;
}
/* end of main function */
/************************/

int string_to_int(char *string)
{
	int len = strlen(string);
	int counter;
	int result=0;
	
	for(counter=0; counter<len; counter++)
	{
		result += pow(10,(len-counter-1)) * (string[counter] - 48);
	}
	
	return result;
}

int find_num_of_digits(int a_number)
{
	int power = 1;
	int done = 0;
	int result = 0;
	
	while (!done)
	{
		if (abs(a_number) < pow(10, power))
		{
			result = power;
			done = 1;
		}
		power ++;	
	}
	
	return result;
}

int array_of_string_contains(char *string, char array[][MAXHOSTNAMELENGTH], int num_strings_in_array)
{
	int counter;
	int ret = 0;
	
	for(counter = 0; counter<num_strings_in_array; counter++)
	{
		if(!strcmp(string, array[counter]))
		{
			/* found a match */
			ret = 1;
		}
	}
	
	return ret;
}
