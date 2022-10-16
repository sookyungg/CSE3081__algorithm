#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MIN -999999999

int kadane(int _arr[], int n);
int max(int x, int y);
int allzero(int row, int col, int **_arr);

int main(int argc, char** argv) {
    int index = atoi(argv[2]);
    int row, col;
    double start, end, time;
    int max_sum, sum_tmp, sum_tmp2;
    int tmp;
    

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("input File open error\n");
        exit(1);
    }
    
    char output_text[100] = "result_";
    strcat(output_text, argv[1]);
    
    FILE *fw = fopen(output_text, "w");
    if (fw == NULL) {
        printf("output FILE open error\n");
        exit(1);
    }


    
	if (argc != 3)
	{
		printf("usage: ./mp1_20190340 filename number\n");
		exit(1);
	}

	
    
    fscanf(fp, "%d %d", &row, &col);


    int **arr;
    arr= (int**)malloc(sizeof(int*)*row);
    for (int i = 0; i< row; i++) {
        arr[i] = (int*)malloc(sizeof(int) * col);
    }

    
    for (int i = 0; i<row; i++) {
        for (int j  =0; j<col; j++) {
            fscanf(fp, "%d", &arr[i][j]);
        }
    }

    switch (index){
            case 1: 
                start = clock();
                sum_tmp = 0;
                max_sum = MIN;
                for (int i = 0; i < row; i++) {
                    for (int j = 0; j < col; j++) {
                        for (int k = i; k < row; k++) {
                            for (int l = j; l < col; l++) {
                                sum_tmp=0;
                                for (int p = i; p <= k; p++) {
                                    for (int q = j; q <= l; q++) {
                                        sum_tmp += arr[p][q];
                                    }
                                }
                               max_sum=max(max_sum, sum_tmp);
                            }
                        }
                    }
                }
              
                break;
                

            case 2:
                start = clock();
                sum_tmp = 0;
                sum_tmp2 = 0;
                max_sum = 0;
                int **case2_arr;
                case2_arr = (int**)malloc(sizeof(int*) * (row+1));
                
                for (int i = 0; i < row+1; i++) {
                    case2_arr[i] = (int*)malloc(sizeof(int) * (col+1));
                }

                
                for (int j = 0; j< col; j++) {
                    for (int i = 0; i < row; i++) {
                        sum_tmp = 0;
                        for (int l = 0; l <= j; l++) {
                            for (int k = 0; k <= i; k++) {
                                sum_tmp = sum_tmp + arr[k][l];  
                            }
                        }
                        case2_arr[i+1][j+1] = sum_tmp;
                    }
   
                    sum_tmp2 = 0;

                    for (int i = 0; i < row; i++) {
                        for (int l = 1; l <= j; l++) {
                            for (int k = 1; k <= i; k++) {
                                    sum_tmp2 = case2_arr[i+1][j+1];
                                    sum_tmp2 -= case2_arr[i+1][l-1];
                                    sum_tmp2 += case2_arr[k-1][l-1];
                                    sum_tmp2 -= case2_arr[k-1][j+1];
                                    
                                    max_sum=max(max_sum, sum_tmp2);
                            }
                        }
                    }
                }

                
                break;

            
           
            case 3:
                start = clock();
                max_sum = MIN;
                int location = 0;
                int *case3_arr;
                case3_arr = (int*)malloc(sizeof(int) * row);

                int l, r;
                for (l = 0; l < col; l++) {
                    for (r = l; r < col; r++) {
                        
                        for (int i = 0; i < row; i++) {
                            case3_arr[i] += arr[i][r];
                        }

                        tmp = kadane(case3_arr, row);
                        max_sum = max(tmp, max_sum);
                        }

                    for (int i = 0; i < row; i++) {
                        case3_arr[i] = 0;
                    }
                }
               
                
                break;
                
            
    }
    end = clock();
    time = end - start;
    fprintf(fw, "%s\n", argv[1]);
    fprintf(fw, "%d\n", index);
    fprintf(fw, "%d\n", row);
    fprintf(fw, "%d\n", col);
    fprintf(fw, "%d\n", max_sum);
    fprintf(fw, "%lf\n", time);

    fclose(fp);
    fclose(fw);

    return 0;
}
int kadane(int _arr[], int n) {
    int tmp = 0;
    int max_sum = MIN;
    
    for (int i = 0; i < n; i++) {
        tmp += _arr[i];
        if (tmp < 0) {
            tmp = 0;

        }
        else if (tmp > max_sum) {
            max_sum = tmp;
        }
    }

    return max_sum;
}
int max(int x, int y){
    if (x>=y){
        return x;
    }
    if (x<y){
        return y;
    }
}
//초기화
int allzero(int row, int col, int **_arr){
    for (int i = 0; i < row+1; i++)
            _arr[i][0] = 0;
    for (int i = 0; i < col+1; i++)
            _arr[0][i] = 0;
}