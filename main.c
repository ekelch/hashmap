#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAP_MAX = 10;
const int MAP_KEY_MAX = 20;

typedef struct Ek_LinkedList {
    char key[MAP_KEY_MAX];
    void* value;
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

void* map_get(Ek_Map* map, char* key) {
    const unsigned long index = getIndex(*map, key);
    Ek_LinkedList* node = map->arr[index];
    if (map->arr[index] == NULL) {
        return NULL;
    }
    while (node->next && strcmp(node->key, key) != 0) {
        node = node->next;
    }
    return node->value;
}

void map_put(Ek_Map* map, char* key, void* value) {
    const unsigned long mapIndex = getIndex(*map, key);
    Ek_LinkedList* node = map->arr[mapIndex];
    if (node == NULL) {
        node = malloc(sizeof(Ek_LinkedList));
        strcpy(node->key, key);
        node->value = value;
    } else {
        Ek_LinkedList* next = malloc(sizeof(Ek_LinkedList));
        strcpy(next->key, key);
        next->value = value;
        node->next = next;
    }
    map->arr[mapIndex] = node;
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

    typedef struct {
        int x;
        int y;
    } Point;

    Point p2 = {3,4};

    map_put(&map, "p2", &p2);
    Point* np = map_get(&map, "p2");
    Point npc = *np;
    printf("%d %d\n", npc.x, npc.y);

    map_destroy(&map);
    return 0;
}