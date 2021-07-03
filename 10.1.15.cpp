#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  


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
	if(mat==NULL)
		return NULL;
		
	mat->height=0;
	mat->width=0;	
	return mat;	
}

GSI *gsi_create_with_geometry(unsigned int m, unsigned int n)
{
	GSI *mat;
	mat = (GSI*)malloc(sizeof(unsigned int)*m*n);
	mat->px = (unsigned char*)malloc(sizeof(unsigned int)*m*n);	
	
	if(mat == NULL)
		return NULL;
	
	mat->height = m;
	mat->width = n;
	
	if(mat->px=NULL)
		return NULL;
	
	return mat;	
	
}


GSI *gsi_create_with_geometry_and_color(unsigned int m, unsigned int n, unsigned char color) 
{
	GSI *mat;
	int i,j;
	mat = (GSI*)malloc(sizeof(unsigned int)*m*n);
	
	if(mat == NULL)
		return NULL;
	
	mat->height = m;
	mat->width = n;
	mat->px=(unsigned char*)malloc(sizeof(unsigned int)*m*n);
	
	if(mat->px==NULL)
	{
		free(mat->px);
		return NULL;
	}
	for(i=0;i<n;i++)
	{
		for(j=0;j<m;j++)
		{
			PIX(mat,i,j)=color;
		}
	}
	
	return mat;	
}

void gsi_destroy(GSI *mat)
{
	free(mat);
}

GSI *gsi_create_by_pgm5(char *file_name) 
{
	GSI *mat;
	FILE* f;
	
	char line[30];
	unsigned int sirka,vyska;
	int i, j, col;
	
	f = fopen(file_name, "rb");
	if(f == NULL)
		return NULL;
		
	fgets(line, 10, f);
	
	if(line[0] != 'P' || line[1] != '5')	//subor musi začinať P5
	{
		fclose(f);
		return 0;
	}
	
	do
	{
		fgets(line, 80, f);
	}
	while(line[0] == '#');
		sscanf(line, "%u%u", &sirka, &vyska);
	
	if((mat = gsi_create_with_geometry(sirka,vyska)) == NULL){
		fclose(f);
		return 0;
	}
	
	for(i = 0; i < mat->height; i++){
		for(j = 0; j < mat->width; j++){
			fscanf(f, "%c", &col);
			PIX(mat, i, j) = col;
		}
	}
	
	if(fclose(f) == EOF){
		return 0;
	}
	
	return mat;
}

char gsi_save_as_pgm5(GSI *mat, char *file_name, char *comment) 
{
	FILE *f;
	int max = 0;
	int i, j;
	
	f = fopen(file_name, "wb");
	
	if(f == NULL){
		return 0;
	}

	fprintf(f,"P5\n");
	
	if(comment != NULL){
		fprintf(f, "#");
		fprintf(f, comment);
		fprintf(f, "\n");
	}
	
	for(i = 0; i < mat->height; i++){
		for(j = 0; j < mat->width; j++){
			
			if(PIX(mat, i, j) > max)
			max = PIX(mat, i, j);
			
		}
	}
	
	fprintf(f,"%u %u %d\n", mat->width, mat->height, max);
	
	for(i = 0; i < mat->height; i++){
		for(j = 0; j < mat->width; j++){
			fprintf(f,"%c", PIX(mat, i, j));
		}
	}	
	
	return 1;
}


//budem to robiť tak že obehnem každy jeden prvok matice a bud mu hodnotu zvýšim alebo znížim podla argumentu br
//treba si asi uvedomiť iba jedinu vec a to že ak ma nejaky prvok hodnutu 254 a chceme ju zvýšiť o 2 tak tak jej hodnota bude stale max 255
//toto iste aj na opačnom konci intervalu
void gsi_adjust_contrast(GSI *mat, float br)
{
	int i,j;
	int max = 255;
	int min = 0;
	
	
	for(i=0;i<mat->height;i++)
	{
		for(j=0;j<mat->width;j++)
		{
			if(br>0)
			{
			if(PIX(mat, i, j)+br > max)
				PIX(mat, i, j) = max;
			else
				PIX(mat, i, j) += br;
			}	
			else if(br<0)
			{
			if(PIX(mat, i, j)+br < min)
				PIX(mat, i, j) = min;
			else
				PIX(mat, i, j) += br;
			}
			else
					PIX(mat, i, j);
				
			
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
			printf("%d ",PIX(mat,i,j));
		}
		printf("\n");
	}
	printf("\n");
}

main()
{
	GSI *mat, *obrazok;
	float br = -30;  //posun kontrastu
	
	mat=gsi_create_with_geometry_and_color(7,7,2);
	mat_print(mat);
	
	gsi_adjust_contrast(mat,br);
	mat_print(mat);
	
	/*
	obrazok=gsi_create_by_pgm5("sunset.pgm");
	gsi_adjust_contrast(obrazok,br);
	save_as_pgm5(obrazok,"susnet_2.pgm", "pgm5")
	
	gsi_destroy(obrazok);
	*/
	
	
	gsi_destroy(mat);
	
}
