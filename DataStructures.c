//
//  main.c
//  DataStructures
//
//  Created by Quantum on 1/4/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <memory.h>

//ARRAYLIST
struct ArrayList {
    int* arr;
    int size;
    int free;
};

struct ArrayList* newArrayList(int size) {
    struct ArrayList* list = malloc(1*sizeof(struct ArrayList));
    int exp = ceil(log2(size));
    list->arr = malloc((1 << exp)*sizeof(int));
    list->free = size;
    list->size = size;
    return list;
}

void append(struct ArrayList* list, int val) {
    if(list->free > 0) {
        list->arr[list->size - list->free] = val;
        list->free--;
    } else {
        int* temp = malloc(((list->size) << 1)*sizeof(int));
        memcpy(temp, list->arr, list->size*sizeof(int));
        free(list->arr);
        list->arr = temp;
        list->arr[list->size] = val;
        list->free = list->size-1;
        list->size = (list->size) << 1;
        temp = NULL;
    }
}

int getList(struct ArrayList* list, int index) {
    return list->arr[index];
}

void setList(struct ArrayList* list, int index, int val) {
    list->arr[index] = val;
}

int removeList(struct ArrayList* list, int index) {
    int val = list->arr[index];
    memmove((list->arr)+index, (list->arr)+index+1, (list->size)-index-1);
    return val;
}

void printList(struct ArrayList* list) {
    for(int i = 0; i < list->size; i++) {
        printf("%d, ", list->arr[i]);
    }
}

// QUEUE/STACK
struct QueueNode {
    struct QueueNode* next;
    int val;
    int dist;
};
struct QueueNode* newNode(int val, int dist) {
    struct QueueNode* node = malloc(1*sizeof(struct QueueNode));
    (*node).dist = dist;
    (*node).val = val;
    return node;
}

struct Queue {
    struct QueueNode* start;
    struct QueueNode* end;
};

struct Queue* newQueue(void) {
    struct Queue* new = malloc(1*sizeof(struct Queue));
    (*new).start = NULL;
    return new;
}

void freeQueue(struct Queue* queuePtr) {
    struct QueueNode* curPtr = (*queuePtr).start;
    struct QueueNode* freePtr;
    while(curPtr != NULL) {
        freePtr = curPtr;
        curPtr = (*curPtr).next;
        free(freePtr);
    }
}

void addQueue(struct Queue* queuePtr, struct QueueNode* new) {
    (*new).next = NULL;
    if((*queuePtr).start != NULL) {
        (*(*queuePtr).end).next = new;
        (*queuePtr).end = new;
    } else {
        (*queuePtr).start = new;
        (*queuePtr).end = (*queuePtr).start;
    }
}

void addStack(struct Queue* stackPtr, struct QueueNode* new) {
    if((*stackPtr).start == NULL) {
        (*stackPtr).start = new;
        (*stackPtr).end = (*stackPtr).start;
    } else  {
        (*new).next = (*stackPtr).start;
        (*stackPtr).start = new;
    }
}

struct QueueNode* deQueue(struct Queue* queuePtr) {
    struct QueueNode* ret = (*queuePtr).start;
    if((*queuePtr).start != NULL) {
        (*queuePtr).start = (*(*queuePtr).start).next;
    }
    return ret;
}

//debug method for Queue
void printQueue(struct Queue* queuePtr) {
    struct QueueNode* curPtr = (*queuePtr).start;
    while(curPtr != NULL) {
        printf("(%p, %d), ", curPtr, (*curPtr).val);
        curPtr = (*curPtr).next;
    }
    printf("\n");
}

// HASHMAP
struct HashMap {
    int size;
    int* hashTable;
    int rand;
    struct HashNode** nodes;
};

struct HashNode {
    int collisions;
    int key;
    int val;
    struct HashNode* next;
};

struct HashNode* newHashNode(int key, int val) {
    struct HashNode* new = malloc(1*sizeof(struct HashNode));
    (*new).key = key;
    (*new).val = val;
    (*new).collisions = 0;
    (*new).next = NULL;
    return new;
}

