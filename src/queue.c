#include "../include/queue.h"

struct connection_queue_node *connection_queue_head = NULL;
struct connection_queue_node *connection_queue_tail = NULL;

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_condition_variable = PTHREAD_COND_INITIALIZER;

void enqueue_connection(int *client_socket_fd) {
    pthread_mutex_lock(&queue_mutex);

    struct connection_queue_node *new_node =
        (struct connection_queue_node *)malloc(
            sizeof(struct connection_queue_node));

    new_node->p_client_socket_fd = client_socket_fd;
    new_node->next = NULL;

    if (connection_queue_tail == NULL) {
        // empty queue
        connection_queue_head = new_node;
    } else {
        connection_queue_tail->next = new_node;
    }
    connection_queue_tail = new_node;

    pthread_mutex_unlock(&queue_mutex);
    pthread_cond_signal(&queue_condition_variable);
}

int *dequeue_connection(){
    pthread_mutex_lock(&queue_mutex);
    if(connection_queue_head == NULL){
        pthread_cond_wait(&queue_condition_variable, &queue_mutex);
        pthread_mutex_unlock(&queue_mutex);

        return NULL;
    }else{
        int *item = connection_queue_head->p_client_socket_fd;
        struct connection_queue_node *temp = connection_queue_head;

        connection_queue_head = connection_queue_head->next;
        if(connection_queue_head = NULL){
            connection_queue_head = NULL;
        }
        free(temp);
        pthread_mutex_unlock(&queue_mutex);
        return item;
    }
}

int connection_queue_size(){
    pthread_mutex_lock(&queue_mutex);
    int count = 0;

    struct connection_queue_node *current = connection_queue_head;
    while(current!=NULL){
        count++;
        current = current->next;
    }

    pthread_mutex_unlock(&queue_mutex);
    return count;
}