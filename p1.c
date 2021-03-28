#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DESCR_SIZE  51
#define NR_TASKS     10
#define ACTIVITY_SIZE    10
#define USER_SIZE    10
#define DEFAULT_ACTIVITY 0

int init_time = 0, active_tasks = 0;
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

struct task tasks[NR_TASKS];

void add_task(){

    struct task current;

    scanf("%d", &current.e_duration);
    fgets(current.description, DESCR_SIZE, stdin);
    strcpy(current.activity, activities[DEFAULT_ACTIVITY]);

    current.identifier = active_tasks + 1;
    tasks[active_tasks] = current;
    active_tasks++;

    printf("task %d\n", current.identifier);
}

void list(){
    int i;
    struct task current;

    for(i = 0; i < active_tasks; i++){
        current = tasks[i];
        printf("%d %s #%d %s\n", current.identifier, current.activity, current.e_duration,
                current.description);
    }
}

void menu(){

    printf("q	termina o programa\n\
t	adiciona uma nova tarefa ao sistema\n\
l	lista as tarefas\n\
n	avança o tempo do sistema\n\
u	adiciona um utilizador ou lista todos os utilizadores\n\
m	move uma tarefa de uma atividade para outra\n\
d	lista todas as tarefas que estejam numa dada atividade\n\
a	adiciona uma atividade ou lista todas as atividades\n");


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
    switch (option){    
        case 'q':
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
    if(option != 'q')
        menu();
}

int main(){

    menu();

    return 0;
}