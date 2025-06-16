#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include "todo.h"

//struct definitions
typedef struct _task 
{
    char description[256];
    bool completed;
    struct _task *next;
} Task;

typedef struct _list 
{
    int num_tasks;
    Task * tasks;
} List;

typedef struct _listsWrapper 
{
    int num_lists;
    List * lists;
} ListsWrapper;


//dat structuree functeon shiz

ListsWrapper * init_lists_wrapper()
{
    ListsWrapper * new_list_wrapper = (ListsWrapper*)malloc(sizeof(*new_list_wrapper));
    if(new_list_wrapper)
    {
        new_list_wrapper->lists = NULL;
        new_list_wrapper->num_lists = 0;
    }
    return new_list_wrapper; 
}
List * init_list()
{
    List * new_list = (List*)malloc(sizeof(*new_list));
    if(new_list)
    {
        new_list->tasks = NULL;
        new_list->num_tasks = 0;
    }
    return new_list;
}
Task * init_task(char *text)
{
    Task * new_task = (Task*)malloc(sizeof(*new_task));
    if(new_task)
    {
        new_task->next = NULL;
        new_task->completed = false;
        strncpy(new_task->description, text, 256);
        new_task->description[255] = '\0';
    }
    return new_task;
}







void print_header()
{

}



int main()
{
    initscr(); //begin curse mode

    //declare variables
    int running = 1;
    int amt_task_list = 0;

    while(running)
    {
        print_header();
    }

    refresh();
    getch();
    endwin();


    return EXIT_SUCCESS;
}