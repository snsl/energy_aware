#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>

/* variables to change depending on the contents of the lsf logdir */
/********************************************************************/
#define NUMBEROFNODES 65									//total number of nodes in the system including head
#define LOGFILEPARSETIME 1366954671								//the log files were parsed on April 7th, 2012 at 14:37
#define NUMBEROFTOTALJOBS 200000 								//total number of jobs since system initialization
#define energysavingnodes  4									//how many energy saving nodes to be assigned
#define spindownthreshold 300									//how long should we wait before putting a node into low-power mode
#define poweruppenalty 60									//how long a job should wait for a node to be powered on
#define currentconfiguration 2									//which configuration we are testing
char *LOGDIR = "/usr/src/erss2012/code/"; 							//the log directory
char *HOSTNAMESTOWORKON = "/usr/src/erss2012/code/hosts_to_work_on";				//this log file has the information about assigned hosts for each job
char *JOBCLEANTIMES = "/usr/src/erss2012/code/job_clean_times_by_ids";				//this log file has the information about clean times for each job
char *JOBEXECUTETIMES = "/usr/src/erss2012/code/job_execute_times_by_ids";			//this log file has the information about execute times for each job
char *JOBFINISHDISPATCHTIMES = "/usr/src/erss2012/code/job_finish_dispatch_time_by_ids";	//this log file has the information about finish dispatch times for each job
char *JOBFINISHFROMHOSTS = "/usr/src/erss2012/code/job_finish_from_host_by_ids";		//this log file has the information about finish hosts for each job
char *JOBFINISHSUBMISSIONTIMES = "/usr/src/erss2012/code/job_finish_submission_time_by_ids";	//this log file has the information about finish submission times for each job
char *JOBFINISHTIMES = "/usr/src/erss2012/code/job_finish_times_by_ids";			//this log file has the information about finish times for each job
char *JOBFINISHUSERIDS = "/usr/src/erss2012/code/job_finish_user_ids_by_ids";			//this log file has the information about finish user ids for each job
char *JOBMOVETIMES = "/usr/src/erss2012/code/job_move_times_by_ids";				//this log file has the information about move times for each job
char *JOBMOVEUSERIDS = "/usr/src/erss2012/code/job_move_user_ids_by_ids";			//this log file has the information about move user ids for each job
char *JOBNEWFROMHOSTS = "/usr/src/erss2012/code/job_new_submit_from_host_by_ids";		//this log file has the information about new hosts for each job
char *JOBNEWTIMES = "/usr/src/erss2012/code/job_new_times_by_ids";				//this log file has the information about new times for each job
char *JOBNEWUSERIDS = "/usr/src/erss2012/code/job_new_user_ids_by_ids";				//this log file has the information about new user ids for each job
char *JOBREQUEUETIMES = "/usr/src/erss2012/code/job_requeue_times_by_ids";			//this log file has the information about requeue times for each job
char *JOBSIGNALTIMES = "/usr/src/erss2012/code/job_signal_times_by_ids";			//this log file has the information about signal times for each job
char *JOBSIGNALUSERIDS = "/usr/src/erss2012/code/job_signal_user_ids_by_ids";			//this log file has the information about signal user ids for each job
char *JOBSTARTACCEPTTIMES = "/usr/src/erss2012/code/job_start_accept_times_by_ids";		//this log file has the information about start accept times for each job
char *JOBSTARTTIMES = "/usr/src/erss2012/code/job_start_times_by_ids";				//this log file has the information about start times for each job
char *JOBSTATUSTIMES = "/usr/src/erss2012/code/job_status_times_by_ids";			//this log file has the information about status times for each job
#define LOGFILENUMBER 33									//total number of log files since system initialization
#define MAXHOSTNAMELENGTH 21									//the maximum length for the name of a node
#define BUFFERSIZE 8192										//buffer size to read log files line by line
/********************************************************************/

