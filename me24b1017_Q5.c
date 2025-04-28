// EDSLT Assignment-1
// Cargo Drone Traffic Controller - Question No.5
// Santhana Sreenivasa S  | ME24B1017 Batch - 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 6  // max size for stack, queue

// Queue for normal delivery requests
char *delivery_requests[MAX];
int front = -1, rear = -1, count_delivery_requests = 0;

// Stack for urgent deliveries
char *Priority_Dispatcher[MAX];
int top = -1, count_urgent_deliveries = 0;

// temp pointer used across functions
char *temp;

// Flight log
char *flight_log[MAX];
int log_size = 0;

// Overloaded drones list
struct sll {
    char drone_id[200];
    struct sll *next;
} *head = NULL, *temp1, *newnode;

// Serviced drones list
struct dll {
    char drone_id[200];
    struct dll *next, *prev;
} *start = NULL, *last = NULL, *newdll;

// Emergency rerouting list
struct cll {
    char drone_id[200];
    struct cll *next;
} *headc = NULL, *temp2, *newc;

// Checking if queue is full
int qisfull() {
    return count_delivery_requests == MAX;
}

// Checking if queue is empty
int qisempty() {
    return front == -1;
}

// Adding a normal delivery to queue
void enqueue(char *item) {
    if (qisfull()) {
        printf("Queue full! Cannot accept more deliveries.\n");
        return;
    }
    if (front == -1) front = 0;
    rear++;
    delivery_requests[rear] = item;
    count_delivery_requests++;
    printf("Added to delivery queue: %s\n", item);
}

// Removing a normal delivery from queue
char* dequeue() {
    if (qisempty()) {
        printf("Queue empty!\n");
        return NULL;
    }
    temp = delivery_requests[front];
    if (front == rear) front = rear = -1;
    else front++;
    count_delivery_requests--;
    return temp;
}

// Pushing an urgent delivery into stack
void push() {
    if (top == MAX - 1) {
        printf("Urgent stack full! Can't prioritize more.\n");
        return;
    }
    char *item = dequeue();
    if (item != NULL) {
        top++;
        Priority_Dispatcher[top] = item;
        count_urgent_deliveries++;
        printf("Moved to urgent stack: %s\n", item);
    }
}

// Dispatching an urgent delivery
void pop() {
    if (top == -1) {
        printf("No urgent deliveries pending!\n");
        return;
    }
    char *dispatched = Priority_Dispatcher[top];
    printf("Urgent Delivery Dispatched: %s\n", dispatched);
    top--;
    count_urgent_deliveries--;
}

//Flight Log Functions

void log_delivery(char *item) {
    if (log_size < MAX) {
        flight_log[log_size++] = item;
    } else {
        //archive oldest
        for (int i = 0; i < MAX - 1; i++) {
            flight_log[i] = flight_log[i + 1];
        }
        flight_log[MAX - 1] = item;
    }
    printf("Logged delivery: %s\n", item);
}

//Viewing the flight log
void view_log() {
    printf("Flight Log (Recent Deliveries):\n");
    for (int i = 0; i < log_size; i++) {
        printf("%d. %s\n", i + 1, flight_log[i]);
    }
}

// Overloaded Drone

void add_overloaded(char *id) {
    newnode = (struct sll*)malloc(sizeof(struct sll));
    if (!newnode) {
        printf("Memory allocation failed!\n");
        return;
    }
    strcpy(newnode->drone_id, id);
    newnode->next = NULL;

    if (head == NULL)
        head = temp1 = newnode;
    else {
        temp1->next = newnode;
        temp1 = newnode;
    }
    printf("Added to overloaded drones: %s\n", id);
}

void move2serviced(char *id) {
    struct sll *prev = NULL;
    temp1 = head;

    while (temp1 != NULL && strcmp(temp1->drone_id, id) != 0) {
        prev = temp1;
        temp1 = temp1->next;
    }

    if (temp1 == NULL) {
        printf("Drone not found in overloaded list.\n");
        return;
    }

    if (prev == NULL) head = temp1->next;
    else prev->next = temp1->next;

    newdll = (struct dll*)malloc(sizeof(struct dll));
    if (!newdll) {
        printf("Memory allocation failed!\n");
        return;
    }
    strcpy(newdll->drone_id, id);
    newdll->next = newdll->prev = NULL;

    if (start == NULL)
        start = last = newdll;
    else {
        last->next = newdll;
        newdll->prev = last;
        last = newdll;
    }
    printf("Moved to serviced drones: %s\n", id);
}

//Traversing serviced drones
void traverse_forward() {
    struct dll *temp = start;
    printf("Serviced Drones (Forward Order): ");
    while (temp != NULL) {
        printf("%s ", temp->drone_id);
        temp = temp->next;
    }
    printf("\n");
}

void traverse_backward() {
    struct dll *temp = last;
    printf("Serviced Drones (Backward Order): ");
    while (temp != NULL) {
        printf("%s ", temp->drone_id);
        temp = temp->prev;
    }
    printf("\n");
}

//Emergency Rerouting

void add_emergency(char *id) {
    newc = (struct cll*)malloc(sizeof(struct cll));
    if (!newc) {
        printf("Memory allocation failed!\n");
        return;
    }
    strcpy(newc->drone_id, id);
    newc->next = NULL;

    if (headc == NULL) {
        headc = newc;
        newc->next = headc;
    } else {
        temp2 = headc;
        while (temp2->next != headc)
            temp2 = temp2->next;
        temp2->next = newc;
        newc->next = headc;
    }
    printf("Emergency reroute added: %s\n", id);
}

void traverse_emergency_twice() {
    if (headc == NULL) {
        printf("No emergency drones to display.\n");
        return;
    }

    temp2 = headc;
    printf("Emergency Drones (Two Full Circuits):\n");
    for (int i = 0; i < 2 * MAX; i++) {
        printf("%s -> ", temp2->drone_id);
        temp2 = temp2->next;
        if (temp2 == headc && i >= MAX - 1) break;
    }
    printf("...\n");
}

//Main Program

int main() {

    enqueue("Food");
    enqueue("Medicine");
    enqueue("Tools");
    enqueue("Water");
    enqueue("Parts");
    enqueue("Fuel");

    printf("\n");

    for (int i = 0; i < 6; i++) {
        push(); // prioritizing urgent deliveries
    }

    printf("\n");

    for (int i = 0; i < 6; i++) {
        pop(); // dispatching urgent deliveries
    }

    printf("\n");
    // Using a stack for urgent deliveries ensures latest emergencies like Fuel for half-empty drones are handled first!
    // This matches how real air emergency teams work in fast-changing situations.

    log_delivery("Del1");
    log_delivery("Del2");
    log_delivery("Del3");
    log_delivery("Del4");
    log_delivery("Del5");
    log_delivery("Del6");
    log_delivery("Del7");
    log_delivery("Del8");

    printf("\n");
    view_log(); // viewing the whole log at once
    printf("\n");
    // Maintaining only the latest few logs mimics real aviation systems where old logs get archived separately!

    add_overloaded("Drone3");
    add_overloaded("Drone6");

    move2serviced("Drone3");

    traverse_forward();
    traverse_backward();
    printf("\n");
    // Drone3 was overloaded due to carrying extra tools. After recalibration and load reduction, it's back in service!

    add_emergency("Drone1");
    add_emergency("Drone4");

    traverse_emergency_twice();
    printf("\n");

    // Drone4 got rerouted after sudden wind alerts. It was retrofitted mid-mission for storm handling and safely completed delivery!

    return 0;
}
