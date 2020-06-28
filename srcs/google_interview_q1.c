#include <stdio.h> /* For printf() */
#include <stdlib.h> /* For dynamic memory allocation func() */

#define DEBUG 1 /* 1 For Verbose Output, 0 For to the point Output */

/***************** User Definable Region *****************/
#define MEETING_INTERVAL 30 

const char *meeting_time_person1[] = {
    "09:00","10:30",
    "12:00","13:00",
    "16:00","18:00",
};

const char *meeting_time_person2[] = {
    "10:00","11:30",
    "12:30","14:30",
    "14:30","15:00",
    "16:00","17:00",
};

const char *inbound_person1[] = {
    "09:00","20:00",
};

const char *inbound_person2[] = {
    "10:00","18:30",
};
/*********************************************************/

/******* Structures for Person and Time Data Types *******/
typedef struct person {
    const char **meeting_ptr;
    int meeting_len;
    const char **inbound_ptr;
    int inbound_len;
    char **freetime_ptr;
    int freetime_len;
} person;

typedef struct time {
    int hour;
    int minute;
} time;
/*********************************************************/

/****************** Function Prototypes ******************/
person initperson(const char *[],int,const char *[],int);
void intersection_meeting_time(person*,person*);
void cal_free_time(person *); 
time conv_time(char *);
void verify_add_elm_free_time(time,time,person*);
void calloc_new_elm(time,person*);
void realloc_new_elm(time,person*);
/*********************************************************/

/******************* Macro definations *******************/
#define PARAMINIT(x,y) initperson(x,LEN(x),y,LEN(y)); 
#define LEN(x) sizeof(x)/sizeof(x[0])
#define CONVTIME(x) conv_time((char *)x);
/*********************************************************/

/****************** Client Side Function *****************/
/* Function Name: main
 * Input Type   : void
 * Purpose      : Driver function to find when two person can meet
 * Output Type  : List of meeting schedules b/w two person
 * Return Type  : int (for success exit code)
*/
int main(void)
{
    // Declare two uninitialiased persons structure
    person p1, p2;

    printf("Person 1 ");
    // Initialize the person structure members with the user defined values
    p1 = PARAMINIT(meeting_time_person1,inbound_person1)
    // Calculate the free time of person 1 and initialise it's free_time_ptr and free_time_len members
    cal_free_time(&p1);

    printf("Person 2 ");
    // Initialize the person structure members with the user defined values
    p2 = PARAMINIT(meeting_time_person2,inbound_person2)
    // Calculate the free time of person 2 and initialise it's free_time_ptr and free_time_len members
    cal_free_time(&p2);
    
    printf("Both Persons Intersection ");
    // Find the time intervals when both the persons are free and list the intervals
    intersection_meeting_time(&p1,&p2);

    return 0;
}
/*********************************************************/

/********************* API Functions *********************/
/* Function Name: initperson
 * Input Type   : Pointers to Meeting Schedule and Inbound Time 
 *                of a Person with their respective length
 * Purpose      : Initialises person struct from the user defined values
 * Output Type  : Binds user defined string arrays with the approriate pointers.
 * Return Type  : person struct
*/
person initperson(const char *pm[], int npm, const char *pib[], int npib)
{
    person p;

    p.meeting_ptr = pm;
    p.meeting_len = npm;
    p.inbound_ptr = pib;
    p.inbound_len = npib;
    p.freetime_ptr = NULL;
    p.freetime_len = 0;

#if DEBUG
    if(p.meeting_ptr != NULL){
        printf("\n\t");
        printf("Meeting Time Array: {");
        for(int i=0;i<p.meeting_len;i++){
            if(i%2==0)
                printf("{");
            printf("\"%s\"",p.meeting_ptr[i]);
            if(i%2!=0)
                printf("}");
            printf(", ");
        }
        printf("\b \b\b \b}\n");

        printf("\n\t");
        printf("Inbound Time Array: {");
        for(int i=0;i<p.inbound_len;i++){
            if(i%2==0)
                printf("{");
            printf("\"%s\"",p.inbound_ptr[i]);
            if(i%2!=0)
                printf("}");
            printf(", ");
        }
        printf("\b \b\b \b}\n");
    }
#endif

    return p;
}

/* Function Name: cal_free_time
 * Input Type   : person structure
 * Purpose      : Calculates free time of a person from 
 *                meeting schedule and inbound time
 * Output Type  : List of free time of that person
 *                and assigns it approriate pointer for reference
 * Return Type  : void
*/
void cal_free_time(person *p)
{
    time t1,t2;

    t1 = CONVTIME(p->inbound_ptr[0])
    t2 = CONVTIME(p->meeting_ptr[0])
   
    verify_add_elm_free_time(t1,t2,p);

    for(int i=0;i<p->meeting_len/2-1;i++){
        t1 = CONVTIME(p->meeting_ptr[2*i+1])
        t2 = CONVTIME(p->meeting_ptr[2*i+2])

        verify_add_elm_free_time(t1,t2,p);
    }

    t1 = CONVTIME(p->meeting_ptr[p->meeting_len-1])
    t2 = CONVTIME(p->inbound_ptr[1])

    verify_add_elm_free_time(t1,t2,p);
#if DEBUG
    printf("\n\t");
#endif
    printf("Free Time Heap: {");
    for(int i=0;i<p->freetime_len;i++){
        if(i%2==0)
            printf("{");
        printf("\"%s\"",p->freetime_ptr[i]);
        if(i%2!=0)
            printf("}");
        printf(", ");
    }
    printf("\b \b\b \b}\n\n");
}

