#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  //odtialto potrebujem O_RDONLY na Ëitanie file


#define PIX(mat,x,y) (mat->px[(mat->height)*x+y])

typedef struct
{
	unsigned int width;
	unsigned int height;
	unsigned char *px;
}GSI;


GSI *gsi_create_empty(void)
{
	GSI *mat; 
	
	mat = (GSI*)malloc(sizeof(GSI));
	
	return mat;	
}

GSI *gsi_create_with_geometry(unsigned int m, unsigned int n)
{
	GSI *mat;
	
	mat = (GSI*)malloc(sizeof(unsigned int)*m*n);
	
	if(mat == NULL)
		return NULL;
	
	mat->height = m;
	mat->width = n;
	
	for(int i=0;i<m;i++)
	{
		for(int j =0;j<n;j++)
		{
		
		}
	}
	
	return mat;	
	
}


GSI *gsi_create_with_geometry_and_color(unsigned int m, unsigned int n, unsigned char color) 
{
	GSI *mat;
	
	mat = (GSI*)malloc(sizeof(unsigned int)*m*n);
	
	if(mat == NULL)
		return NULL;
	
	mat->height = m;
	mat->width = n;
	
	mat->px=(unsigned char*)malloc(sizeof(unsigned int)*m*n);
	
	for(int i=0;i<m;i++)
	{
		for(int j =0;j<n;j++)
		{
			PIX(mat,m,n)=color;
		}
	}
	
	return mat;	
}

void gsi_destroy(GSI *mat)
{
	free(mat);
}

GSI *gsi_create_by_pgm5(char *file_name) //naËitanie z file dorobiù
{
	GSI *mat;
	char *format,*dat;
	int fr = open(file_name, O_RDONLY);
	int x,y;
	//if ((fr = open(file_name, O_RDONLY) == NULL)
	if(fr ==NULL)
	{
		//printf("subor sa neda optvoriù");
		return NULL;
	}
		
	read(fr,format,2);	//najprv overime Ëi je  na zaËiatku P5
	
	if(format[0]!= 'P'  || format[1]!= '5')
		return NULL;
	
	read(fr,dat,5);  //naËit data do matice
	
	mat->width=dat[0];
	mat->height=dat[2];
	
	PIX(mat,x,y)= read(fr,dat,mat->height*mat->width);
	
	return mat;
}

char gsi_save_as_pgm5(GSI *img, char *file_name, char *comment) //dorobiù
{
	
}


//budem to robiù tak ûe obehnem kaûdy jeden prvok matice a bud mu hodnotu zv˝öim alebo znÌûim podla argumentu br
//treba si asi uvedomiù iba jedinu vec a to ûe ak ma nejaky prvok hodnutu 254 a chceme ju zv˝öiù o 2 tak tak jej hodnota bude sstale max 255
//toto iste aj na opaËnom konci intervalu
void gsi_adjust_contrast(GSI *mat, float br)
{
	int i,j;
	
	
	for(i=0;i<mat->height;i++)
	{
		for(j=0;j<mat->width;j++)
		{
			PIX(mat,i,j) += br;
			
			if (PIX(mat,i,j) > 255)
				PIX(mat,i,j) =255;
			if (PIX(mat,i,j) < 0)
				PIX(mat,i,j) =0;	
			
		}
	}
}

//aby som ju aj ked vlastne nemusim videl
void mat_print(GSI *mat)
{
	int i, j;
	
	for (i=0;i<mat->height;i++)
	{
		for (j=0;j<mat->width;j++)
		{
			printf("%3.0f",PIX(mat,i,j));
		}
		printf("\n");
	}
	printf("\n");
}

main()
{
	GSI *mat;
	mat=gsi_create_with_geometry_and_color(7,3,60);
	
	float br = 10;  //posun kontrastu
	
	gsi_adjust_contrast(mat,br);
	mat_print(mat);
	
	
	
	gsi_destroy(mat);
	
}