struct HashMap* newMap(int size) {
    int exp = (int) ceil(log2(size));
    struct HashMap* new = malloc(1*sizeof(struct HashMap));
    if(exp < 14) {
        (*new).size = 1 << (2*exp);
    } else {
        (*new).size = 1 << 28;
    }
    (*new).nodes = calloc(((*new).size),sizeof(struct HashNode*));
    clock_t t = clock();
    srand((unsigned) t);
    // (*new).rand = rand();
    int r;
    (*new).hashTable = malloc(4*sizeof(int));
    for(int i = 0; i < 4; i++) {
        r = rand();
        (*new).hashTable[i] = (r + ~(r & 1));
    }
    return new;
}

int hashKey(struct HashMap* map, int key) {
    int bits = log2((*map).size);
 //    multiply each section of 8 bits in key by a random constant specific to the map
     int val0 = (((key & 0xFF000000) * (*map).hashTable[0]))&0xFF000000;
     int val1 = (((key & 0x00FF0000) * (*map).hashTable[1]))&0x00FF0000;
     int val2 = (((key & 0x0000FF00) * (*map).hashTable[2]))&0x0000FF00;
     int val3 = (((key & 0x000000FF) * (*map).hashTable[3]))&0x000000FF;
 //    XOR each section of 8 bits together (notice that A XOR 0 = A) so the sections will concat to an int
 //    Take Modulo by getting the last |bits| bits, done by shifting -1(xFFFFFFFF) to the right by 32-bits
     int index = (val0 ^ val1 ^ val2 ^ val3) & (0xFFFFFFFF >> (32-bits));
//    int index = key & (0xFFFFFFFF >> (32-bits));
    // printf("%d\n", index);
    return index;
}

void addVal(struct HashMap* map, struct HashNode* new) {
    int index = hashKey(map,(*new).key);
    if ((*map).nodes[index] == NULL) {
        (*map).nodes[index] = new;
    } else {
        (*new).next = (*map).nodes[index];
        (*new).collisions = 1+(*(*map).nodes[index]).collisions;
        // printf("collision!, length was: %d\n", (*new).collisions);
        (*map).nodes[index] = new;
    }
}

int getVal(struct HashMap* map, int key) {
    int index = hashKey(map, key);
    struct HashNode* curNode = (*map).nodes[index];
    int curKey = (*curNode).key;
    int val;
    while(curKey != key) {
        curNode = (*curNode).next;
        curKey = (*curNode).key;
    }
    val = (*curNode).val;
    return val;
}

struct HashNode* getNode(struct HashMap* map, int key) {
    int index = hashKey(map, key);
    struct HashNode* curNode = (*map).nodes[index];
    if(curNode == NULL) {
        return NULL;
    }
    int curKey = (*curNode).key;
    while(curKey != key && (*curNode).next != NULL) {
        curNode = (*curNode).next;
        curKey = (*curNode).key;
    }
    if(curKey != key) {
        return NULL;
    }
    return curNode;
}

void removeVal(struct HashMap* map, int key) {
    int index = hashKey(map, key);
    struct HashNode* nodePtr = (*map).nodes[index];
    struct HashNode* freePtr = NULL;
    while((*nodePtr).next != NULL) {
        freePtr = nodePtr;
        nodePtr = (*nodePtr).next;
        free(freePtr);
    }
}

void freeMap(struct HashMap* map) {
    free((*map).hashTable);
    struct HashNode* nodePtr = NULL;
    struct HashNode* freePtr = NULL;
    for(int i = 0; i < (*map).size; i++) {
        nodePtr = (*map).nodes[i];
        while((*nodePtr).next != NULL) {
            freePtr = nodePtr;
            nodePtr = (*nodePtr).next;
            free(freePtr);
        }
    }
    free(nodePtr);
    free(freePtr);
    free((*map).nodes);
    free(map);
}
