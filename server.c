#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
    char *dir;




	if(argc == 7){
		
		for(int i=0; i < argc; i++)
		{
			if(strcmp("-d",argv[i]) == 0){
                dir = argv[i+1];
                printf("%s\n",dir);
            }
		}
	}
	else
	{
		printf("Give correct arguments");
	}



	
	return 0;
}
