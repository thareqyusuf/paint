/**
 * @file coordinate_queue.h
 * @brief Queue data structure for coordinate point management
 */

#ifndef COORDINATE_QUEUE_H
#define COORDINATE_QUEUE_H

#include "coordinate_system.h"

/**
 * @brief Queue node structure for linked list implementation
 */
typedef struct coordinate_queue_node* queue_node_pointer;
typedef struct coordinate_queue_node {
    queue_node_pointer next_node;
    struct coordinate_point stored_coordinate;
} coordinate_queue_node;

/**
 * @brief Queue management structure
 */
typedef struct coordinate_point_queue {
    struct coordinate_queue_node* first_element;
    struct coordinate_queue_node* last_element;
} coordinate_point_queue;

/**
 * @brief Initialize empty coordinate queue
 * @param target_queue Queue to initialize
 */
void initialize_coordinate_queue(struct coordinate_point_queue* target_queue);

/**
 * @brief Check if coordinate queue is empty
 * @param target_queue Queue to check
 * @return 1 if empty, 0 if contains elements
 */
char is_coordinate_queue_empty(struct coordinate_point_queue* target_queue);

/**
 * @brief Add coordinate point to queue
 * @param target_queue Queue to add to
 * @param new_coordinate Coordinate to add
 */
void enqueue_coordinate_point(struct coordinate_point_queue* target_queue, 
                             struct coordinate_point new_coordinate);

/**
 * @brief Remove and return next coordinate from queue
 * @param target_queue Queue to remove from
 * @return Next coordinate point in queue
 */
struct coordinate_point dequeue_coordinate_point(struct coordinate_point_queue* target_queue);

#endif

