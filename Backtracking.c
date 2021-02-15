#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void shift_right(int **matrix, int row, int N);
int unique_columns(int **matrix, int N, int row, int **matrixCopy, int bot_flag, char *colors[]);
void print_colors(int **matrix, int N, char *colors[]);
int check_above(int **matrix, int row, int N);
int are_rows_same(int **matrix, int **matrixCopy, int i, int N);

int main(){
	int i; /* ITERATOR */
	int j; /* ITERATOR */
	int k; /* ITERATOR */
	int x; /* RETURNING VALUE FROM FUNCTION */
	int N; /* N FROM USER */
	int N_flag; /* CONTROLLER TO CHECK IF 3<=N<=8 */
	int true_input_flag; /* CONTROLLER TO CHECK COLOR INPUT IF IT IS IN COLOR ARRAY */
	int include_flag; /* CONTROLLER TO CHECK COLOR INPUT IF IT IS IN COLOR ARRAY */
	char color[10];
	while(1)
	{	
		char *colors[] = {"RED","BLUE","GREEN","BLACK","YELLOW","ORANGE","WHITE","PINK"};
		printf("--------------------------------------------------------------\nCOLORS: ");
		for(i=0;i<8;i++)
			printf("%s ",colors[i]);
		printf("\n");
		N_flag=0;
		
		while(N_flag==0)
		{
			printf("ENTER N WHERE 3 <= N <= 8 (0 TO TERMINATE):");
			scanf("%d",&N);
			if(N==0)
			{
				printf("PROGRAM TERMINATED!\n");
				exit(0);
			}
			if(N>=3 && N<=8)
				N_flag=1;
		}
/* CREATING COLOR MATRIX and COPYING OF IT TO CHECK IF A LINE IS EQUAL TO ORIGINAL FORM AFTER SHIFTING */
		int **matrix=(int**)malloc(N*sizeof(int*)); 
	    for(i=0;i<N;i++) 
	        matrix[i]=(int*)malloc(N*sizeof(int));
	        
	    int **matrixCopy=(int**)malloc(N*sizeof(int*)); 
	    for(i=0;i<N;i++) 
	        matrixCopy[i]=(int*)malloc(N*sizeof(int));
/* GETTING VALUES OF COLOR MATRIX */
		printf("TYPE EXIT to TERMINATE!\n");
		for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
			{	
				true_input_flag=0;
				while(true_input_flag==0)
				{	
					include_flag=0;
					printf("ENTER MATRIX[%d][%d]:",i,j);
					scanf("%s",color);
					strcpy(color,strupr(color));
					if(!strcmp(color,"EXIT"))
					{
						printf("PROGRAM TERMINATED!");
						exit(0);
					}
					k=0;
					while(k<8 && include_flag==0)
					{
						if(!strcmp(colors[k],color))
						{
							include_flag=1;
							true_input_flag=1;
						}
						else k++;
					}
					if(k!=8)
					{
						matrix[i][j]=k;
						matrixCopy[i][j]=matrix[i][j];
					}	
				}
			}
		}
		printf("\nYOU ENTERED ");
		print_colors(matrix,N,colors);
		x=unique_columns(matrix,N,1,matrixCopy,0,colors);
		free(matrix);
		free(matrixCopy);
	}
	return 0;
}