/* declare arrays to hold the results */
/**************************************/
char hosts_to_work_on[NUMBEROFTOTALJOBS][NUMBEROFNODES*MAXHOSTNAMELENGTH];
int job_clean_times_by_ids[NUMBEROFTOTALJOBS];
int job_execute_times_by_ids[NUMBEROFTOTALJOBS];
int job_finish_dispatch_times_by_ids[NUMBEROFTOTALJOBS];
char job_finish_from_host_by_ids[NUMBEROFTOTALJOBS][MAXHOSTNAMELENGTH];
int job_finish_submission_time_by_ids[NUMBEROFTOTALJOBS];
int job_finish_times_by_ids[NUMBEROFTOTALJOBS];
int job_finish_user_ids_by_ids[NUMBEROFTOTALJOBS];
int job_move_times_by_ids[NUMBEROFTOTALJOBS];
int job_move_user_ids_by_ids[NUMBEROFTOTALJOBS];
char job_new_submit_from_host_by_ids[NUMBEROFTOTALJOBS][MAXHOSTNAMELENGTH];
int job_new_times_by_ids[NUMBEROFTOTALJOBS];
int job_new_user_ids_by_ids[NUMBEROFTOTALJOBS];
int job_requeue_times_by_ids[NUMBEROFTOTALJOBS];
int job_signal_times_by_ids[NUMBEROFTOTALJOBS];
int job_signal_user_ids_by_ids[NUMBEROFTOTALJOBS];
int job_start_accept_times_by_ids[NUMBEROFTOTALJOBS];
int job_start_times_by_ids[NUMBEROFTOTALJOBS];
int job_status_times_by_ids[NUMBEROFTOTALJOBS];
/**************************************/

/* function prototypes */
/***********************/
int string_to_int(char *string);
void trimstring(char *string, char a_letter);
void overlap( int an_array[], int offset);
int non_decreasing(int an_array[], int num_elements);
int array_include_integer(int an_array[], int num_elements, int a_number);
int find_aggregate_time(int an_array[], int num_elements);
void check_penalty(int an_array[], int num_elements);
unsigned long long fact(int a_number);
unsigned long long combinatorial(int first, int second);
void mi_factorial(mpz_t mi_result, unsigned long n, unsigned long stop);
int next_pair(int values[], int r, int n);
/***********************/

