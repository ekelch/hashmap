#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAP_MAX = 100;
const int MAP_KEY_MAX = 20;
const int MAP_VALUE_MAX = 255;

typedef struct Ek_LinkedList {
    char key[MAP_KEY_MAX];
    char value[MAP_VALUE_MAX];
    struct Ek_LinkedList* next;
} Ek_LinkedList;

typedef struct {
    int size;
    Ek_LinkedList* arr[MAP_MAX];
} Ek_Map;

void destroyLinkedList(Ek_LinkedList* head) {
    while (head != NULL) {
        Ek_LinkedList* temp = head;
        head = head->next;
        free(temp);
    }
}

unsigned long hash(unsigned char* k) {
    unsigned long hash = 5381;
    int c;
    while ((c = *k++)) {
        hash = (hash << 5) + hash + c;
    }
    return hash;
}

unsigned long getIndex(const Ek_Map map, char* key) {
    return hash((unsigned char*)key) % (unsigned long) map.size;
}

char* map_get(Ek_Map* map, char* key) {
    const unsigned long index = getIndex(*map, key);
    Ek_LinkedList* node = map->arr[index];
    while (strcmp(node->key, key) != 0) {
        node = node->next;
    }
    return node->value;
}

void map_put(Ek_Map* map, char* key, const char* value) {
    const unsigned long index = getIndex(*map, key);
    Ek_LinkedList* node = map->arr[index];
    if (node == NULL) {
        node = malloc(sizeof(Ek_LinkedList));
    } else {
        while (node->next != NULL) {
            node = map->arr[index]->next;
        }
        node->next = malloc(sizeof(Ek_LinkedList));
    }

    strcpy(node->key, key);
    strcpy(node->value, value);
    map->arr[index] = node;
}

void map_destroy(Ek_Map* map) {
    for (int i = 0; i < map->size; i++) {
        if (map->arr[i] != NULL) {
            destroyLinkedList(map->arr[i]);
        }
    }
}

int main(void) {
    Ek_Map map = {.size = MAP_MAX};

    char key[MAP_KEY_MAX];
    char value[MAP_VALUE_MAX];
    for (int i = 0; i < 40; i++) {
        sprintf(key, "key %d", i);
        sprintf(value, "test %d", i);
        map_put(&map, key, value);
    }

    map_destroy(&map);
    return 0;
}