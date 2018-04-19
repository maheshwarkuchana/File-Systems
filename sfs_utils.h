#include<stdint.h>
#ifndef SFS_API_UTIL
#define SFS_API_UTIL

char vd[100][30000];
int fatID = 0;

void FAT_init(FileAllocationTable *fat)
{
	for(fatID= 2; fatID < FAT_SIZE-1; fatID++)
	{
		fat->table[fatID].db_index = fatID*sizeof(DiskBlock);
		fat->table[fatID].next = EOF;
		fat->table[fatID].free = 1;
	}
	fat->count = 0;
}


int FAT_getFreeNode(FileAllocationTable * fat)
{
	if(++fat->count < FAT_SIZE -1)	
	{
		return fat->count;
	}
	else
	{
		int i;
		for(i = 2; i < FAT_SIZE -1; i++)
		{
			if(fat->table[i].free)
			{
				return i;
			}
		}
	}
	fat->count--;
	printf("Error : Cannot get free block in Fat. \n");
	exit(0);
	return -5;
}

char * FAT_getPartFile(FileDescriptor file, FileAllocationTable fat, int length)
{
	int curr, next, red = 0;
	int positionOnDisk = 0;
	void * positionOnBuff = 0;
	char buff[length];
	
	curr = file.fat_index;
	next = fat.table[curr].next;
	for(; red* sizeof(DiskBlock) < length && curr != EOF; curr = fat.table[curr].next, next = fat.table[curr].next,red++)
	{
		positionOnDisk = fat.table[curr].db_index;
		positionOnBuff = (void *)((uintptr_t)(buff) + red *sizeof(DiskBlock));
		//(void *)
		read_blocks(positionOnDisk, length, positionOnBuff);
	}
	
	return strdup(buff);
}

char * FAT_getFullFile(FileDescriptor file, FileAllocationTable fat)
{
	return FAT_getPartFile(file, fat, file.size);
}

void DirectoryDescriptor_init(DirectoryDescriptor *root)
{
	int i;
	for( i = 0; i < MAX_FILE ; i++)
	{
		root->table[i].size = EMPTY;
	}
}
int DirectoryDescriptor_getFreeSpot(DirectoryDescriptor * root)
{
	if(++root->count < MAX_FILE)
	{
		return root->count;
	}
	else
	{
		int i;
		for(i = 0; i < MAX_FILE ; i++)
		{
			if(root->table[i].size == EMPTY)
			{
				return i;
			}
		}
	}
	
	root->count--;
	printf("Error : cannot get free block  DD \n");
	exit(0);
	return -1;
}

int getIndexOfFileInDirectory(char *name, DirectoryDescriptor *rootDir)
{
	int i, fileID = -1;
	for(i = 0; i < MAX_FILE; i++)
	{
		if(!strcmp(rootDir->table[i].filename, name))
		{
			fileID = i;
			break;
		}
	}
	
	if(fileID != -1)
	{
		if(rootDir -> table[fileID].fas.opened == 1)
		{
			return fileID;
			//return -2;
		}
	}
	return fileID;
}

void FileDescriptor_createFile(char *name, FileDescriptor * file)
{
	strcpy(file->filename, name);
	file->fas.opened = 1;
	file->fas.rd_ptr = 0;
	file->fas.wr_ptr = 0;
	file->size = 0;
	file->timestamp = time(NULL);
}

int FileDescriptor_getNextWritable(FileDescriptor *file, FileAllocationTable *fat)
{
	int currNode = file->fat_index;
	while(fat->table[currNode].next != EOF)
	{
		currNode = fat->table[currNode].next;
		file->fas.wr_ptr = currNode;
	}
	
	int nextNode = FAT_getFreeNode(fat);
	fat->table[currNode].next = nextNode;
	fat->table[nextNode].next = EOF;
	
	return nextNode < 0? fat->table[nextNode].db_index : -1;
}

void VirtualDisk_write(int address, int length, void *buff)
{
	char *string = (char *)buff;
	printf("%i \n", string[length+1]);
	strncpy(vd[address], string, length);
	
}

void VirtualDisk_read(int address, void *buff, int length)
{
	char *string = (char *)buff;
	vd[address][length] = '\0';
	strncpy(string , vd[address], length);
}

int FreeBlockList_getBlock(freeblocklist *fbl)
{
	int i;
	for(i = 2; i < NB_BLOCK -1; i++)
	{
		if(fbl->list[i] == 0)
		{
			fbl->list[i] = 1;
			return i;
		}
	}
	return -1;
}

#endif