/* Function Name: intersection_meeting_time
 * Input Type   : Two person structures
 * Purpose      : Finds when two persons can meet based upon their freetime
 * Output Type  : List of meeting time b/w two person and assigns  
 *                them to another third person structure for reference
 * Return Type  : void
*/
void intersection_meeting_time(person *p1, person *p2)
{   
    time t1,t2,tu,td;
    person r;
    r = initperson(NULL,0,NULL,0);
    int count = (p1->freetime_len <= p2->freetime_len) ? p1->freetime_len/2 : p2->freetime_len/2;

    for(int i=0;i<count;i++){
        t1 = CONVTIME(p1->freetime_ptr[2*i])
        t2 = CONVTIME(p2->freetime_ptr[2*i])
        if((t1.hour*60+t1.minute) <= (t2.hour*60+t2.minute)){
            tu = t2;
            t1 = CONVTIME(p1->freetime_ptr[2*i+1])
            t2 = CONVTIME(p2->freetime_ptr[2*i+1])

            if((t1.hour*60+t1.minute) <= (t2.hour*60+t2.minute)){
                td = t1;
            }
            else{
                td = t2;
            }

            verify_add_elm_free_time(tu,td,&r); 
        }
        else{
            tu = t1;
            t1 = CONVTIME(p1->freetime_ptr[2*i+1])
            t2 = CONVTIME(p2->freetime_ptr[2*i+1])

            if((t1.hour*60+t1.minute) <= (t2.hour*60+t2.minute)){
                td = t1;
            }
            else{
                td = t2;
            }

            verify_add_elm_free_time(tu,td,&r);

        }
    }
#if DEBUG
    printf("\n\t");
#endif
    printf("Free Time Heap: {");
    for(int i=0;i<r.freetime_len;i++){
        if(i%2==0)
            printf("{");
        printf("\"%s\"",r.freetime_ptr[i]);
        if(i%2!=0)
            printf("}");
        printf(", ");
    }
    printf("\b \b\b \b}\n\n");

}
/*********************************************************/

/******************** Helper Functions *******************/
/* Function Name: conv_time
 * Input Type   : String pointer
 * Purpose      : Converts a String to Numerical Time
 * Output Type  : Stores the Numerical time in time structure
 *                and returns the structure
 * Return Type  : time structure
*/
time conv_time(char *end)
{
    time t;

    t.hour = (int)strtol(end,&end,10);
    t.minute = (int)strtol(end+1,&end,10);

    return t;
}

/* Function Name: verify_add_elm_free_time
 * Input Type   : Two time structures and one person pointer structure
 * Purpose      : Verifies if two times have difference of at least MEETING_INTERVAL,
                  if it is then adds those time intervals to the 
                  free_time heap string array of a person
 * Output Type  : Verify and then add element to free_time heap
 * Return Type  : void
*/
void verify_add_elm_free_time(time t1,time t2, person *p)
{
    if((t2.hour*60+t2.minute)-(t1.hour*60+t1.minute)>=MEETING_INTERVAL){
#if DEBUG
        printf("\n\tNew Element\n");
#endif
        if(p->freetime_ptr == NULL){
            calloc_new_elm(t1,p);
            realloc_new_elm(t2,p);
        }
        else{
            realloc_new_elm(t1,p);
            realloc_new_elm(t2,p);
        }
    }
}

/* Function Name: calloc_new_elm
 * Input Type   : time struture and person pointer stucture
 * Purpose      : Allocates the NULL Pointer heap of pointers by sizeof(char *)
 *                and allocates 6*sizeof(char) to the newly allocated pointer.
 * Output Type  : Stores the new time element in the heap array of strings.
 * Return Type  : void
*/
void calloc_new_elm(time t, person *p)
{
#if DEBUG
    printf("\t\tAllocated:   ");
#endif
    p->freetime_ptr = (char **)calloc(++p->freetime_len,sizeof(char *));
    p->freetime_ptr[p->freetime_len-1] = (char *)calloc(6,sizeof(char));
    sprintf(p->freetime_ptr[p->freetime_len-1],"%02d:%02d",t.hour,t.minute);
#if DEBUG
    printf("%s\n",p->freetime_ptr[p->freetime_len-1]);
#endif
}

/* Function Name: realloc_new_elm
 * Input Type   : time struture and person pointer stucture
 * Purpose      : Reallocates the existing heap of pointers by sizeof(char *)
 *                and allocates 6*sizeof(char) to the newly allocated pointer.
 * Output Type  : Stores the new time element in the heap array of strings
 * Return Type  : void
*/
void realloc_new_elm(time t,person *p)
{
#if DEBUG
    printf("\t\tReAllocated: ");
#endif
    p->freetime_ptr = (char **)realloc(p->freetime_ptr,++p->freetime_len*sizeof(char *));
    p->freetime_ptr[p->freetime_len-1] = (char *)calloc(6,sizeof(char));
    sprintf(p->freetime_ptr[p->freetime_len-1],"%02d:%02d",t.hour,t.minute);
#if DEBUG
    printf("%s\n",p->freetime_ptr[p->freetime_len-1]);
#endif
}
/*********************************************************/
