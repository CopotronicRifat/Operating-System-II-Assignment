/*
Assignment 03
S M Rafiuddin
CWID: A20387294

Acknowledgments:

Some code segments are used from-

1. Medium Blog Posts
2. Geeks For Geeks
3. Stack Overflow
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<time.h>

int find_index(int temp[], int n, int target)
{
    int i = 0;  // initialize loop counter to 0
    while (i < n)   // iterate through array until end is reached
    {
        if (temp[i] == target)  // if target is found at current index
        {
            return i;   // return the index
        }
        i++;    // increment loop counter
    }
    return -1;  // if target is not found, return -1
}


int find_min_index(int temp[],int n)
{
    // Initialize variables to hold minimum value and its index
    int min=INT_MAX;
    int min_index=-1;
    int i=0;

    // Iterate over the array elements
    while(i<n)
    {
        // Update minimum value and its index if current element is smaller
        if(temp[i]<min)
        {
            min=temp[i];
            min_index=i;
        }
        i++;
    }

    // Return the index of the minimum value
    return min_index;
}

void page_repl(char filename[], int numOfFrames, int repititionAlgorithm)
{
    char tmp_char;
    int tmp=0,i=0,n=0,nowrites=0;
    int *a; // pointer to array holding page numbers
    FILE * filePointer;
    filePointer = fopen(filename,"r"); // open file for reading
    if(filePointer==NULL)
    {
        printf("file doesn't exist"); // error message if file doesn't exist
        exit(0); // exit program
    }
    while (fscanf(filePointer,"%x %c\n",&tmp,&tmp_char)!=EOF)
    {
        n++; // count number of lines in file
    }
    fclose(filePointer); // close file
    a=(int *) calloc(n,sizeof(int));  // allocate memory for page numbers
    i=0;
    filePointer = fopen(filename,"r"); // open file again for reading
    while (fscanf(filePointer,"%x %c\n",&tmp,&tmp_char)!=EOF)
    {
        a[i]=tmp; // store page number in array

        if(tmp_char=='W')
        {
           nowrites++; // count number of writes
        }
        i++;
    }

    int j=0,pagefault=0,pagehit=0;
    int *frames; // pointer to array holding page frames

    frames=(int *) calloc(numOfFrames,sizeof(int)); // allocate memory for page frames
    for(i=0;i<numOfFrames;i++) frames[i]=-1; // initialize all frames to -1
    if(repititionAlgorithm==0) // if using FIFO algorithm
    {
        int i=0;
        while(i<n)
        {
            if(find_index(frames,numOfFrames,a[i])==-1)  // if page is not in frame
            {
                j=j%numOfFrames;  // reset j if it exceeds number of frames
                frames[j++]=a[i]; // replace page at index j with new page
                pagefault++; // increment page fault count
            }
            else
            {
                pagehit++; // increment page hit count
            }
            i++;
        }
    }
    else // if using LRU algorithm
    {
        int *tmpFrame; // pointer to temporary frame array
        tmpFrame=(int *)calloc(numOfFrames,sizeof(int)); // allocate memory for temporary frame array
        for(i=0;i<numOfFrames;i++)
        {
            tmpFrame[i]=i-numOfFrames; // initialize temporary frame array with negative indices
        }
        int i=0;
        while(i<n)
        {
            if(find_index(frames,numOfFrames,a[i])==-1) // if page is not in frame
            {
                j=find_min_index(tmpFrame,numOfFrames); // find the index of the page that has been in the frame the longest
                frames[j]=a[i]; // replace page at index j with new page
                tmpFrame[j]=i; // update temporary frame array with index of new page
                pagefault++; // increment page fault count
            }
            else
            {
                j=find_index(frames,numOfFrames,a[i]); // find the index of the page in the frame
                tmpFrame[j]=i; // update temporary frame array with index of page
                pagehit++; // increment page hit count
            }
            i++;
        }
        free(tmpFrame); // free memory allocated for temporary frame array
        tmpFrame=NULL;
    }
    free(a); // free memory allocated for page number array
    a=NULL;
    printf("\nContents of frames at last:\n");
    for(i=0;i<numOfFrames;i++)
    {
        printf("%x ",frames[i]);
    }
    printf("\nNumber of Reads: %d",pagefault);
    printf("\nNumber of Writes: %d",nowrites);
    return;
}


int main(int argc,char* argv[])
{
    clock_t start, end;
    double cpu_time_used;

    // Start the clock to measure execution time
    start = clock();

    // Check if enough arguments are passed
    if(argc!=4)
    {
        printf("Not enough arguments\n");
        exit(0);
    }
    else
    {
        int repititionAlgorithm, frames;
        char filename[100];

        // Copy the file name to filename array
        strcpy(filename,argv[1]);

        // Convert the string argument to integer
        frames=atoi(argv[2]);

        // Check the replacement algorithm to be used
        if (strcmp(argv[3], "lru") == 0)
        {
            repititionAlgorithm = 1;
        }
        else if (strcmp(argv[3], "fifo") == 0)
        {
            repititionAlgorithm = 0;
        }

        // Call the page replacement function
        page_repl(filename, frames, repititionAlgorithm);

        // Stop the clock and calculate CPU time used
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\nCPU TIME USED: %f\n",cpu_time_used);
        return 0;
    }
}
