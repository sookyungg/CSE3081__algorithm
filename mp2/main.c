#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define _CRT_SECURE_NO_WARNINGS

double insertion_sort(int length, int arr[]);
double quicksort(int arr[], int left, int right);
double QuickSort4(int arr[],int left, int right);
double mergesort(int arr[], int left, int right);
double sort1(int length, int arr[]);
double sort2(int length, int arr[]);
double sort3(int length, int arr[]);
double sort4(int length, int arr[]);

int main(int argc, char *argv[])
{
    int length;
    FILE *fp = fopen(argv[1], "r");
    
    char result[200] = "result_";
    char s1[20] = "_";
    strcat(result, argv[2]);
    strcat(result, s1);
    strcat(result, argv[1]);
    FILE *fw = fopen(result, "w");
    
    
    fscanf(fp, "%d", &length);
    int arr[length];

    for(int i = 0; i < length; i++) fscanf(fp, "%d", &arr[i]);
    fclose(fp); //get file data

    int sort_num = argv[2][0] - '0';
    
    
    clock_t start, end;
    start = clock();
    switch(sort_num){
        case 1: 
            insertion_sort(length, arr);
        case 2: 
            quicksort(arr, 0, length-1);
        case 3: 
            mergesort(arr, 0, length-1);
        case 4: 
            QuickSort4(arr, 0, length-1);
        
    }
    end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    

    
    fprintf(fw ,"%s\n", argv[1]);
    fprintf(fw ,"%s\n", argv[2]);
    fprintf(fw,"%d\n", length);
    fprintf(fw ,"%f\n", duration);

    for(int i = 0; i < length; i++) 
        fprintf(fw, "%d ", arr[i]);
    fclose(fw); 

    return 0;
}

// double sort1(int length, int arr[]){
//     clock_t start, end;
//     start = clock();  
//     insertion_sort(length, arr);
//     end = clock();
//     double duration = (double)(end - start) / CLOCKS_PER_SEC;
//     return duration; 
// }

// double sort2(int length, int arr[]){
//     clock_t start, end;
//     start = clock();  
//     quicksort(arr, 0, length-1);
//     end = clock();
//     double duration = (double)(end - start) / CLOCKS_PER_SEC;
//     return duration; 
// }

// double sort3(int length, int arr[]){
//     clock_t start, end;
//     start = clock();  
//     mergesort(arr, 0, length-1);
//     end = clock();
//     double duration = (double)(end - start) / CLOCKS_PER_SEC;
//     return duration; 
// }

// double sort4(int length, int arr[]){
//     clock_t start, end;
//     start = clock();  
//     QuickSort4(arr, 0, length-1);
//     end = clock();
//     double duration = (double)(end - start) / CLOCKS_PER_SEC;
//     return duration; 
// }


//sort1
double insertion_sort(int length, int arr[]) //insertion sort
{

   int i, j;
    int key;
 
    for(i=1; i< length; i++){
        key = arr[i];
        
        for(j=i-1; j>=0; j--){
        
            if(arr[j] > key){       
                arr[j+1] = arr[j];  
            }else{                  
                break;
            }
        
        }
        
        arr[j+1] = key;             
    }
    
}
//sort2
int Partition(int arr[], int left, int right) 
{
    int pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; j++){
        if (arr[j] < pivot + 1)
        {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i+1];
    arr[i+1] = arr[right];
    arr[right] = temp;

    return i+1;
}

double quicksort(int arr[], int left, int right) 
{
    
    if (left < right)
    {
        int pivot = Partition(arr, left, right);
        quicksort(arr, left, pivot - 1);
        quicksort(arr, pivot + 1, right);
    }

   
}

//sort 3
void merge(int arr[], int left, int mid, int right) 
{
    int i = left, j = mid + 1, k = left;
    
    int* temp;
    temp = (int*)malloc(sizeof(int)*(right+1));

    while (i < mid+1 && j < right+1)
    {
        if (arr[i] <= arr[j]) 
            temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
   
    while (i < mid+1) 
        temp[k++] = arr[i++];


    while (j < right+1) 
        temp[k++] = arr[j++];


    int a = left;
    while( a < right + 1){
        arr[a] = temp[a];
        a++;
    }


    free(temp);
}

double mergesort(int arr[], int left, int right) //for sort3
{
    
    int mid;
    if (left < right)
    {
        mid = (left + right)/2;
        mergesort(arr, left, mid);
        mergesort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
    
}



//sort4
double QuickSort4(int arr[],int left, int right)
{
   
    if(left<=right)
    {
        int pivot = Partition2(arr, left, right);
        QuickSort4(arr, left, pivot-1);
        QuickSort4(arr, pivot+1, right);
    }
  
}

void Swap(int arr[],int idx1, int idx2)
{
    int temp = arr[idx1];
    arr[idx1] = arr[idx2];
    arr[idx2] = temp;
}
int MedianOfThree(int arr[],int left,int right)
{
    int samples[3] = {left,(left+right)/2,right}; //values is index
    if(arr[samples[0]]>arr[samples[1]])
           Swap(samples,0,1);
    
    if(arr[samples[1]] > arr[samples[2]])
        Swap(samples, 1, 2);
    if(arr[samples[0]]>arr[samples[1]])
        Swap(samples, 0, 1);
        
    return samples[1];
}
int Partition2(int arr[],int left, int right)
{
    int mid_pivot = MedianOfThree(arr, left, right); 
    int pivot = arr[mid_pivot];
    
    int low = left+1;
    int high = right;
    
    Swap(arr, left, mid_pivot); 
    
   
    while(low <= high)
    {
        
        while(pivot >= arr[low] && low<= right)
            low++; 
        
        while(pivot<=arr[high] && high >= (left+1))
            high --;
        
        if(low<=high)
            Swap(arr,low,high);
    }
    Swap(arr,left,high); 
    return high; 
}

