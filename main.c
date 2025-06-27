#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ncursesw/ncurses.h>  // FOR WINDOWS DEV
#include <ncurses.h>             // FOR LINUX DEV
#include "todo.h"


#define INPUT_LEN 16


/*
Tasks needing to be done/reminders
    * 

*/


//struct definitions
typedef struct _listsWrapper {
    int num_lists;
    struct _list * head;
} ListsWrapper;

typedef struct _list {
    char title[33];
    int num_tasks;
    struct _list * next_list;
    struct _task * task;
} List;

typedef struct _task {
    char description[256];
    bool complete;
    struct _task * next_task;
} Task;



//conversion function
int char_to_int()
{
    char number_str[16];
    getnstr(number_str, sizeof(number_str));
    int number_int;

    if(sscanf(number_str, "%d", &number_int) == 1)
    {
        return number_int;
    }
    return -1;
}



//dat structuree functeon shz
ListsWrapper * init_lists_wrapper()
{
    ListsWrapper * new_lists_wrapper = (ListsWrapper*)malloc(sizeof(*new_lists_wrapper));
    if(new_lists_wrapper)
    {
        new_lists_wrapper->head = NULL;
        new_lists_wrapper->num_lists = 0;
    }
    return new_lists_wrapper;
}
List * init_list()
{
    List * new_list = (List*)malloc(sizeof(*new_list));
    if(new_list)
    {
        new_list->title[0] = '\0';
        new_list->num_tasks = 0;
        new_list->task = NULL;
        new_list->next_list = NULL;
    }
    return new_list;
}
Task * init_task()
{
    Task * new_task = (Task*)malloc(sizeof(*new_task));
    if(new_task)
    {
        new_task->description[0] = '\0';
        new_task->complete = false;
        new_task->next_task = NULL;
    }
    return new_task;
}

List * create_list(const char * input_title)
{
    if(input_title)
    {
        List * new_list = init_list();
        if(new_list)
        {
            snprintf(new_list->title, sizeof(new_list->title), "%s", input_title);
            return new_list;
        }
    }
    return NULL;
}

