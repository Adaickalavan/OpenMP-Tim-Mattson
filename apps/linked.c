#include <stdlib.h>
#include <stdio.h>
#include "omp.h"

#define N 5
#define FS 38
#define NMAX 10

struct node
{
    int data;
    int fibdata;
    struct node *next;
};

struct node* init_list(struct node* p);
int fib(int n);
void processwork(struct node *p);
int fibTasks(int n);
void processworkTasks(struct node *p);
void serial(struct node* p);
void parallel(struct node* head);
void parallelTasks(struct node* head);

int main(int argc, char *argv[])
{
    double start, end;
    struct node *p = NULL;
    struct node *temp = NULL;
    struct node *head = NULL;

    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);

    p = init_list(p);
    head = p;

    // Slide 124 - Linked list computed serially
    serial(head);
    // Slide 128 - Linked list without Tasks
    parallel(head);
    // Slide 143 - Linked list with Tasks
    parallelTasks(head);

    p = head;
    while (p != NULL)
    {
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);

    return 0;
}

struct node *init_list(struct node *p)
{
    int i;
    struct node *head = NULL;
    struct node *temp = NULL;

    head = malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i = 0; i < N; i++)
    {
        temp = malloc(sizeof(struct node));
        p->next = temp;
        p = temp;
        p->data = FS + i + 1;
        p->fibdata = i + 1;
    }
    p->next = NULL;
    return head;
}

int fib(int n)
{
    int x, y;
    if (n < 2)
    {
        return (n);
    }
    else
    {
        x = fib(n - 1);
        y = fib(n - 2);
        return (x + y);
    }
}

void processwork(struct node *p)
{
    int n;
    n = p->data;
    p->fibdata = fib(n);
}

int fibTasks(int n)
{
    int x, y;
    if (n < 2)
    {
        return (n);
    }
    else if (n < 30){
        return fibTasks(n-1) + fibTasks(n-2);
    }
    else
    {
        #pragma omp task default(none) shared(x) firstprivate(n)
        x = fibTasks(n - 1);
        #pragma omp task default(none) shared(y) firstprivate(n)
        y = fibTasks(n - 2);
        #pragma omp taskwait
        return (x + y);
    }
}

void processworkTasks(struct node *p)
{
    int n;
    n = p->data;
    p->fibdata = fibTasks(n);
}

void serial(struct node* p){
    double start = omp_get_wtime();
    {
        while (p != NULL)
        {
            processwork(p);
            p = p->next;
        }
    }

    double end = omp_get_wtime();
    printf("Serial: %f seconds\n", end - start);
}

void parallel(struct node* head){
    int count = 0;
    struct node *parr[NMAX];

    double start = omp_get_wtime();
    {
        // count number of items in the list.  Strictly speaking this isn't
        // needed since we know there are N elements in the list.  But in
        // most cases you don't know this and need to count nodes.
        for (struct node* p = head; p != NULL; p = p->next)
        {
            count++;
        }

        // traverse the list and collect pointers into an array.
        struct node* p = head;
        for (int i = 0; i < count; i++)
        {
            parr[i] = p;
            p = p->next;
        }

        // do the work in parallel
        omp_set_num_threads(4);
        #pragma omp parallel
        {
            #pragma omp single
            printf("%d threads \n", omp_get_num_threads());
            #pragma omp for
            for (int i = 0; i < count; i++)
                processwork(parr[i]);
        }
    }
    double end = omp_get_wtime();
    printf("Parallel without tasks: %f seconds\n", end - start);
}

void parallelTasks(struct node* head){
    double start = omp_get_wtime();
    {
        #pragma omp parallel default(none) firstprivate(head)
        {
            #pragma omp single
            {
                for (struct node* p = head; p != NULL; p = p->next){
                    #pragma omp task default(none) firstprivate(p)
                    {
                        processwork(p);
                        // processworkTasks(p);
                    }
                }
            }
        }
    }
    double end = omp_get_wtime();
    printf("Parallel with tasks: %f seconds\n", end - start);
}