#include<stdio.h>
#include<string.h>


void main(){	
	int grades[]={1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6};
	int predata[]={1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6};
	int present[]={1,1,0,1,0,2,2,2,15,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6};
   	int final_grades[12]={};
	static int j=0;	

	for(int i=0;i<=23;i++){
		if(predata[i]!=present[i]){
			if(final_grades[j-1]!=grades[i]){
				final_grades[j]=grades[i];
				j++;
			}
		}				
	}
	for(int b=0;b<j;b++){
		printf("%d :",final_grades[b]);	
		for(int final=0;final<=23;final++){
			if(final_grades[b]==grades[final]){
				printf("%d ",present[final]);
			}
		}
		printf("\n");
	}
}