int add_list(ListsWrapper * lists_wrapper, List * new_list)
{
    if(lists_wrapper && new_list)
    {
        if(lists_wrapper->num_lists) //if there are pre-existing lists!
        {
            List * cur = lists_wrapper->head;
            while(cur->next_list)
            {
                cur = cur->next_list;
            }
            cur->next_list = new_list;
        }
        else //if there are no pre-existing lists!
        {
            lists_wrapper->head = new_list;
        }
        lists_wrapper->num_lists++;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int delete_list(ListsWrapper * lists_wrapper, int list_number, List ** selected_list)
{
    if(lists_wrapper && lists_wrapper->num_lists > 0 && list_number <= lists_wrapper->num_lists)
    {
        List * cur = lists_wrapper->head;
        if(list_number == 1)
        {
            lists_wrapper->head = cur->next_list;
            if(cur == *selected_list)
            {
                *selected_list = NULL;
            }
            free(cur);
        }
        else
        {
            for(int i = 1; i < list_number-1; i++)
            {
                cur = cur->next_list;
            }
            List * tmp = cur->next_list;
            cur->next_list = cur->next_list->next_list;
            if(tmp == *selected_list)
            {
                *selected_list = NULL;
            }
            free(tmp);
        }
        lists_wrapper->num_lists--;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

Task * create_task(const char * input_description)
{
    if(input_description)
    {
        Task * new_task = init_task();
        if(new_task)
        {
            snprintf(new_task->description, sizeof(new_task->description), "%s", input_description);
            return new_task;
        }
    }
    return NULL;
}

int add_task(List * selected_list, Task * new_task)
{
    if(selected_list && new_task)
    {
        if(selected_list->num_tasks) //if there are pre-existing tasks!
        {
            Task * cur = selected_list->task;
            while(cur->next_task)
            {
                cur = cur->next_task;
            }
            cur->next_task = new_task;
        }
        else //if there are no pre-existing tasks!
        {
            selected_list->task = new_task;
        }
        selected_list->num_tasks++;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int delete_task(List * selected_list, int task_number)
{
    if(selected_list && task_number <= selected_list->num_tasks && task_number > 0)
    {
        Task * cur = selected_list->task;
        if(task_number == 1)
        {
            selected_list->task = cur->next_task;
            free(cur);
        }
        else
        {
            for(int i = 1; i < task_number-1; i++)
            {
                cur = cur->next_task;
            }
            Task * tmp = cur->next_task;
            cur->next_task = cur->next_task->next_task;
            free(tmp);
        }
        selected_list->num_tasks--;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

//accessor functions

char * get_list_title(List * list)
{
    if(list)
    {
        return list->title;
    }
}

Task * get_task(List * list, int task_number)
{
    if(list && list->num_tasks > 0)
    {
        Task * cur = list->task;
        for(int i = 1; i != task_number; i++)
        {
            cur = cur->next_task;
        }
        return cur;
    }
    return NULL;
}






void print_list_titles(ListsWrapper * lists_wrapper)
{
    if(lists_wrapper && lists_wrapper->num_lists > 0)
    {
        List * cur = lists_wrapper->head;
        printw("[All Lists]\n\n");
        for(int i = 1; cur; i++)
        {
            printw("[%d]. %s\n", i, cur->title);
            cur = cur->next_list;
        }
        //printw("\n");
        refresh();
    }
}






//command functions

int prompt_add_task_helper(List * selected_list)
{
    if(selected_list)
    {
        printw("\n");
        printw("Give task a description: ");
        
        char input_description[256];
        getstr(input_description);
        
        Task * new_task = create_task(input_description);
        if(new_task)
        {
            add_task(selected_list, new_task);
            return EXIT_SUCCESS;
        }
    }
    printw("\nTry selecting a list first.\n");
    refresh();
    getch();
    return EXIT_FAILURE;
}

int prompt_delete_task_helper(List * selected_list)
{
    if(selected_list && selected_list->num_tasks > 0)
    {
        printw("Enter number of task to remove: ");
        int task_number = char_to_int();
        refresh();
        if(task_number > 0 && task_number <= selected_list->num_tasks)
        {
            delete_task(selected_list, task_number);
            return EXIT_SUCCESS;
        }
    }
    printw("Try selecting a valid list with tasks!");
    refresh();
    getch();
    return EXIT_FAILURE;
}


int prompt_create_list_helper(ListsWrapper * lists_wrapper, List ** selected_list)
{
    if(lists_wrapper)
    {
        printw("\n");
        printw("Give your new list a name: ");

        char input_list_title[33];
        getstr(input_list_title);

        List * new_list = create_list(input_list_title);
        if(new_list)
        {
            add_list(lists_wrapper, new_list);
            (*selected_list) = new_list;
            return EXIT_SUCCESS;
        }
        else
        {
            printw("Creating list failed! Not sure why though haha loser.\n");
            return EXIT_FAILURE;
        }
    }
}

int prompt_delete_list_helper(ListsWrapper * lists_wrapper, List ** selected_list)
{

    if(lists_wrapper)
    {
        erase();
        print_list_titles(lists_wrapper);
        printw("Enter the number of the list to delete: ");
        int list_number = char_to_int();
        refresh();

        delete_list(lists_wrapper, list_number, selected_list);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

void prompt_complete_toggle_helper(Task * task)
{
    if(task)
    {
        task->complete = true;
    }
    else
    {
        printw("\nThe selected task is invalid.\n");
        refresh();
        getch();
    }
}

int prompt_complete_toggle(List * selected_list)
{
    if(selected_list)
    {
        printw("Enter number of task to toggle completion: ");
        refresh();
        int task_number = char_to_int();

        if(task_number <= selected_list->num_tasks)
        {
            prompt_complete_toggle_helper(get_task(selected_list, task_number));
        }
        else
        {
            printw("\nSelected task is invalid!\n");
            refresh();
            getch();
        }
    }
    else
    {
        printw("\nThere is currently no list selected!\nSwitch to an existing list or create a new one!\n");
        refresh();
        getch();
    }
    return EXIT_SUCCESS;
}




int prompt_switch_list_helper(ListsWrapper * lists_wrapper, List ** selected_list)
{
    if(lists_wrapper && lists_wrapper->num_lists > 0)
    {
        erase();
        //print_header(selected_list);
        //print_list(selected_list);
        print_list_titles(lists_wrapper);

        printw("Enter the number corresponding to the list you want to switch to: ");
        int input_list_number = char_to_int();

        if(input_list_number <= lists_wrapper->num_lists && input_list_number > 0)
        {
            List * cur = lists_wrapper->head;
            for(int i = 1; i != input_list_number; i++)
            {
                cur = cur->next_list;
            }
            *selected_list = cur;
            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}


void quit(int * running)
{
    *running = 0;
}






int command_handler(const char * input_command, ListsWrapper * lists_wrapper, List ** selected_list, int * running)
{
    if(input_command && lists_wrapper)
    {
        if(!strcmp(input_command, "createlist"))
        {
            prompt_create_list_helper(lists_wrapper, selected_list);
        }
        else if(!strcmp(input_command, "deletelist"))
        {
            prompt_delete_list_helper(lists_wrapper, selected_list);
        }
        else if(!strcmp(input_command, "switchlist"))
        {
            prompt_switch_list_helper(lists_wrapper, selected_list);
        }
        else if(!strcmp(input_command, "addtask"))
        {
            prompt_add_task_helper(*selected_list);
        }
        else if(!strcmp(input_command, "deletetask"))
        {
            prompt_delete_task_helper(*selected_list);
        }
        else if(!strcmp(input_command, "complete"))
        {
            prompt_complete_toggle(*selected_list);
        }
        else if(!strcmp(input_command, "quit"))
        {
            quit(running);
        }
        else 
        {
            printw("\nInvalid input.\n");
            refresh();
            getch();
        }
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}






//UI elements shzbreh
int print_header(List * selected_list)
{
    printw("===================================\n");
    if(selected_list)
    {
        printw("   To-Do List: [%s]\n", get_list_title(selected_list));
    }
    else
    {
        printw("   To-Do List: [No list selected]\n"); 
    }
    printw("===================================\n\n");
    return EXIT_SUCCESS;
}

int print_task(Task * task, int num)
{
    if(task)
    {
        char complete = ' ';
        if(task->complete == true)
        {
            complete = 'x';
        }
        printw("%d. [%c] %s\n", num, complete, task->description);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int print_list(List * selected_list)
{
    if(selected_list)
    {
        if(selected_list->num_tasks)
        {
            Task * cur = selected_list->task;
            for(int i = 0; i < selected_list->num_tasks; i++)
            {
                print_task(cur, i+1);
                cur = cur->next_task;
            }
        }
        else
        {
            printw("No tasks in list.\nType * to add your first task.\n");
        }
        printw("\n");
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int print_commands()
{
    printw("Commands: (type ? to toggle)\n");
    printw("[addtask], [deletetask], [complete]\n");   
    printw("[switchlist], [createlist], [deletelist]\n");
    printw("[quit]\n");
    printw("enter command: ");
}



int main()
{
    initscr(); //begin curse mode

    //declare variables
    int running = 1;
    int amt_task_list = 0;
    ListsWrapper * lists_wrapper = init_lists_wrapper();
    List * selected_list = NULL;

    while(running)
    {
        print_header(selected_list);
        if(lists_wrapper->num_lists)
        {
            if(selected_list)
            {
                if(selected_list->num_tasks)
                {
                    print_list(selected_list);
                }
                else
                {
                    printw("Type addtask to add your first task!\n\n");
                }
            }
        }
        else
        {
            printw("Type [createlist] to create your first list!\n\n");
        }
    

        print_commands();

        refresh();
        //getch();


        char input_command[13];
        getstr(input_command);

        command_handler(input_command, lists_wrapper, &selected_list, &running);

        erase();
    }


    endwin();


    return EXIT_SUCCESS;
}