#ifndef SFS_API_H
#define SFS_API_H

#include "disk_emu.h"
#include "sfs_header.h"
#include "sfs_utils.h"
DirectoryDescriptor root;
FileAllocationTable fat;

static int opened_files = 0;

void mksfs(int fresh)
{
	if(fresh)
	{
		init_fresh_disk("/home/prakash/Desktop/os/project/disk.sfs",sizeof(DiskBlock), NB_BLOCK);
		
		FAT_init(&fat);
		DirectoryDescriptor_init(&root);
		
		write_blocks(0,1,(void *)&root);
		write_blocks(1,1,(void *)&fat);
	}
	else
	{
		init_disk("disk.sfs",sizeof(DiskBlock),NB_BLOCK);
		read_blocks(1,1,(void *)&fat);
		read_blocks(0,1,(void *)&root);
	}
}

void sfs_ls()
{
	int i;
	printf("\n");
	for(i = 0; i < MAX_FILE; i++)
	{
		//if(root.table[i].size > 0)
		if(root.table[i].fas.opened == 1)
		{
			int b = root.table[i].size;
			char * tm = ctime(&root.table[i].timestamp);
			tm[24] = '\0';
			printf("%25s \t %d b \t %s \n", tm, b, root.table[i].filename);
		}
	}
}

int sfs_open(char * name)
{
	int fileID = getIndexOfFileInDirectory(name, &root);
	if(fileID != -1)
	{
		return fileID;
	}
	
	fileID = opened_files++;
	FileDescriptor_createFile(name, &(root.table[fileID]));
	
	root.table[root.count].fat_index = FAT_getFreeNode(&fat);
	fat.table[fat.count].next = EOF;
	
	write_blocks(0,1,(void*)&root);
	write_blocks(1,1,(void *)&fat);
	
	return fileID;
}

int sfs_close(int fileID)
{
	if(opened_files <= fileID)
	{
		printf("Error : file %d does not exist",fileID);
		return -1;
	}
	
	root.table[fileID].fas.opened = 0;
	opened_files --;
	return 0;
}

int sfs_write(int fileID, char *buff, int length)
{
	if(opened_files <= fileID)
	{
		printf("Error %d does not exist",fileID);
		return 0;
	}
	
	int i, addr = 2;
	for(i = 0; i < length ; i++)
	{
		if(addr !=-1)
		{	
			write_blocks(addr, 1,(void *)&buff[i]);
			root.table[fileID].size++;
		}
		else
		{
			printf("Error: Disk is full .\n");
			break;
		}
	}


	write_blocks(0,1,(void *)&root);
	write_blocks(1,1,(void *) &fat);
	
	return root.table[fileID].size;
}


int sfs_read(int fileID, char *buf, int length)
{
	if(opened_files-1 <= fileID && root.table[fileID].fas.opened == 0)
	{
		printf("No such file %d is opened \n", fileID);
		return 0;
	}
	
	buf = FAT_getPartFile(root.table[fileID],fat, length);
	return EOF;
}

#endif
	