int unique_columns(int **matrix, int N, int row, int **matrixCopy, int bot_flag, char *colors[])
{	
	/* bot_flag = 1 WHEN CURRENT ROW>=2 (in C) AND GOING BACK TO ROW ABOVE TO PREVENT ENDLESS LOOP
	matrix = NUMBER MATRIX (VALUES CHANGE)
	row = CURRENT ROW
	matrixCopy = FIRST MATRIX (VALUES DO NOT CHANGE) 
	colors = COLOR ARRAY TO PRINTING 
	N = MATRIX SIZE FROM USER */
	
	int same_flag; /*  1 WHEN A NEW ROW IS EQUAL TO SAME ROW OF ORIGINAL MATRIX AFTER SHIFTING */
	int above_flag; /* 1 WHEN A NUMBER IN CURRENT COLUMN IS EQUAL TO CURRENT VALUE AT THE MATRIX */
	int x; /* RETURNING VALUE */
	
/*-----------------------------------------------------------------------------------------------------*/
	if(row==0)
	{	/* AT FIRST ROW, WE CAN NOT COMPARE THE VALUES WITH ROW ABOVE, 
		SO WE ONLY CAN SHIFT UNTIL SUCCESSFULL TRIES OR
		GETTING SAME ROW WITH ORIGINAL MATRIX'S CURRENT ROW */
		shift_right(matrix,row,N);
		same_flag=are_rows_same(matrix,matrixCopy,0,N);
		if(same_flag==1)
		{	/* IF FIRST ROW SHIFTING N-1 TIMES, IT MEANS WE CAN NOT GO ON ANYMORE */
			printf("\nFAIL, THERE IS NO SOLUTION!\n");
			return 1;	
		}
		else/* ELSE WE CAN TRY ROWS BELOW AFTER SHIFTING */
			return(unique_columns(matrix,N,row+1,matrixCopy,0,colors));
	}
/*-----------------------------------------------------------------------------------------------------*/
	/* IF N == CURRENT ROW -> PROGRAM SUCCESSFULLY PLACED THE COLORS */ 
	else if(row==N)
	{	
		printf("*******************************\nSUCCESS! LAST STATE OF COLORS ARE BELOW\n");
		print_colors(matrix,N,colors);
		return 1;
	} 
/*-----------------------------------------------------------------------------------------------------*/	
	/* ELSE, WE CHECK ROWS ABOVE */
	else
	{
		above_flag=check_above(matrix,row,N);
		if(above_flag==1)
		{	/* IF A NUMBER IN COLUMNS ABOVE == CURRENT NUMBER -> SHIFT AND CHECK THAT ROW IF IT IS EQUAL TO FIRST STATE (INPUT STATE) */
			shift_right(matrix,row,N);
			same_flag=are_rows_same(matrix, matrixCopy,row,N);
			if(same_flag==1) /* WE SHIFTED CURRENT ROW N TIMES */
				return(unique_columns(matrix,N,row-1,matrixCopy,1,colors));
			else
			{	/* WE CAN STILL SHIFT RIGHT */
				if(bot_flag==0) /* IF FROM UP TO DOWN -> SHIFT RIGHT WITH BOT_FLAG=0 */
					return(unique_columns(matrix,N,row,matrixCopy,0,colors));
				/* IF FROM DOWN TO UP -> SHIFT RIGHT WITH BOT_FLAG=1 */
				else return(unique_columns(matrix,N,row,matrixCopy,1,colors));
			}
		}
		else
		{	/* NO CONFLICT ROWS ABOVE -> GO TO ROW BELOW */
			
			
			if(bot_flag==1)
			{	/* AIM OF BOT_FLAG IS CONTROLLING ENDLESS LOOP */
			
				shift_right(matrix,row,N);
				same_flag=are_rows_same(matrix,matrixCopy,row,N);
				if(same_flag==0) /* WE CAN STILL SHIFT RIGHT */
					return(unique_columns(matrix,N,row,matrixCopy,0,colors));
				else /* CHECK THAT ROW AFTER SHIFT IF IT IS EQUAL TO FIRST STATE (INPUT STATE) -> IF IT IS SAME, GO ROW ABOVE */
					return(unique_columns(matrix,N,row-1,matrixCopy,1,colors)); 
			}
			else 
			{	printf("*******************************\nCURRENT ROW = %d (in C SYNTAX)\n",row);
				print_colors(matrix,N,colors);
				return(unique_columns(matrix,N,row+1,matrixCopy,0,colors)); /* FROM TOP TO DOWN WITHOUT CONFLICT ABOVE */
			}
		}	
	}	
}

/* CHECKING IN A COLUMN THAT ONE OF THE NUMBERS ABOVE THE COLUMN 
IS EQUAL TO THE NUMBER IN THE CURRENT ROW IN THAT COLUMN 
RETURNS 1 WHEN A MATCH FOUND
RETURNS 0 WHEN MISMATCH
(row=CURRENT ROW, N=MATRIX SIZE) */
int check_above(int **matrix, int row, int N)
{	
	int temp;
	int j=0;
	while(j<N) /* ITERATING TO LAST COLUMN FROM FIRST COLUMN */
	{	
		temp=row-1; /* THE COLUMN ABOVE CURRENT COLUMN */
		while(temp>=0) /* GOING UP IN THE COLUMN */
		{	
		/* IF THERE IS A MATCH, RETURN 1 */
			if(matrix[row][j]==matrix[temp][j])
				return 1;
			temp--;
		}
		j++; 
	}
	return 0;
}

/* SHR MATRIX (ROW = CURRENT ROW, N = MATRIX SIZE) */
void shift_right(int **matrix, int row, int N)
{
	int i,last;
	last=matrix[row][N-1];
	for(i=N-1;i>0;i--)
		matrix[row][i]=matrix[row][i-1];
	matrix[row][0]=last;
}

/* IF THE ROWS OF 2 MATRIXES ARE EQUAL, RETURN 1, ELSE RETURN 0 
(i=CURRENT ROW, N=MATRIX SIZE)          */
int are_rows_same(int **matrix, int **matrixCopy, int i, int N)
{
	int j=0;
	while(j<N && matrix[i][j]==matrixCopy[i][j])
		j++;
	if(j==N)
		return 1;
	return 0;
}

/*PRINTING COLOR MATRIX */
void print_colors(int **matrix, int N, char *colors[])
{	
	printf("COLOR MATRIX:\n\n");
	int i,j;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
			printf("%s ",colors[matrix[i][j]]);
		printf("\n");
	}
}
