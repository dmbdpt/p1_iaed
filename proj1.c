/*
 * File:  proj1.c
 * Author:  Diogo Dinis (99066)
 * Description: IAED 1st Project - 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --General defines-- */

/* Defines the max. size of a task description */
#define TASK_DESCR_SIZE 50
/* Defines the max. number of tasks */
#define MAX_TASKS 10000
/* Defines the max. size of an activity name */
#define ACTIVITY_SIZE 20
/* Defines the max. number of activities */
#define MAX_ACTIVITIES 10
/* Defines the starting activity */
#define DEFAULT_ACTIVITY "TO DO"
/* Defines the ending activity */
#define FINAL_ACTIVITY "DONE"
/* Defines the initial time */
#define INIT_TIME 0
/* Defines the max. number of users */
#define MAX_USERS 50
/* Defines the max. size of an user name */
#define USER_SIZE 20
/* Value to choose sorting by time */
#define SORT_TYPE_TIME 0
/* Value to choose sorting by description */
#define SORT_TYPE_DESC 1

/* --String defines (Valid)-- */
#define VALID_TASK "task %d\n"
#define LIST_TASK "%d %s #%d %s\n"
#define PRINT_TIME "%d\n"
#define LIST_STRING "%s\n"
#define VALID_TODO "duration=%d slack=%d\n"
#define VALID_LIST_ACT "%d %d %s\n"

/* --String defines (Errors - General)-- */
#define ERROR_INV_TIME "invalid time\n"
#define ERROR_INV_DUR "invalid duration\n"
#define ERROR_INV_DESC "invalid description\n"
#define ERROR_TASK_AS "task already started\n"

/* --String defines (Errors - Item not found)-- */
#define ERROR_NO_TASK "%d: no such task\n"
#define ERROR_NO_TASK_S "no such task\n"
#define ERROR_NO_USER "no such user\n"
#define ERROR_NO_ACTIVITY "no such activity\n"

/* --String defines (Errors - Duplicated Item)-- */
#define ERROR_DUP_DESC "duplicate description\n"
#define ERROR_DUP_ACT "duplicate activity\n"
#define ERROR_USER_AE "user already exists\n"

/* --String defines (Errors - Size Limit)-- */
#define ERROR_TM_TASK "too many tasks\n"
#define ERROR_TM_USERS "too many users\n"
#define ERROR_TM_ACT "too many activities\n"

/* --Sorting Macros-- */
#define key(A) (A)
#define less(A, B) (key(A) < key(B))

/* Initializes global time */
int time = INIT_TIME;

/* Initializes counting of tasks, users and activities */
int nr_tasks = 0, nr_users = 0, nr_activities = 3;

/* Initializes list of activities and users */
char activities[MAX_ACTIVITIES + 1]
               [ACTIVITY_SIZE + 1] = {"TO DO", "IN PROGRESS", "DONE"},
                                users[MAX_USERS + 1][USER_SIZE + 1];

/* Defines the Task struct */
typedef struct Task {
    int identifier;
    char description[TASK_DESCR_SIZE + 1];
    char user[USER_SIZE + 1];
    char activity[ACTIVITY_SIZE];
    int e_duration;
    int t_beginning;
} Task;

/* Initializes list of tasks*/
Task tasks[MAX_TASKS + 1];

/* Initializes auxiliary task and list of tasks */
Task current;
Task aux[MAX_TASKS + 1];

/* Auxiliary function of merge-sort (sort according task time) */
void merge_time(Task a[], int l, int m, int r) {
    int i, j, k;
    /* Fills auxiliary list */
    for (i = m + 1; i > l; i--)
        aux[i - 1] = a[i - 1];
    for (j = m; j < r; j++)
        aux[r + m - j] = a[j + 1];
    /* Add sorted elements to destination list */
    for (k = l; k <= r; k++)
        /* Adds stability to the sorting when t_beggining is the same */
        if (i > m)
            a[k] = aux[j--];
        else if (aux[j].t_beginning < aux[i].t_beginning)
            a[k] = aux[j--];
        else
            a[k] = aux[i++];
}

/* Auxiliary function of merge-sort (sort according task description) */
void merge_desc(Task a[], int l, int m, int r) {
    int i, j, k;
    /* Fills auxiliary list */
    for (i = m + 1; i > l; i--)
        aux[i - 1] = a[i - 1];
    for (j = m; j < r; j++)
        aux[r + m - j] = a[j + 1];
    /* Add sorted elements to destination list */
    for (k = l; k <= r; k++)
        if (strcmp(aux[j].description, aux[i].description) < 0)
            a[k] = aux[j--];
        else
            a[k] = aux[i++];
}

