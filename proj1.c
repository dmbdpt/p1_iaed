#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* General defines */
#define DESCR_SIZE 50
#define MAX_TASKS 10000
#define ACTIVITY_SIZE 20
#define MAX_ACTIVITIES 10
#define DEFAULT_ACTIVITY "TO DO"
#define FINAL_ACTIVITY "DONE"
#define INIT_TIME 0
#define MAX_USERS 50
#define USER_SIZE 20
#define SORT_TYPE_TIME 0
#define SORT_TYPE_DESC 1

/* String defines (Valid) */
#define VALID_TASK "task %d\n"
#define LIST_TASK "%d %s #%d %s\n"
#define PRINT_TIME "%d\n"
#define LIST_STRING "%s\n"
#define VALID_TODO "duration=%d slack=%d\n"
#define VALID_LIST_ACT "%d %d %s\n"

/* String defines (Errors - General) */
#define ERROR_INV_TIME "invalid time\n"
#define ERROR_INV_DESC "invalid description\n"
#define ERROR_TASK_AS "task already started\n"

/* String defines (Errors - Item not found) */
#define ERROR_NO_TASK "%d: no such task\n"
#define ERROR_NO_TASK_S "no such task\n"
#define ERROR_NO_USER "no such user\n"
#define ERROR_NO_ACTIVITY "no such activity\n"

/* String defines (Errors - Duplicated Item) */
#define ERROR_DUP_DESC "duplicate description\n"
#define ERROR_DUP_ACT "duplicate activity\n"
#define ERROR_USER_AE "user already exists\n"

/* String defines (Errors - Size Limit) */
#define ERROR_TM_TASK "too many tasks\n"
#define ERROR_TM_USERS "too many users\n"
#define ERROR_TM_ACT "too many activities\n"

/* Sorting Macros */
#define key(A)(A)
#define less(A, B)(key(A) < key(B))

int time, nr_tasks = 0, nr_users = 0, nr_activities = 3;
char option,
activities[MAX_ACTIVITIES + 1]
    [ACTIVITY_SIZE + 1] = {
        "TO DO",
        "IN PROGRESS",
        "DONE"
    },
    users[MAX_USERS + 1][USER_SIZE + 1];

typedef struct Task {
    int identifier;
    char description[DESCR_SIZE + 1];
    char user[USER_SIZE + 1];
    char activity[ACTIVITY_SIZE];
    int e_duration;
    int t_beginning;
}
Task;

Task tasks[MAX_TASKS + 1];
Task current;
Task aux[MAX_TASKS + 1];

void merge_time(Task a[], int l, int m, int r) {
    int i, j, k;
    for (i = m + 1; i > l; i--)
        aux[i - 1] = a[i - 1];
    for (j = m; j < r; j++)
        aux[r + m - j] = a[j + 1];
    for (k = l; k <= r; k++)
        if (i > m)
            a[k] = aux[j--];
        else if (aux[j].t_beginning < aux[i].t_beginning)
        a[k] = aux[j--];
    else
        a[k] = aux[i++];
}

void merge_desc(Task a[], int l, int m, int r) {
    int i, j, k;
    for (i = m + 1; i > l; i--)
        aux[i - 1] = a[i - 1];
    for (j = m; j < r; j++)
        aux[r + m - j] = a[j + 1];
    for (k = l; k <= r; k++)
        if (strcmp(aux[j].description, aux[i].description) < 0)
            a[k] = aux[j--];
        else
            a[k] = aux[i++];
}

void sort(Task a[], int l, int r, int type) {
    int m = (r + l) / 2;
    if (l >= r)
        return;
    sort(a, l, m, type);
    sort(a, m + 1, r, type);
    if (type == SORT_TYPE_TIME)
        merge_time(a, l, m, r);
    else
        merge_desc(a, l, m, r);
}

void add_task() {
    int i;
    scanf(" %d ", & current.e_duration);
    fgets(current.description, DESCR_SIZE + 1, stdin);
    current.description[strcspn(current.description, "\n")] = 0;

    if (nr_tasks == MAX_TASKS) {
        printf(ERROR_TM_TASK);
        return;
    }

    for (i = 0; i < nr_tasks; i++) {
        if (strcmp(tasks[i].description, current.description) == 0) {
            printf(ERROR_DUP_DESC);
            return;
        }
    }

    strcpy(current.activity, DEFAULT_ACTIVITY);

    current.identifier = nr_tasks + 1;
    tasks[nr_tasks] = current;
    nr_tasks++;

    printf(VALID_TASK, current.identifier);
}

void list_tasks() {
    int i, id = 0, found;
    char ch;

    if ((ch = getchar()) == ' ') {
        while (scanf(" %d", & id)) {
            found = 0;
            for (i = 0; i < nr_tasks; i++) {
                if (tasks[i].identifier == id) {
                    current = tasks[i];
                    printf(LIST_TASK, current.identifier, current.activity,
                        current.e_duration, current.description);
                    found++;
                    break;
                }
            }
            if (found == 0) {
                printf(ERROR_NO_TASK, id);
            }
        }
    } else {
        sort(tasks, 0, nr_tasks - 1, SORT_TYPE_DESC);
        for (i = 0; i < nr_tasks; i++) {
            current = tasks[i];
            printf(LIST_TASK, current.identifier, current.activity,
                current.e_duration, current.description);
        }
    }
}

