#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DESCR_SIZE 51
#define MAX_TASKS 10001
#define ACTIVITY_SIZE 20
#define MAX_ACTIVITIES 10
#define DEFAULT_ACTIVITY "TO DO"
#define FINAL_ACTIVITY "DONE"
#define INIT_TIME 0
#define MAX_USERS 50
#define USER_SIZE 20

int time, nr_tasks = 0, nr_users = 0, nr_activities = 3;
char option, activities[MAX_ACTIVITIES][ACTIVITY_SIZE] = {"TO DO", "IN PROGRESS", "DONE"}, users[MAX_USERS][USER_SIZE];

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

void coloca_pos_desc(struct task current)
{
    int i = 0, c = 0, state = 0;
    struct task new_tasks[MAX_TASKS];

    for (i = 0; i <= nr_tasks; i++)
    {
        if (state == 0)
        {
            if (i < nr_tasks)
            {
                if (tasks[i].description[c] < current.description[c])
                {
                    c = 0;
                    new_tasks[i] = tasks[i];
                }
                else if (tasks[i].description[c] == current.description[c])
                {
                    c++;
                    i--;
                }
                else if (tasks[i].description[c] > current.description[c])
                {
                    new_tasks[i] = current;
                    state = 1;
                    i--;
                }
                else
                {
                    new_tasks[i] = tasks[i];
                    i++;
                    new_tasks[i] = current;
                    state = 1;
                }
            }
            else
            {
                new_tasks[i] = current;
            }
        }
        else
        {
            new_tasks[i + 1] = tasks[i];
        }
    }
    for (i = 0; i <= nr_tasks; i++)
    {
        tasks[i] = new_tasks[i];
    }
}

void add_task()
{
    int i;

    if (nr_tasks + 1 == MAX_TASKS)
    {
        printf("too many tasks\n");
        return;
    }

    scanf(" %d ", &current.e_duration);
    fgets(current.description, DESCR_SIZE, stdin);

    for (i = 0; i < nr_tasks; i++)
    {
        if (strcmp(tasks[i].description, current.description) == 0)
        {
            printf("duplicate description\n");
            return;
        }
    }

    strcpy(current.activity, DEFAULT_ACTIVITY);

    current.identifier = nr_tasks + 1;
    coloca_pos_desc(current);
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
    char user[USER_SIZE];
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
                    return;
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
    int id, i, found = 0, act, duration, slack;
    char user[USER_SIZE], activity[ACTIVITY_SIZE];

    scanf("%d %s ", &id, user);
    fgets(activity, ACTIVITY_SIZE, stdin);
    activity[strlen(activity) - 1] = '\0';

    if (!strcmp(activity, DEFAULT_ACTIVITY))
    {
        printf("task already started\n");
        return;
    }

    for (i = 0; i < nr_activities; i++)
    {
        if (!strcmp(activity, activities[i]))
        {
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("no such activity\n");
        return;
    }

    found = 0;

    for (i = 0; i < nr_users; i++)
    {
        if (!strcmp(user, users[i]))
        {
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("no such user\n");
        return;
    }

    found = 0;

    for (i = 0; i < nr_tasks; i++)
    {
        if (id == tasks[i].identifier)
        {
            act = i;
            found++;
            break;
        }
    }

    if (found == 0)
    {
        printf("no such task\n");
        return;
    }

    if (!strcmp(tasks[act].activity, DEFAULT_ACTIVITY))
    {
        tasks[act].t_beginning = time;
    }

    strcpy(tasks[act].user, user);
    strcpy(tasks[act].activity, activity);

    if (!strcmp(activity, FINAL_ACTIVITY))
    {
        duration = time - tasks[act].t_beginning;
        slack = duration - tasks[act].e_duration;
        printf("duration=%d slack=%d\n", duration, slack);
    }
}

void coloca_pos_time(struct task dest[], int count)
{
    int i = 0, state = 0, dest_t = 0, current_t = 0;
    struct task new_tasks[MAX_TASKS];

    for (i = 0; i <= count; i++)
    {
        dest_t = dest[i].t_beginning;
        current_t = current.t_beginning;

        if (state == 0)
        {
            if (i < count)
            {
                if (dest_t <= current_t)
                {
                    new_tasks[i] = dest[i];
                }
                else if (dest_t > current_t)
                {
                    new_tasks[i] = current;
                    state = 1;
                }
            }
            else
            {
                new_tasks[i] = current;
            }
        }
        else
        {
            new_tasks[i] = dest[i - 1];
        }
    }
    for (i = 0; i <= count; i++)
    {
        dest[i] = new_tasks[i];
    }
}

void list_act()
{
    int i = 0, count = 0, found = 0;
    char activity[ACTIVITY_SIZE];
    struct task list[MAX_TASKS] = {0};

    getchar();
    fgets(activity, ACTIVITY_SIZE, stdin);
    activity[strlen(activity) - 1] = '\0';

    for (i = 0; i < nr_activities; i++)
    {
        if (!strcmp(activity, activities[i]))
        {
            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        printf("no such activity\n");
        return;
    }

    for (i = 0; i < nr_tasks; i++)
    {
        if (!strcmp(tasks[i].activity, activity))
        {
            current = tasks[i];
            coloca_pos_time(list, count);
            count++;
        }
    }

    for (i = 0; i < count; i++)
    {
        printf("%d %d %s", list[i].identifier, list[i].t_beginning, list[i].description);
    }
}

void activ()
{
    int i, size;
    char activity[ACTIVITY_SIZE] = "\0";

    if (getchar() == ' ')
    {
        fgets(activity, ACTIVITY_SIZE, stdin);
        activity[strlen(activity) - 1] = '\0';
        size = strlen(activity);
        for (i = 0; i < size; i++)
        {
            if (activity[i] > 'Z' || activity[i] < 'A')
            {
                printf("invalid description\n");
                return;
            }
        }

        strcpy(activities[nr_activities], activity);
        nr_activities++;
        return;
    }
    for (i = 0; i < nr_activities; i++)
    {
        printf("%s\n", activities[i]);
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
        return;
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
        list_act();
        break;
    case 'a':
        activ();
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