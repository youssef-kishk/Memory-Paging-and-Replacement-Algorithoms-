#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct input{
    int *inputValues;
    int pageSize;
    char simulatedAlgorithmType[30];
    int numOFPageRefrences;
    int *faults;
}inputRefrence;
struct output{
    int **contents;
    char *replacmentPolicy;
    int numOfPageFaults;
    int numOFPageRefrences;
}outputRefrence;

void clockALgorithm();
int searchValueClock(int *,int);
void getValuesToOutputClock(int *,int *);

void FIFOALgorithm();

void LRUALgorithm();

int searchValue(int *,int);
int getMinTime(int *);
void getValuesToOutput(int *,int *);

typedef struct Node{
    int data;
    struct Node *Next;
}Node;
typedef struct{
    Node *first,*last;
    int length;
}Queue;

Node* newNode(int);
void initialize(Queue *);
void enqueue(Queue *,int );
int dequeue (Queue *);
int top(Node*);
int contains(Queue *,int );
int isEmpty(Queue *);
int i;
void clockALgorithm(){
    int i;
    int index=0;
    int count=0;
    int pointer=0;
    int *valuesArray;
    int *clockArray;
    valuesArray=(int*) malloc(outputRefrence.numOFPageRefrences * sizeof(int));
    clockArray=(int*) malloc(outputRefrence.numOFPageRefrences * sizeof(int));
    for(i=0;i<outputRefrence.numOFPageRefrences ;i++){
        valuesArray[i]=-1;
        clockArray[i]=0;
    }
    while(inputRefrence.inputValues[count]!=-1){
            if(inputRefrence.pageSize!=index){
                //not found in page and page are not full
                if(searchValueClock(valuesArray,inputRefrence.inputValues[count])==-1){
                    valuesArray[index]=inputRefrence.inputValues[count];
                    clockArray[index]=1;
                        index++;
                        pointer++;
                        count++;
                    }
                else
                    count++;
                }
             else{

                 //found in page not a new value
                 if(searchValueClock(valuesArray,inputRefrence.inputValues[count])!=-1){
                        pointer = searchValueClock(valuesArray,inputRefrence.inputValues[count]);
                        clockArray[pointer]=1;
                        count++;
                 }
                //new value and page is full
                else{
                    int check=0;
                    int loc;
                    loc=pointer;
                    do{
                        if(pointer==index)
                            pointer=0;
                        if(clockArray[pointer]==0){
                             valuesArray[pointer]=inputRefrence.inputValues[count];
                             clockArray[pointer]=1;
                            pointer++;
                            check=1;
                            break;
                        }
                        else
                            clockArray[pointer]=0;
                        pointer++;
                    }while(pointer!=loc);
                        if(pointer==index)
                            pointer=0;
                        if(check==0){
                            valuesArray[pointer]=inputRefrence.inputValues[count];
                            clockArray[pointer]=1;
                            pointer++;
                        }
                        check=0;
                        inputRefrence.faults[count]=1;
                        outputRefrence.numOfPageFaults++;
                        count++;
                }
        }
        getValuesToOutputClock(valuesArray,outputRefrence.contents[count-1]);
    }
}
int searchValueClock(int *arr,int val){
    int i;
    for(i=0;i<outputRefrence.numOFPageRefrences;i++)
        if(arr[i]==val)
            return i;
    return -1;
}

void getValuesToOutputClock(int *valuesArray,int *arr){
    int i=0;
    while(valuesArray[i]!=-1){
        arr[i]=valuesArray[i];
        i++;
    }
     arr[i]=-1;
}
Queue q;
void FIFOALgorithm(){
    initialize(&q);
    int tempArray[outputRefrence.numOFPageRefrences ];
    int i=0;
    for(i=0;i<outputRefrence.numOFPageRefrences  ;i++)
        tempArray[i]=-1;
    int tempArrayCount=0;
    int index=0;
    int count=0;
    while(inputRefrence.inputValues[count]!=-1){
        //if queue size not equal page size
        if(inputRefrence.pageSize!=index){
            if(contains(&q,inputRefrence.inputValues[count])==0){
                enqueue(&q,inputRefrence.inputValues[count]);
                tempArray[tempArrayCount]=inputRefrence.inputValues[count];
                tempArrayCount++;
                index++;
                count++;
            }
            else
                count++;
        }
        //page is filled
        else{
            if(contains(&q,inputRefrence.inputValues[count])==1)
                    count++;
            else{
                    int dequedValue = dequeue(&q);
                    for(i=0;i<inputRefrence.pageSize;i++)
                        if(tempArray[i]==dequedValue)
                            tempArray[i]=inputRefrence.inputValues[count];
                    enqueue(&q,inputRefrence.inputValues[count]);
                    inputRefrence.faults[count]=1;
                    outputRefrence.numOfPageFaults++;
                    count++;
                }
        }
        i=0;
        while(tempArray[i]!=-1){
            outputRefrence.contents[count-1][i]= tempArray[i];
            i++;
        }
         outputRefrence.contents[count-1][i]=-1;
    }
}