void step_forward() {
    int dur = 0;
    char end;

    if (scanf("%d%c", & dur, & end) == 2 && end == '\n' && dur >= 0) {
        time += dur;
        printf(PRINT_TIME, time);
    } else {
        printf(ERROR_INV_TIME);
    }
}

void list_users() {
    int i = 0;
    for (i = 0; i < nr_users; i++) {
        printf(LIST_STRING, users[i]);
    }
}

void add_user() {
    char user[USER_SIZE + 1];
    int i;

    if (getchar() == ' ') {
        fscanf(stdin, "%s", user);
        if (nr_users == MAX_USERS) {
            printf(ERROR_TM_USERS);
            return;
        }
        for (i = 0; i < nr_users; i++) {
            if (!strcmp(user, users[i])) {
                printf(ERROR_USER_AE);
                return;
            }
        }

        strcpy(users[nr_users], user);
        nr_users++;
    } else {
        list_users();
    }
}

void move_task() {
    int id, i, found = 0, act, duration, slack;
    char user[USER_SIZE + 1], activity[ACTIVITY_SIZE + 1];

    scanf("%d %s ", & id, user);
    fgets(activity, ACTIVITY_SIZE + 1, stdin);
    if (activity[strlen(activity) - 1] == '\n') {
        activity[strlen(activity) - 1] = '\0';
    }

    for (i = 0; i < nr_tasks; i++) {
        if (id == tasks[i].identifier) {
            act = i;
            found++;
            break;
        }
    }

    if (found == 0) {
        printf(ERROR_NO_TASK_S);
        return;
    }

    if (!strcmp(activity, DEFAULT_ACTIVITY)) {
        printf(ERROR_TASK_AS);
        return;
    }

    found = 0;

    for (i = 0; i < nr_users; i++) {
        if (!strcmp(user, users[i])) {
            found = 1;
        }
    }

    if (found == 0) {
        printf(ERROR_NO_USER);
        return;
    }

    found = 0;

    for (i = 0; i < nr_activities; i++) {
        if (!strcmp(activity, activities[i])) {
            found = 1;
        }
    }

    if (found == 0) {
        printf(ERROR_NO_ACTIVITY);
        return;
    }

    if (!strcmp(tasks[act].activity, DEFAULT_ACTIVITY)) {
        tasks[act].t_beginning = time;
    }

    strcpy(tasks[act].user, user);

    if (!strcmp(activity, FINAL_ACTIVITY) &&
        strcmp(tasks[act].activity, FINAL_ACTIVITY)) {
        duration = time - tasks[act].t_beginning;
        slack = duration - tasks[act].e_duration;
        printf(VALID_TODO, duration, slack);
    }
    strcpy(tasks[act].activity, activity);
}

void list_act() {
    int i = 0, count = 0, found = 0;
    char activity[ACTIVITY_SIZE + 1];
    Task list[MAX_TASKS + 1];

    getchar();
    fgets(activity, ACTIVITY_SIZE + 1, stdin);
    activity[strlen(activity) - 1] = '\0';

    for (i = 0; i < nr_activities; i++) {
        if (!strcmp(activity, activities[i])) {
            found = 1;
            break;
        }
    }

    if (found == 0) {
        printf(ERROR_NO_ACTIVITY);
        return;
    }

    for (i = 0; i < nr_tasks; i++) {
        if (!strcmp(tasks[i].activity, activity)) {
            list[count] = tasks[i];
            count++;
        }
    }

    sort(list, 0, count - 1, SORT_TYPE_DESC);
    sort(list, 0, count - 1, SORT_TYPE_TIME);

    for (i = 0; i < count; i++) {
        printf(VALID_LIST_ACT, list[i].identifier, list[i].t_beginning,
            list[i].description);
    }
}

void activ() {
    int i, size;
    char activity[ACTIVITY_SIZE + 1];

    if (getchar() == ' ') {
        fgets(activity, ACTIVITY_SIZE + 1, stdin);
        if (nr_activities == MAX_ACTIVITIES) {
            printf(ERROR_TM_ACT);
        }

        size = strlen(activity);
        for (i = 0; i < size; i++) {
            if (activity[i] == '\n') {
                activity[i] = '\0';
                break;
            }
            if (activity[i] >= 'a' && activity[i] <= 'z') {
                printf(ERROR_INV_DESC);
                return;
            }
        }

        for (i = 0; i < nr_activities; i++) {
            if (!strcmp(activity, activities[i])) {
                printf(ERROR_DUP_ACT);
                return;
            }
        }

        strcpy(activities[nr_activities], activity);
        nr_activities++;
        return;
    }
    for (i = 0; i < nr_activities; i++) {
        printf(LIST_STRING, activities[i]);
    }
}

int main() {
    time = INIT_TIME;

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
            activ();
            break;
        }
    }
    return 0;
}