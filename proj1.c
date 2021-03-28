#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DESCR_SIZE 51
#define MAX_TASKS 10
#define ACTIVITY_SIZE 10
#define USER_SIZE 10
#define DEFAULT_ACTIVITY 0

int init_time = 0, nr_tasks = 0;
char option, activities[][ACTIVITY_SIZE] = {"TO DO", "MEH", "MEH2"};

struct task
{
    int identifier;
    char description[DESCR_SIZE];
    char user[USER_SIZE];
    char activity[ACTIVITY_SIZE];
    int e_duration;
    int t_beginning;
};

struct task tasks[MAX_TASKS];
struct task current;

void add_task()
{
    int i;

    if (nr_tasks + 1 > MAX_TASKS)
    {
        printf("too many tasks\n");
        exit(1);
    }

    scanf("%d", &current.e_duration);
    fgets(current.description, DESCR_SIZE, stdin);

    for (i = 0; i < nr_tasks; i++)
    {
        if (strcmp(tasks[i].description, current.description) == 0)
        {
            printf("duplicate description\n");
            exit(1);
        }
    }

    strcpy(current.activity, activities[DEFAULT_ACTIVITY]);

    current.identifier = nr_tasks + 1;
    tasks[nr_tasks] = current;
    nr_tasks++;

    printf("task %d\n", current.identifier);
}

void list()
{
    int i, id = 0, found;
    char ch;
    struct task current;

    if ((ch = getchar()) == ' ')
    {
        ch = getchar();
        while (scanf("%d", &id) != EOF)
        {
            found = 0;
            for (i = 0; i < MAX_TASKS; i++)
            {
                if (tasks[i].identifier == id)
                {
                    current = tasks[i];
                    printf("%d %s #%d %s", current.identifier, current.activity,
                           current.e_duration, current.description);
                    found++;
                    break;
                }
            }
            if (found == 0){
                printf("%d: no such task\n", id);
            }
            ch = getchar();
            if (ch == ']')
            {
                scanf("%c", &ch);
                break;
            }
            scanf("%d", &id);
        }
    }

    else
    {
        for (i = 0; i < nr_tasks; i++)
        {
            current = tasks[i];
            printf("%d %s #%d %s\n", current.identifier, current.activity, current.e_duration,
                   current.description);
        }
    }
}

void menu()
{

    printf("q	termina o programa\n\
t	adiciona uma nova tarefa ao sistema\n\
l	lista as tarefas\n\
n	avança o tempo do sistema\n\
u	adiciona um utilizador ou lista todos os utilizadores\n\
m	move uma tarefa de uma atividade para outra\n\
d	lista todas as tarefas que estejam numa dada atividade\n\
a	adiciona uma atividade ou lista todas as atividades\n\n");

    option = getchar();

    /*
q	termina o programa
t	adiciona uma nova tarefa ao sistema
l	lista as tarefas
n	avança o tempo do sistema
u	adiciona um utilizador ou lista todos os utilizadores
m	move uma tarefa de uma atividade para outra
d	lista todas as tarefas que estejam numa dada atividade
a	adiciona uma atividade ou lista todas as atividades
*/
    switch (option)
    {
    case 'q':
        exit(0);
        break;
    case 't':
        add_task();
        break;
    case 'l':
        list();
        break;
    case 'n':
        break;
    case 'u':
        break;
    case 'm':
        break;
    case 'd':
        break;
    case 'a':
        break;
    }
    menu();
}

int main()
{

    menu();

    return 0;
}