#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DESCR_SIZE 51
#define MAX_TASKS 10
#define ACTIVITY_SIZE 10
#define USER_SIZE 10
#define DEFAULT_ACTIVITY 0
#define INIT_TIME 0
#define MAX_USERS 50
#define MAX_L_USERS 20

int time, nr_tasks = 0, nr_users = 0, nr_activities = 3;
char option, activities[][ACTIVITY_SIZE] = {"TO DO", "MEH", "MEH2"}, users[MAX_USERS][MAX_L_USERS];

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

void coloca_pos(struct task current){
    int i = 0, c = 0, state = 0;
    struct task new_tasks[MAX_TASKS];

    new_tasks[0] = current;

    for (i = 0; i < nr_tasks; i++){
        if (state == 0){
            if(tasks[i].description[c] == current.description[c]){
                c++;
                i--;
                /*new_tasks[i] = tasks[i];*/
            }
            else if (tasks[i].description[c] > current.description[c])
            {
                new_tasks[i] = current;
                state = 1;
                i--;
            }
            else{
                new_tasks[i] = tasks[i];
                i++;
                new_tasks[i] = current;
                state = 1;
            }
        }
        else{
            new_tasks[i + 1] = tasks[i];
        }
    }
    for (i = 0; i <= nr_tasks; i++){
        tasks[i] = new_tasks[i];
    }
}

void add_task()
{
    int i, pos;

    if (nr_tasks + 1 > MAX_TASKS)
    {
        printf("too many tasks\n");
        exit(1);
    }

    scanf(" %d ", &current.e_duration);
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
    coloca_pos(current);
    /*tasks[nr_tasks] = current;*/
    nr_tasks++;

    printf("task %d\n", current.identifier);
}

void list_tasks()
{
    int i, id = 0, found;
    char ch;
    struct task current;

    if ((ch = getchar()) == ' ')
    {
        while (scanf(" %d", &id))
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
            if (found == 0)
            {
                printf("%d: no such task\n", id);
            }
        }
    }

    else
    {
        for (i = 0; i < nr_tasks; i++)
        {
            current = tasks[i];
            printf("%d %s #%d %s", current.identifier, current.activity, current.e_duration,
                   current.description);
        }
    }
}

void step_forward()
{
    int dur = 0;
    char end;

    if (scanf("%d%c", &dur, &end) == 2 && end == '\n' && dur > 0)
    {
        time += dur;
        printf("%d\n", time);
    }
    else
    {
        printf("invalid time\n");
    }
}

void list_users()
{
    int i = 0;
    for (i = 0; i < nr_users; i++)
    {
        printf("%s\n", users[i]);
    }
}

void add_user()
{
    char user[MAX_L_USERS];
    int i;

    if (nr_users < MAX_USERS)
    {
        if (getchar() == ' ')
        {
            fscanf(stdin, "%s", user);
            for (i = 0; i < nr_users; i++)
            {
                if (!strcmp(user, users[i]))
                {
                    printf("user already exists\n");
                    exit(1);
                }
            }

            strcpy(users[nr_users], user);
            nr_users++;

            if (nr_users != 1)
            {
                list_users();
            }
        }
    }
    else
    {
        printf("too many users\n");
    }
}

void move_task()
{
    int id, i/*, act*/;
    char user[MAX_L_USERS], activity[ACTIVITY_SIZE];
    scanf("%d %s %s", &id, user, activity);
    for (i = 0; i < nr_activities; i++)
    {
        if(strcmp(activity, activities[i])){
            /*act = i;*/  
        }
    }
    for (i = 0; i < nr_tasks; i++)
    {
        if (id == tasks[i].identifier)
        {
            current = tasks[i];
            strcpy(current.user, user);
        }
    }
}

void menu()
{
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
        list_tasks();
        break;
    case 'n':
        step_forward();
        break;
    case 'u':
        add_user();
        break;
    case 'm':
        move_task();
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
    time = INIT_TIME;
    menu();

    return 0;
}