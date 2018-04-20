#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "sfs_api.h"

#define MAX_FNAME_LENGTH 12

#define MAX_FD 100

#define MAX_BYTES 30000
#define MIN_BYTES 10000

int main()
{
	int i,j,k;
	int chunksize;
	char *buffer;
	//int fds[MAX_FD];
	int fds;
	//char *names[MAX_FNAME_LENGTH];
	char fname[MAX_FNAME_LENGTH];
	//int filesize[MAX_FD];
	int filesize;
	int error_count = 0;
	int tmp;
	int sel;
	char character;
	mksfs(1);
	while(1)
	{
		int ops;
		printf("enter operations below : ");
		printf("\n 1.list the files \n 2. create a file \n 3. open a file \n 4.write into  file \n 5. exit the system \n");
		scanf("%d",&ops);
		switch(ops)
		{
			case 1 : sfs_ls();
			break;
			
			case 2 : 
				printf("enter the name of the file : ");
				scanf("%s",fname);
				fds = sfs_open(fname);
		
				if(fds < 0)
				{
					fprintf(stderr, "Error: creating file %s",fname);
				}
				else
				{
					printf("file created succesfully \n");
				}
				break;
				
				
			case 3 : 
				
					printf("enter the file name you want to open : ");
					scanf("%s",fname);
					buffer = malloc(MAX_BYTES);
					if(buffer == NULL)
					{
						printf("enter loading into memory \n ");
					}
					fds = sfs_open(fname);
					sfs_read(fds,buffer, 10);
					free(buffer);
					break;
					
			 case 4 : printf("enter the file name you want to open \n");
			 		scanf("%s",fname);
			 		printf("enter the size of data you want to give : ");
			 		scanf("%d",&chunksize);
			 		buffer = malloc(chunksize);
			 		for(i = 0; i < chunksize; i++)
			 		{
			 			buffer[i] = scanf(" %c",&character);
			 		}
			 		fds = sfs_open(fname);
			 		sfs_write(fds, buffer, chunksize);
			 	break;
			 	
			 case 5 : exit(1);
					
		}
	}
}
	
					
				
				 
			
	
	