/* main function */
/*****************/
int main()
{
	/* file open variables */
	FILE *file;
	
	/* open the log files */
	file = fopen(HOSTNAMESTOWORKON, "r");
	int counter = 0;
	int counter2 = 0;

	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			strcpy(hosts_to_work_on[counter], line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBCLEANTIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_clean_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBEXECUTETIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_execute_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBFINISHDISPATCHTIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_finish_dispatch_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBFINISHFROMHOSTS, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			strcpy(job_finish_from_host_by_ids[counter], line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBFINISHSUBMISSIONTIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_finish_submission_time_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBFINISHTIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_finish_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBFINISHUSERIDS, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_finish_user_ids_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBMOVETIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_move_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBMOVEUSERIDS, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_move_user_ids_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBNEWFROMHOSTS, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			strcpy(job_new_submit_from_host_by_ids[counter], line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBNEWTIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_new_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBNEWUSERIDS, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_new_user_ids_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBREQUEUETIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_requeue_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBSIGNALTIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_signal_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBSIGNALUSERIDS, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_signal_user_ids_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBSTARTACCEPTTIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_start_accept_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBSTARTTIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_start_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	file = fopen(JOBSTATUSTIMES, "r");
	counter = 0;
	if (file != NULL)
	{
		char line[BUFFERSIZE];
		while ( fgets ( line, sizeof line, file ) != NULL )
		{
			job_status_times_by_ids[counter] = string_to_int(line);
			counter++;
		}
	}
	fclose(file);

	int client_time_starts[NUMBEROFTOTALJOBS] = {0};
	int client_time_ends[NUMBEROFTOTALJOBS] = {0};
	int server_time_starts[NUMBEROFTOTALJOBS] = {0};
	int server_time_ends[NUMBEROFTOTALJOBS] = {0};
	
	for(counter=0; counter<NUMBEROFTOTALJOBS; counter++)
	{
		if(job_finish_submission_time_by_ids[counter]+ job_new_times_by_ids[counter]+ job_execute_times_by_ids[counter]+ job_finish_dispatch_times_by_ids[counter]+ job_start_accept_times_by_ids[counter]+ job_start_times_by_ids[counter]+ job_signal_times_by_ids[counter]+ job_finish_times_by_ids[counter]+ job_status_times_by_ids[counter]+ job_clean_times_by_ids[counter])
		{
			if(job_finish_submission_time_by_ids[counter])
			{
				if(job_clean_times_by_ids[counter])
				{
					server_time_ends[counter] = job_clean_times_by_ids[counter];
					server_time_starts[counter] = job_finish_submission_time_by_ids[counter];
					if(job_finish_dispatch_times_by_ids[counter])
					{
						client_time_ends[counter] = job_clean_times_by_ids[counter];
						client_time_starts[counter] = job_finish_dispatch_times_by_ids[counter];
					}
				}
				else
				{
					server_time_ends[counter] = LOGFILEPARSETIME;
					server_time_starts[counter] = job_finish_submission_time_by_ids[counter];
					if(job_finish_dispatch_times_by_ids[counter])
					{
						client_time_ends[counter] = LOGFILEPARSETIME;
						client_time_starts[counter] = job_finish_dispatch_times_by_ids[counter];
					}
				}
			}
			else
			{
				server_time_ends[counter] = LOGFILEPARSETIME;
				server_time_starts[counter] = job_new_times_by_ids[counter];
				client_time_ends[counter] = LOGFILEPARSETIME;
				client_time_starts[counter] = job_start_times_by_ids[counter];
			}
		}
	}
	
	int index = -1;
	int indices[NUMBEROFNODES] = {-1};
	int sums[NUMBEROFNODES] = {0};
	int counter4;
/*	for(counter=0; counter<NUMBEROFNODES; counter++)*/
/*	{*/
/*		int offset = -2;*/
/*		int overlap_offset = 0;*/
/*		int times_to_overlap[NUMBEROFTOTALJOBS] = {0};*/
/*		int num_elements = 0;*/
/*		for(counter2=0; counter2<NUMBEROFTOTALJOBS; counter2++)*/
/*		{*/
/*			index = -1;*/
/*			for(counter4 = 0; counter4 < NUMBEROFNODES; counter4++)*/
/*			{*/
/*				indices[counter4] = -1;*/
/*			}*/

/*			if(!strncmp(&(job_new_submit_from_host_by_ids[counter2][1]), "cn", 2) || !strncmp(&(job_new_submit_from_host_by_ids[counter2][0]), "cn", 2))*/
/*			{*/
/*				trimstring(job_new_submit_from_host_by_ids[counter2], '"');*/
/*				index = string_to_int(&job_new_submit_from_host_by_ids[counter2][2]);*/
/*			}*/
/*			if(!strncmp(&(job_new_submit_from_host_by_ids[counter2][4]), "head", 4))*/
/*			{*/
/*				index = 0;*/
/*			}*/
/*			if(!strncmp(&(hosts_to_work_on[counter2][5]), ".", 1))*/
/*			{*/
/*				char str1[2];*/
/*				strncpy(str1, &hosts_to_work_on[counter2][3], 2);*/
/*				indices[0] = string_to_int(str1);*/
/*			}*/
/*			if(!strncmp(&(hosts_to_work_on[counter2][4]), "head", 4))*/
/*			{*/
/*				indices[0] = 0;*/
/*			}*/
/*			if(!strncmp(&(hosts_to_work_on[counter2][1]), "cn", 2) && strncmp(&(hosts_to_work_on[counter2][5]), ".", 1))*/
/*			{*/
/*				int long_offset = 3;*/
/*				char str2[2];*/
/*				int counter3 = 0;*/
/*				strncpy(str2, &hosts_to_work_on[counter2][long_offset], 2);*/
/*				indices[counter3] = string_to_int(str2);*/
/*				while(strcmp(&hosts_to_work_on[counter2][long_offset+3], "\n"))*/
/*				{*/
/*					char str3[2];*/
/*					counter3++;*/
/*					long_offset += 6;*/
/*					strncpy(str3, &hosts_to_work_on[counter2][long_offset], 2);*/
/*					indices[counter3] = string_to_int(str3);*/
/*				}*/
/*			}*/

/*			if(array_include_integer(indices, NUMBEROFNODES, counter))*/
/*			{*/
/*				offset += 2;*/
/*				times_to_overlap[offset] = client_time_starts[counter2];*/
/*				times_to_overlap[offset+1] = client_time_ends[counter2];*/
/*			}*/
/*			if(index == counter)*/
/*			{*/
/*				offset += 2;*/
/*				times_to_overlap[offset] = server_time_starts[counter2];*/
/*				times_to_overlap[offset+1] = server_time_ends[counter2];*/
/*			}*/
/*		}*/

/*		for(counter2=0; counter2<NUMBEROFTOTALJOBS; counter2++)*/
/*		{*/
/*			if(times_to_overlap[counter2] == 0 && times_to_overlap[counter2+1] == 0)*/
/*			{*/
/*				num_elements = counter2;*/
/*				break;*/
/*			}	*/
/*		}*/
/*		*/
/*		int interarray[num_elements];*/
/*		for(counter2=0; counter2<num_elements; counter2++)*/
/*		{*/
/*			interarray[counter2] = times_to_overlap[counter2];*/
/*		}*/

/*		while(!non_decreasing(interarray, num_elements))*/
/*		{*/
/*			for(overlap_offset = 0; overlap_offset<=(num_elements-4); overlap_offset += 2)*/
/*			{*/
/*				overlap(interarray, overlap_offset);*/
/*			}*/
/*		}*/
/*	*/
/*		sums[counter] += find_aggregate_time(interarray, num_elements);*/
/*	}*/

	/* ENERGY SAVING CONFIGURATIONS*/
	int firstconfoffset = 0;
	index = -1;
	int nodestobeassigned[NUMBEROFTOTALJOBS][energysavingnodes];
	
	for(counter = 0; counter<NUMBEROFTOTALJOBS; counter++)
	{
		if(!strncmp(&job_new_submit_from_host_by_ids[counter][1], "cn", 2) || !strncmp(&job_new_submit_from_host_by_ids[counter][1], "hp", 2))
		{
			if(currentconfiguration == 1)
			{
				if(job_new_user_ids_by_ids[counter])
				{
					firstconfoffset = job_new_user_ids_by_ids[counter] % (NUMBEROFNODES-1);
				}
			
				for(counter2=0; counter2<energysavingnodes; counter2++)
				{
					if((firstconfoffset + counter2 + 1) > (NUMBEROFNODES - 1))
					{
						nodestobeassigned[counter][counter2] = (firstconfoffset + counter2 + 1) % (NUMBEROFNODES-1);
					}
					else
					{
						nodestobeassigned[counter][counter2] = firstconfoffset + counter2 + 1;
					}
				}
			}
			else if(currentconfiguration == 2)
			{
				mpz_t mi_fact1;
				mpz_t mi_fact2;
				mpz_t mi_result;
				mpz_t user_id;
				mpz_init(mi_fact1);
				mpz_init(mi_fact2);
				mpz_init(mi_result);
				mpz_init(user_id);
				char* result;
				mi_factorial(mi_fact1, NUMBEROFNODES -1, energysavingnodes);
				mi_factorial(mi_fact2, NUMBEROFNODES -1 - energysavingnodes, 0);
				mpz_cdiv_q(mi_result, mi_fact1, mi_fact2);
				mpz_init_set_ui(user_id, job_new_user_ids_by_ids[counter]);
				mpz_tdiv_r(mi_result, user_id, mi_result);
				result = malloc(mpz_sizeinbase(mi_result, 10) + 2);
				mpz_get_str(result, 10, mi_result);
			
				int set[energysavingnodes];

				for (counter2 = 0; counter2 < energysavingnodes; ++counter2)
					set[counter2] = counter2;

				counter2 = 2;
				while (next_pair(set, energysavingnodes, NUMBEROFNODES - 1))
				{
					counter2++;
					if(counter2 == string_to_int(result))
					break;
				}
	
				for(counter2=0; counter2<energysavingnodes; counter2++)
				{
					nodestobeassigned[counter][counter2] = set[counter2] + 1;
				}
			}
			else
			{
				printf("Please choose configuration 1 or 2\n");
			}
		}
	}
	
	for(counter=1; counter<NUMBEROFNODES; counter++)
	{
		int offset = -2;
		int overlap_offset = 0;
		int times_to_overlap[NUMBEROFTOTALJOBS] = {0};
		int num_elements = 0;
		for(counter2=0; counter2<NUMBEROFTOTALJOBS; counter2++)
		{
			if(array_include_integer(nodestobeassigned[counter2], energysavingnodes, counter))
			{
				offset += 2;
				times_to_overlap[offset] = server_time_starts[counter2];
				times_to_overlap[offset+1] = server_time_ends[counter2];
			}
		}

		for(counter2=0; counter2<NUMBEROFTOTALJOBS; counter2++)
		{
			if(times_to_overlap[counter2] == 0 && times_to_overlap[counter2+1] == 0)
			{
				num_elements = counter2;
				break;
			}	
		}

		int interarray[num_elements];
		for(counter2=0; counter2<num_elements; counter2++)
		{
			interarray[counter2] = times_to_overlap[counter2];
		}

		while(!non_decreasing(interarray, num_elements))
		{
			for(overlap_offset = 0; overlap_offset<=(num_elements-4); overlap_offset += 2)
			{
				overlap(interarray, overlap_offset);
			}
		}
	
		check_penalty(interarray, num_elements);

		sums[counter] += find_aggregate_time(interarray, num_elements);
	}

	for(counter=0; counter<NUMBEROFNODES; counter++) {
		printf("%d\n", sums[counter]);
	}
	
	return 0;
}
/* end of main function */
/************************/

int next_pair(int values[], int r, int n)
{
	int counter = r - 1;
	++values[counter];
	while ((counter >= 0) && (values[counter] >= n - r + 1 + counter)) {
		--counter;
		++values[counter];
	}

	if (values[0] > n - r)
		return 0;

	for (counter = counter + 1; counter < r; ++counter)
		values[counter] = values[counter - 1] + 1;

	return 1;
}

void mi_factorial(mpz_t mi_result, unsigned long n, unsigned long stop)
{
    unsigned long int n1 = n;
    mpz_set_ui(mi_result, 1);
    while(n1 > stop) {
        mpz_mul_ui(mi_result, mi_result, n1--);
    }
}
unsigned long long combinatorial(int first, int second)
{
	unsigned long long ret;
	unsigned long long numerator = 1;
	unsigned long long denominator = 1;
	int counter;
	
	for(counter=1; counter<=second; counter++)
	{
		numerator = numerator * first;
		first--;
	}
	
	denominator = fact(second);
	
	ret = numerator / denominator;
	
	return ret;
}

unsigned long long fact(int a_number)
{
	unsigned long long ret = 1;
	int counter;
	while(a_number)
	{
		ret = ret * a_number;
		a_number--;
	}

	return ret;
}

void check_penalty(int an_array[], int num_elements)
{
	int counter = 0;
	for(counter = 0; counter<num_elements; counter += 2)
	{
		if(an_array[counter])
		{
			if(an_array[counter-1] == 0)
			{
				an_array[counter+1] += poweruppenalty;
				continue;
			}

			if((an_array[counter] - an_array[counter-1]) >= spindownthreshold)
			{
				an_array[counter+1] += poweruppenalty;
			}
		}
	}
}

int find_aggregate_time(int an_array[], int num_elements)
{
	int ret = 0;
	int counter;
	for(counter=num_elements-1; counter>=1; counter -= 2)
	{
		ret += an_array[counter]-an_array[counter-1];
		if(an_array[counter-1] == 0)
		{
			break;
		}
	}

	return ret;
}

int non_decreasing(int an_array[], int num_elements)
{
	int ret = 1;
	int counter;
	for(counter = 1; counter<num_elements; counter++)
	{
		if(an_array[counter]<an_array[counter-1])
		{
			ret = 0;
			break;
		}
	}
	
	return ret;
}

int array_include_integer(int an_array[], int num_elements, int a_number)
{
	int ret = 0;
	int counter;
	for(counter = 0; counter<num_elements; counter++) {
		if(an_array[counter] == a_number)
		{
			ret = 1;
			break;
		}
	}
	
	return ret;
}

void overlap(int an_array[], int offset)
{
	int counter;
	int holder1;
	int holder2;
	
	if (an_array[offset+2] > an_array[offset+1])
	{
	}
	if ((an_array[offset+2] <= an_array[offset+1]) && (an_array[offset+2] >= an_array[offset]) && (an_array[offset+3] > an_array[offset+1]))
	{
		an_array[offset+2] = an_array[offset];
		an_array[offset] = 0;
		an_array[offset+1] = 0;
	}
	if ((an_array[offset+2] <= an_array[offset+1]) && (an_array[offset+2] >= an_array[offset]) && (an_array[offset+3] <= an_array[offset+1]))
	{
		an_array[offset+2] = an_array[offset];
		an_array[offset+3] = an_array[offset+1];
		an_array[offset] = 0;
		an_array[offset+1] = 0;
	}
	if ((an_array[offset+2] < an_array[offset]) && (an_array[offset+3] >= an_array[offset]) && (an_array[offset+3] <= an_array[offset+1]))
	{
		an_array[offset+3] = an_array[offset+1];
		an_array[offset] = 0;
		an_array[offset+1] = 0;
	}
	if ((an_array[offset+3] < an_array[offset]))
	{
		holder1 = an_array[offset+2];
		holder2 = an_array[offset+3];
		an_array[offset+2] = an_array[offset];
		an_array[offset+3] = an_array[offset+1];
		an_array[offset] = holder1;
		an_array[offset+1] = holder2;
	}
	if ((an_array[offset+2] < an_array[offset]) && (an_array[offset+3] > an_array[offset+1]))
	{
		an_array[offset] = 0;
		an_array[offset+1] = 0;
	}

}

int string_to_int(char *string)
{
	int len = strlen(string);
	int counter;
	int result=0;

	if(string[len-1] == '\n')
		len--;
	for(counter=0; counter<len; counter++)
	{
		result += pow(10,(len-counter-1)) * (string[counter] - 48);
	}
	
	return result;
}

void trimstring(char *string, char a_letter)
{
	int counter = 0;
	for(counter = 0; counter < strlen(string); counter++ )
	{
		if(string[counter] == a_letter)
			strcpy(string + counter, string + counter + 1);
	}
}