void LRUALgorithm(){
    int i;
    int index=0;
    int count=0;
    int *valuesArray;
    int *timeArray;
    int timeval=1;
    valuesArray=(int*) malloc(outputRefrence.numOFPageRefrences * sizeof(int));
    timeArray=(int*) malloc(outputRefrence.numOFPageRefrences * sizeof(int));
    for(i=0;i<outputRefrence.numOFPageRefrences ;i++){
        valuesArray[i]=-1;
        timeArray[i]=-1;
    }
    while(inputRefrence.inputValues[count]!=-1){
         if(inputRefrence.pageSize!=index){
                if(searchValue(valuesArray,inputRefrence.inputValues[count])==-1){
                    valuesArray[index]=inputRefrence.inputValues[count];
                    timeArray[index]=timeval;
                        timeval+=10;
                        index++;
                        count++;
                }
                else{
                    timeArray[searchValue(valuesArray,inputRefrence.inputValues[count])]=timeval;
                    timeval+=10;
                    count++;
                }
        }
        else{
            if(searchValue(valuesArray,inputRefrence.inputValues[count])!=-1){
                    timeArray[searchValue(valuesArray,inputRefrence.inputValues[count])]=timeval;
                    timeval+=10;
                    count++;
            }
            else{
                 int minTimeElement=getMinTime(timeArray);
                 valuesArray[minTimeElement]=inputRefrence.inputValues[count];
                 timeArray[minTimeElement]=timeval;
                 timeval+=10;
                 inputRefrence.faults[count]=1;
                  outputRefrence.numOfPageFaults++;
                  count++;
            }
        }
            getValuesToOutput(valuesArray,outputRefrence.contents[count-1]);

    }
}
int searchValue(int *arr,int val){
    int i;
    for(i=0;i<outputRefrence.numOFPageRefrences;i++)
        if(arr[i]==val)
            return i;
    return -1;
}
int getMinTime(int *arr){
    int min=0;
    int i=0;
    while(arr[i]!=-1){
        if(arr[i]<arr[min])
            min=i;
        i++;
    }
    return min;
}
void getValuesToOutput(int *valuesArray,int *arr){
    int i=0;
    while(valuesArray[i]!=-1){
        arr[i]=valuesArray[i];
        i++;
    }
     arr[i]=-1;
}
void readInput(){
    scanf("%d",&inputRefrence.pageSize);
    scanf("%s",inputRefrence.simulatedAlgorithmType);
    inputRefrence.inputValues=(int*) malloc(100 * sizeof(int));
    //initialze pages faults array
    inputRefrence.faults=(int*) malloc(100 * sizeof(int));
    int x;
    int count=0;
    while(1){
         scanf("%d",&x);
         inputRefrence.inputValues[count]=x;
         count++;
         inputRefrence.numOFPageRefrences=count-1;
         if(x==-1)
            break;
    }
}
void printOutput(){
    int i,j;
    printf("Replacement Policy = %s\n",outputRefrence.replacmentPolicy);
    printf("-------------------------------------\n");
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    for(i=0;i<outputRefrence.numOFPageRefrences;i++){
            j=0;
        printf("%02d",inputRefrence.inputValues[i]);
        if(inputRefrence.faults[i]==1)
            printf(" F   ");
        else
            printf("     ");
        while(outputRefrence.contents[i][j]!=-1){
            printf("%02d ",outputRefrence.contents[i][j]);
            j++;
        }
        printf("\n");
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",outputRefrence.numOfPageFaults);
}
int main()
{
    int i;
    //read input
   readInput();
   //initialze output values
   outputRefrence.numOFPageRefrences=inputRefrence.numOFPageRefrences;
   outputRefrence.replacmentPolicy=inputRefrence.simulatedAlgorithmType;
   outputRefrence.contents=(int **)malloc(outputRefrence.numOFPageRefrences* sizeof(int *));
    for (i=0; i<outputRefrence.numOFPageRefrences ; i++)
        outputRefrence.contents[i] = (int *)malloc(100 * sizeof(int));
     //run certain algorithom according to input
    if(strcmp(inputRefrence.simulatedAlgorithmType,"FIFO")==0)
         FIFOALgorithm();
    else if(strcmp(inputRefrence.simulatedAlgorithmType,"LRU")==0)
         LRUALgorithm();
    else if(strcmp(inputRefrence.simulatedAlgorithmType,"CLOCK")==0)
         clockALgorithm();
    //print output
   printOutput();
    return 0;
}

Node* newNode(int value){
    Node* n = (Node*)malloc(sizeof(Node));
    n->data=value;
    n->Next= NULL;
    return n;
}
void initialize(Queue *s){
    s->first=NULL;
    s->last=NULL;
    s->length=0;
}
void enqueue(Queue *s,int value){
    Node *n = newNode(value);
    if(isEmpty(s)){
        s->first=n;
        s->last=n;
    }
    else{
        s->last->Next=n;
        s->last=n;
    }
    s->length++;
}
int dequeue (Queue *s){
    if(!isEmpty(s)){
        int x = s->first->data;
        Node *n = s->first;
        s->first=s->first->Next;
        free (n);
        if(isEmpty(s))
            s->last = s->first=NULL;
            s->length--;
        return x;
    }
    else{
        exit(1);
    }
}
int top(Node*s){
    return (s->data);
}
int contains(Queue *s,int val){
    Node *temp = s->first;
    int i;
    while(temp!=NULL){
        if(top(temp)==val)
            return 1;
        else
           temp=temp->Next;
    }
    return 0;
}
int isEmpty(Queue *s){
    return s->first == NULL? 1 :0;
}