/* Main function of merge-sort */
void sort(Task a[], int l, int r, int type) {
    /* Chooses the point where to split */
    int m = (r + l) / 2;
    if (l >= r)
        return;
    /* Splits list */
    sort(a, l, m, type);
    sort(a, m + 1, r, type);
    /* Chooses whether to sort by description or beggining time */
    if (type == SORT_TYPE_TIME)
        merge_time(a, l, m, r);
    else
        merge_desc(a, l, m, r);
}

/* Function to add task from stdin input */
void add_task() {
    int i;
    scanf(" %d ", &current.e_duration);
    fgets(current.description, TASK_DESCR_SIZE + 1, stdin);
    current.description[strcspn(current.description, "\n")] = 0;

    /* Checks if limit not exceeded */
    if (nr_tasks == MAX_TASKS) {
        printf(ERROR_TM_TASK);
        return;
    }
    /* Checks if task already exists */
    for (i = 0; i < nr_tasks; i++) {
        if (strcmp(tasks[i].description, current.description) == 0) {
            printf(ERROR_DUP_DESC);
            return;
        }
    }

    if (current.e_duration <= 0) {
        printf(ERROR_INV_DUR);
        return;
    }

    strcpy(current.activity, DEFAULT_ACTIVITY);

    current.identifier = nr_tasks + 1;
    tasks[nr_tasks] = current;
    nr_tasks++;

    printf(VALID_TASK, current.identifier);
}

/* List specific task or all of them depending on input*/
void list_tasks() {
    int i, id = 0, found;
    char ch;

    if ((ch = getchar()) == ' ') {
        while (scanf(" %d", &id)) {
            /* List task(s) given by the input or error if it doesnt exist */
            for (i = 0; i < nr_tasks; i++) {
                found = 0;
                if (tasks[i].identifier == id) {
                    printf(LIST_TASK, tasks[i].identifier, tasks[i].activity,
                           tasks[i].e_duration, tasks[i].description);
                    found++;
                    break;
                }
            }
            if (!found)
                printf(ERROR_NO_TASK, id);
        }
    } else {
        /* List all tasks sorted by description */
        sort(tasks, 0, nr_tasks - 1, SORT_TYPE_DESC);
        for (i = 0; i < nr_tasks; i++) {
            current = tasks[i];
            printf(LIST_TASK, current.identifier, current.activity,
                   current.e_duration, current.description);
        }
    }
}

/* Shows or advances time depending on the input */
void step_forward() {
    int dur = 0;
    char end;
    /* Verifies if input is valid and adds time */
    if (scanf("%d%c", &dur, &end) == 2 && end == '\n' && dur >= 0) {
        time += dur;
        printf(PRINT_TIME, time);
    } else {
        printf(ERROR_INV_TIME);
    }
}

/* List all users */
void list_users() {
    int i = 0;
    for (i = 0; i < nr_users; i++) {
        printf(LIST_STRING, users[i]);
    }
}

/* Adds an user given from the input */
void add_user() {
    char user[USER_SIZE + 1];
    int i;

    if (getchar() == ' ') {
        fscanf(stdin, "%s", user);

        /* If user doesn't exists and max users not exceeded, add it */
        for (i = 0; i < nr_users; i++) {
            if (!strcmp(user, users[i])) {
                printf(ERROR_USER_AE);
                return;
            }
        }
        if (nr_users == MAX_USERS) {
            printf(ERROR_TM_USERS);
            return;
        }
        strcpy(users[nr_users], user);
        nr_users++;

    } else {
        list_users();
    }
}

/* Verifies for impediments to moving tasks */
int mv_task_ver(char activity[], char user[], int *id, int *act) {
    int i;
    /* Verifies task existence */
    for (i = 0; i <= nr_tasks; i++) {
        if (i == nr_tasks) {
            printf(ERROR_NO_TASK_S);
            return 1;
        }
        if (*id == tasks[i].identifier) {
            *act = i;
            break;
        }
    }
    /* Verifies if task already started*/
    if (!strcmp(activity, DEFAULT_ACTIVITY)) {
        if (strcmp(tasks[*act].activity, DEFAULT_ACTIVITY))
            printf(ERROR_TASK_AS);
        return 1;
    }
    /* Verifies user existence */
    for (i = 0; i <= nr_users; i++) {
        if (i == nr_users) {
            printf(ERROR_NO_USER);
            return 1;
        }
        if (!strcmp(user, users[i]))
            break;
    }

    /* Verifies if activity exists */
    for (i = 0; i <= nr_activities; i++) {
        if (i == nr_activities) {
            printf(ERROR_NO_ACTIVITY);
            return 1;
        }
        if (!strcmp(activity, activities[i]))
            break;
    }

    return 0;
}

