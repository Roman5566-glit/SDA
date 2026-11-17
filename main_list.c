#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define SEC_PER_PAGE 2
#define MAX_JOBS 20
#define TEST_SIZE 5

typedef struct {
    int id;
    int arrival;
    int pages;
} Job;

typedef struct Node {
    Job job;
    struct Node *next;
} Node;

Node *head = NULL;   
Node *tail = NULL;    
int count = 0;

bool is_empty(void)
{
    return head == NULL;
}

bool is_full(void)
{
    return false;    
}

bool enqueue(Job job)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        return false;
    }
    node->job = job;
    node->next = NULL;
    if (tail == NULL) {
        head = node;
    } else {
        tail->next = node;
    }
    tail = node;
    count++;
    return true;
}

bool dequeue(Job *job)
{
    if (is_empty()) {
        return false;
    }
    Node *old = head;
    *job = old->job;
    head = old->next;
    if (head == NULL) {
        tail = NULL;
    }
    free(old);
    count--;
    return true;
}

bool peek_front(Job *job)
{
    if (is_empty()) {
        return false;
    }
    *job = head->job;
    return true;
}

void clear_queue(void)
{
    Job tmp;
    while (dequeue(&tmp)) {
        ;
    }
}

Job make_job(int id, int arrival, int pages)
{
    Job j;
    j.id = id;
    j.arrival = arrival;
    j.pages = pages;
    return j;
}

void simulate(Job jobs[], int m)
{
    int next = 0;
    int now = 0;
    int busy = 0;
    Job cur;

    clear_queue();
    while (next < m || !is_empty()) {
        while (next < m && jobs[next].arrival <= now && !is_full()) {
            enqueue(jobs[next]);
            next++;
        }
        if (is_empty()) {
            printf("Принтер простоює з %d с до %d с\n", now, jobs[next].arrival);
            now = jobs[next].arrival;
            continue;
        }
        dequeue(&cur);
        int start = now;
        int len = cur.pages * SEC_PER_PAGE;
        now += len;
        busy += len;
        printf("Завдання %d: надійшло %d с, %d стор., "
               "друк %d..%d с, завершено у %d с\n",
               cur.id, cur.arrival, cur.pages, start, now, now);
    }
    printf("Останнє завдання завершилось у момент %d с\n", now);
    printf("Сумарний час роботи принтера: %d с (простій %d с)\n", busy, now - busy);
}

bool read_int(int *x)
{
    if (scanf("%d", x) == 1) {
        return true;
    }
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        ;
    }
    return false;
}

void run_example(void)
{
    Job jobs[] = {
        {1, 0, 4}, {2, 0, 2}, {3, 1, 5}, {4, 2, 1},
        {5, 3, 3}, {6, 4, 2}, {7, 5, 6}, {8, 60, 3}
    };
    simulate(jobs, 8);
}

void run_manual(void)
{
    Job jobs[MAX_JOBS];
    int m, prev = 0;

    printf("Кількість завдань (1..%d): ", MAX_JOBS);
    if (!read_int(&m) || m < 1 || m > MAX_JOBS) {
        printf("Некоректна кількість завдань\n");
        return;
    }
    for (int i = 0; i < m; i++) {
        int arrival, pages;
        printf("Завдання %d (час надходження, сторінок): ", i + 1);
        if (!read_int(&arrival) || !read_int(&pages) ||
            arrival < prev || pages < 1) {
            printf("Некоректні дані: час не має зменшуватись, "
                   "сторінок не менше 1\n");
            return;
        }
        jobs[i] = make_job(i + 1, arrival, pages);
        prev = arrival;
    }
    simulate(jobs, m);
}

void test_empty(void)
{
    Job j;
    clear_queue();
    printf("Порожня черга: is_empty = %d, елементів %d\n", is_empty(), count);
    if (!dequeue(&j)) {
        printf("dequeue: черга порожня, забирати нічого\n");
    }
    if (!peek_front(&j)) {
        printf("peek_front: черга порожня, дивитись нічого\n");
    }
    enqueue(make_job(1, 0, 3));
    peek_front(&j);
    printf("Додали завдання, на початку черги завдання %d\n", j.id);
    dequeue(&j);
    if (is_empty()) {
        printf("Забрали його, черга знову порожня\n");
    }
}

void test_full(void)
{
    int added = 0, rejected = 0;
    clear_queue();
    printf("Додаємо %d завдань (у масиві було б %d місць)\n",
           TEST_SIZE + 2, TEST_SIZE);
    for (int i = 1; i <= TEST_SIZE + 2; i++) {
        if (enqueue(make_job(i, 0, 1))) {
            added++;
        } else {
            printf("Не вдалося додати завдання %d\n", i);
            rejected++;
        }
    }
    printf("Додано %d, відхилено %d, у черзі %d\n", added, rejected, count);
    clear_queue();
}

void test_wrap(void)
{
    Job j;
    int k = 3;
    clear_queue();
    for (int i = 1; i <= TEST_SIZE; i++) {
        enqueue(make_job(i, 0, 1));
    }
    printf("Додали %d завдань, елементів %d\n", TEST_SIZE, count);
    printf("Забрали:");
    for (int i = 0; i < k; i++) {
        dequeue(&j);
        printf(" %d", j.id);
    }
    printf("\n");
    for (int i = 1; i <= k; i++) {
        enqueue(make_job(TEST_SIZE + i, 0, 1));
    }
    printf("Додали ще %d, елементів %d\n", k, count);
    printf("Порядок виймання:");
    while (dequeue(&j)) {
        printf(" %d", j.id);
    }
    printf("\n");
}

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    int choice;

    printf("Лабораторна робота 3. Черга принтера (список)\n");
    printf("1 - приклад\n");
    printf("2 - ввести завдання вручну\n");
    printf("3 - тест: порожня черга\n");
    printf("4 - тест: переповнення\n");
    printf("5 - тест: порядок FIFO\n");
    printf("0 - вихід\n");

    for (;;) {
        printf("Ваш вибір: ");
        if (!read_int(&choice)) {
            if (feof(stdin)) {
                break;
            }
            printf("Некоректне введення\n");
            continue;
        }
        if (choice == 0) {
            break;
        }
        switch (choice) {
            case 1: run_example(); break;
            case 2: run_manual(); break;
            case 3: test_empty(); break;
            case 4: test_full(); break;
            case 5: test_wrap(); break;
            default: printf("Немає такого пункту\n");
        }
    }
    clear_queue();
    return 0;
}
