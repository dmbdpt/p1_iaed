#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DESCR_SIZE 50
#define MAX_TASKS 10000
#define ACTIVITY_SIZE 20
#define MAX_ACTIVITIES 10
#define DEFAULT_ACTIVITY "TO DO"
#define FINAL_ACTIVITY "DONE"
#define INIT_TIME 0
#define MAX_USERS 50
#define USER_SIZE 20

int time, nr_tasks = 0, nr_users = 0, nr_activities = 3;
char option, activities[MAX_ACTIVITIES + 1][ACTIVITY_SIZE + 1] = {"TO DO", "IN PROGRESS", "DONE"},
                                                            users[MAX_USERS + 1][USER_SIZE + 1];

typedef struct Task
{
    int identifier;
    char description[DESCR_SIZE + 1];
    char user[USER_SIZE + 1];
    char activity[ACTIVITY_SIZE];
    int e_duration;
    int t_beginning;
} Task;

Task tasks[MAX_TASKS + 1];
Task current;

void coloca_pos_desc(Task current)
{
    int i = 0, state = 0;
    Task aux;
    for (i = 0; i <= nr_tasks; i++)
    {
        if (!state)
        {
            if (i < nr_tasks)
            {
                if (strcmp(tasks[i].description, current.description) > 0)
                {
                    aux = tasks[i];
                    tasks[i] = current;
                    state = 1;
                }
            }
            else
            {
                tasks[i] = current;
                break;
            }
        }
        else
        {
            current = tasks[i];
            tasks[i] = aux;
            aux = current;
        }
    }
}

void add_task()
{
    int i;

    scanf(" %d ", &current.e_duration);
    fgets(current.description, DESCR_SIZE + 1, stdin);
    current.description[strcspn(current.description, "\n")] = 0;

    if (nr_tasks == MAX_TASKS)
    {
        printf("too many tasks\n");
        return;
    }

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
    Task current;

    if ((ch = getchar()) == ' ')
    {
        while (scanf(" %d", &id))
        {
            found = 0;
            for (i = 0; i < nr_tasks; i++)
            {
                if (tasks[i].identifier == id)
                {
                    current = tasks[i];
                    printf("%d %s #%d %s\n", current.identifier, current.activity,
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
            printf("%d %s #%d %s\n", current.identifier, current.activity, current.e_duration,
                   current.description);
        }
    }
}

void step_forward()
{
    int dur = 0;
    char end;

    if (scanf("%d%c", &dur, &end) == 2 && end == '\n' && dur >= 0)
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
    char user[USER_SIZE + 1];
    int i;

    if (getchar() == ' ')
    {
        fscanf(stdin, "%s", user);
        if (nr_users == MAX_USERS)
        {
            printf("too many users\n");
            return;
        }
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
    }

    else
    {
        list_users();
    }
}

void move_task()
{
    int id, i, found = 0, act, duration, slack;
    char user[USER_SIZE + 1], activity[ACTIVITY_SIZE + 1];

    scanf("%d %s ", &id, user);
    fgets(activity, ACTIVITY_SIZE + 1, stdin);
    if (activity[strlen(activity) - 1] == '\n')
    {
        activity[strlen(activity) - 1] = '\0';
    }

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

    if (!strcmp(activity, DEFAULT_ACTIVITY))
    {
        printf("task already started\n");
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

    if (!strcmp(tasks[act].activity, DEFAULT_ACTIVITY))
    {
        tasks[act].t_beginning = time;
    }

    strcpy(tasks[act].user, user);

    if (!strcmp(activity, FINAL_ACTIVITY) && strcmp(tasks[act].activity, FINAL_ACTIVITY))
    {
        duration = time - tasks[act].t_beginning;
        slack = duration - tasks[act].e_duration;
        printf("duration=%d slack=%d\n", duration, slack);
    }
        strcpy(tasks[act].activity, activity);

}

void coloca_pos_time(Task dest[], int count)
{
    int i = 0, state = 0, dest_t = 0, current_t = 0;
    Task new_tasks[MAX_TASKS + 1];

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
    char activity[ACTIVITY_SIZE + 1];
    Task list[MAX_TASKS + 1];

    getchar();
    fgets(activity, ACTIVITY_SIZE + 1, stdin);
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
        printf("%d %d %s\n", list[i].identifier, list[i].t_beginning, list[i].description);
    }
}

void activ()
{
    int i, size;
    char activity[ACTIVITY_SIZE + 1];

    if (getchar() == ' ')
    {

        fgets(activity, ACTIVITY_SIZE + 1, stdin);
        if (nr_activities == MAX_ACTIVITIES)
        {
            printf("too many activities\n");
        }

        size = strlen(activity);
        for (i = 0; i < size; i++)
        {
            if (activity[i] == '\n')
            {
                activity[i] = '\0';
                break;
            }
            if (activity[i] >= 'a' && activity[i] <= 'z')
            {
                printf("invalid description\n");
                return;
            }
        }

        for (i = 0; i < nr_activities; i++)
        {
            if (!strcmp(activity, activities[i]))
            {
                printf("duplicate activity\n");
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

int main()
{
    time = INIT_TIME;

    while (1)
    {
        option = getchar();
        switch (option)
        {
        case 'q':
            return 0;
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
    }
    return 0;
}