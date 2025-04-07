#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// ----------------------------
// Macro para declarar estructuras y prototipos
// ----------------------------
#define DECLARE_CIRCULAR_LIST(TYPE) \
    typedef struct NodeC_##TYPE { \
        TYPE data; \
        struct NodeC_##TYPE* next; \
    } NodeC_##TYPE; \
    \
    typedef struct { \
        NodeC_##TYPE* tail; \
        size_t length; \
    } CircularList_##TYPE; \
    \
    CircularList_##TYPE* circular_list_##TYPE##_create(void); \
    void circular_list_##TYPE##_destroy(CircularList_##TYPE* list); \
    bool circular_list_##TYPE##_append(CircularList_##TYPE* list, TYPE data); \
    bool circular_list_##TYPE##_insert(CircularList_##TYPE* list, TYPE data, size_t pos); \
    bool circular_list_##TYPE##_remove_at(CircularList_##TYPE* list, size_t pos); \
    bool circular_list_##TYPE##_get(const CircularList_##TYPE* list, size_t pos, TYPE* out); \
    size_t circular_list_##TYPE##_length(const CircularList_##TYPE* list); \
    void circular_list_##TYPE##_print(const CircularList_##TYPE* list, void (*print_fn)(TYPE));

// ----------------------------
// Macro para implementación
// ----------------------------
#define IMPLEMENT_CIRCULAR_LIST(TYPE) \
    CircularList_##TYPE* circular_list_##TYPE##_create(void) { \
        CircularList_##TYPE* list = malloc(sizeof(CircularList_##TYPE)); \
        if (!list) return NULL; \
        list->tail = NULL; \
        list->length = 0; \
        return list; \
    } \
    \
    void circular_list_##TYPE##_destroy(CircularList_##TYPE* list) { \
        if (!list || !list->tail) return; \
        NodeC_##TYPE* current = list->tail->next; \
        NodeC_##TYPE* start = current; \
        do { \
            NodeC_##TYPE* temp = current; \
            current = current->next; \
            free(temp); \
        } while (current != start); \
        free(list); \
    } \
    \
    bool circular_list_##TYPE##_append(CircularList_##TYPE* list, TYPE data) { \
        return circular_list_##TYPE##_insert(list, data, list->length); \
    } \
    \
    bool circular_list_##TYPE##_insert(CircularList_##TYPE* list, TYPE data, size_t pos) { \
        if (!list || pos > list->length) return false; \
        NodeC_##TYPE* new_node = malloc(sizeof(NodeC_##TYPE)); \
        if (!new_node) return false; \
        new_node->data = data; \
        \
        if (!list->tail) { \
            new_node->next = new_node; \
            list->tail = new_node; \
        } else if (pos == 0) { \
            new_node->next = list->tail->next; \
            list->tail->next = new_node; \
        } else { \
            NodeC_##TYPE* prev = list->tail->next; \
            for (size_t i = 0; i < pos - 1; ++i) { \
                prev = prev->next; \
            } \
            new_node->next = prev->next; \
            prev->next = new_node; \
            if (pos == list->length) list->tail = new_node; \
        } \
        list->length++; \
        return true; \
    } \
    \
    bool circular_list_##TYPE##_remove_at(CircularList_##TYPE* list, size_t pos) { \
        if (!list || list->length == 0 || pos >= list->length) return false; \
        NodeC_##TYPE* to_delete; \
        if (pos == 0) { \
            to_delete = list->tail->next; \
            if (list->length == 1) { \
                list->tail = NULL; \
            } else { \
                list->tail->next = to_delete->next; \
            } \
        } else { \
            NodeC_##TYPE* prev = list->tail->next; \
            for (size_t i = 0; i < pos - 1; ++i) { \
                prev = prev->next; \
            } \
            to_delete = prev->next; \
            prev->next = to_delete->next; \
            if (to_delete == list->tail) list->tail = prev; \
        } \
        free(to_delete); \
        list->length--; \
        return true; \
    } \
    \
    bool circular_list_##TYPE##_get(const CircularList_##TYPE* list, size_t pos, TYPE* out) { \
        if (!list || pos >= list->length || !out) return false; \
        NodeC_##TYPE* current = list->tail->next; \
        for (size_t i = 0; i < pos; ++i) { \
            current = current->next; \
        } \
        *out = current->data; \
        return true; \
    } \
    \
    size_t circular_list_##TYPE##_length(const CircularList_##TYPE* list) { \
        return list ? list->length : 0; \
    } \
    \
    void circular_list_##TYPE##_print(const CircularList_##TYPE* list, void (*print_fn)(TYPE)) { \
        if (!list || !list->tail || !print_fn) { \
            printf("[]\n"); \
            return; \
        } \
        printf("["); \
        NodeC_##TYPE* current = list->tail->next; \
        for (size_t i = 0; i < list->length; ++i) { \
            print_fn(current->data); \
            if (i != list->length - 1) printf(", "); \
            current = current->next; \
        } \
        printf("]\n"); \
    }

// ----------------------------
// Declaraciones para tipos concretos
// ----------------------------
DECLARE_CIRCULAR_LIST(int)
DECLARE_CIRCULAR_LIST(char)
DECLARE_CIRCULAR_LIST(float)

// ----------------------------
// Implementaciones para tipos concretos
// ----------------------------
#ifdef CIRCULAR_LIST_IMPLEMENTATION
IMPLEMENT_CIRCULAR_LIST(int)
IMPLEMENT_CIRCULAR_LIST(char)
IMPLEMENT_CIRCULAR_LIST(float)
#endif
