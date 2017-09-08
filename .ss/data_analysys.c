#include <stdio.h>
#include <string.h>

#define arraylength 20


int main ()
{
    int data[arraylength],address[arraylength],pre_data[arraylength];
    int temp = 0,temp1 = 0;

    for (int i = 0; i < arraylength; i++)
    {
	printf("address:");
        scanf("%d", &temp);
	printf("data:");
        scanf("%d", &temp1);
        if(address[i-1] > temp){
		printf("less value\n");
		for(int j=0;j<arraylength;j++){
			pre_data[j]=data[j];
		}
		i=0;
		for (int j =0; j < arraylength; j++ )   
    		{
			if(pre_data[j] != data[i])
				printf("error\n");
			else
				printf("no error\n");
		}	
		
	}
	address[i]= temp ;
	data[i]=temp1;

    }

   /* for (int i =0; i < arraylength; i++ )   
    {
            printf("%d \t %d\n", address[i],data[i]);
    }
*/

}




