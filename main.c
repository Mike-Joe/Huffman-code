//#include "folder.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "extract.h"
#define MAX_STRING 1000
int main()
{
    char f_name[MAX_STRING];
   	int x;

	printf("1.Compress \t 2.Decompress\n");
	scanf("%d",&x);
	printf("enter file name: ");
     scanf("%s", f_name);

	if(x == 1)
		compress_file(f_name);
	else
		filesinit(f_name);

    return 0;
}