/* Moves task from current activity to one given by the input */
void move_task() {
    int id, act = 0, duration, slack;
    char user[USER_SIZE + 1], activity[ACTIVITY_SIZE + 1];

    /* Get input */
    scanf("%d %s ", &id, user);
    fgets(activity, ACTIVITY_SIZE + 1, stdin);
    if (activity[strlen(activity) - 1] == '\n') {
        activity[strlen(activity) - 1] = '\0';
    }

    /* Check for errors*/
    if (mv_task_ver(activity, user, &id, &act)) {
        return;
    }

    /* Update task beggining time*/
    if (!strcmp(tasks[act].activity, DEFAULT_ACTIVITY)) {
        tasks[act].t_beginning = time;
    }

    /* Update task user*/
    strcpy(tasks[act].user, user);

    /* If task is moved to the Final activity, print duration info*/
    if (!strcmp(activity, FINAL_ACTIVITY) &&
        strcmp(tasks[act].activity, FINAL_ACTIVITY)) {
        duration = time - tasks[act].t_beginning;
        slack = duration - tasks[act].e_duration;
        printf(VALID_TODO, duration, slack);
    }
    strcpy(tasks[act].activity, activity);
}

/* Verifies for impediments to list tasks of a given activity */
int ls_act_ver(char activity[]) {
    int i;
    /* Verifies if the given activity exists */
    for (i = 0; i < nr_activities; i++) {
        if (!strcmp(activity, activities[i])) {
            return 0;
        }
    }
    printf(ERROR_NO_ACTIVITY);
    return 1;
}

/* List all the tasks that are on a certain activity given by the input */
void list_act() {
    int i = 0, count = 0;
    char activity[ACTIVITY_SIZE + 1];
    Task list[MAX_TASKS + 1];

    /* Get input */
    getchar();
    fgets(activity, ACTIVITY_SIZE + 1, stdin);
    activity[strlen(activity) - 1] = '\0';

    /* Check errors */
    if (ls_act_ver(activity))
        return;

    /* Filters tasks to a given activity */
    for (i = 0; i < nr_tasks; i++) {
        if (!strcmp(tasks[i].activity, activity)) {
            list[count] = tasks[i];
            count++;
        }
    }

    /* Sorts the output by beggining time and then by description */
    sort(list, 0, count - 1, SORT_TYPE_DESC);
    sort(list, 0, count - 1, SORT_TYPE_TIME);

    for (i = 0; i < count; i++) {
        printf(VALID_LIST_ACT, list[i].identifier, list[i].t_beginning,
               list[i].description);
    }
}

/* Verifies for impediments to add an activity */
int add_act_ver(char activity[]) {
    int i, size = strlen(activity);

    /* Searches for invalid characters */
    for (i = 0; i < size; i++) {
        if (activity[i] >= 'a' && activity[i] <= 'z') {
            printf(ERROR_INV_DESC);
            return 1;
        }
    }

    /* Corrects the last character */
    if (activity[size - 1] == '\n') {
        activity[size - 1] = '\0';
    }

    /* Verifies if the activity already exists */
    for (i = 0; i < nr_activities; i++) {
        if (!strcmp(activity, activities[i])) {
            printf(ERROR_DUP_ACT);
            return 1;
        }
    }
    return 0;
}

/* Adds an activity from the input */
void add_activ() {
    int i;
    char activity[ACTIVITY_SIZE + 1];

    if (getchar() == ' ') {
        fgets(activity, ACTIVITY_SIZE + 1, stdin);
        if (nr_activities == MAX_ACTIVITIES) {
            printf(ERROR_TM_ACT);
            return;
        }

        if (add_act_ver(activity)) {
            return;
        }

        strcpy(activities[nr_activities], activity);
        nr_activities++;
        return;
    }
    for (i = 0; i < nr_activities; i++) {
        printf(LIST_STRING, activities[i]);
    }
}

/* Reads the option character from stdin and launches the function associated */
int main() {
    char option;
    while (1) {
        option = getchar();
        switch (option) {
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
                add_activ();
                break;
        }
    }
    return 0;
}