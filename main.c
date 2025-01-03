#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAP_KEY_MAX = 20;

typedef struct Ek_LinkedList {
    char key[MAP_KEY_MAX];
    void *value;
    struct Ek_LinkedList *next;
} Ek_LinkedList;

typedef struct {
    int size;
    int capacity;
    Ek_LinkedList** mapArr;
} Ek_Map;

typedef struct {
    int size;
    int capacity;
    char** arr;
} Ek_List;

Ek_List* list_new(const int capacity) {
    Ek_List* list = malloc(sizeof(Ek_List));
    list->capacity = capacity;
    list->arr = malloc(sizeof(char*) * capacity);
    return list;
}

void list_add(Ek_List* list, char* in) {
    if (list == NULL) {
        return;
    }
    if (list->size >= list->capacity) {
        char** newArr = malloc(sizeof(char*) * list->capacity * 2);
        list->capacity = list->capacity * 2;
        for (int i = 0; i < list->capacity; i++) {
            newArr[i] = list->arr[i];
        }
        newArr[list->size++] = in;
        free(list->arr);
        list->arr = newArr;
    } else {
        list->arr[list->size++] = in;
    }
}

void list_deleteIndex(Ek_List* list, const int index) {
    if (list == NULL) {
        return;
    }
    for (int i = index; i < list->size - 1; i++) {
        list->arr[i] = list->arr[i + 1];
    }
    list->arr[list->size--] = NULL;
}

void destroyLinkedList(Ek_LinkedList *head) {
    while (head != NULL) {
        Ek_LinkedList *temp = head;
        head = head->next;
        free(temp);
    }
}

unsigned long hash(unsigned char *k) {
    unsigned long hash = 5381;
    int c;
    while ((c = *k++)) {
        hash = (hash << 5) + hash + c;
    }
    return hash;
}

unsigned long getIndex(const Ek_Map map, char *key) {
    return hash((unsigned char *) key) % (unsigned long) map.capacity;
}

void* map_get(Ek_Map *map, char *key) {
    if (map == NULL || key == NULL) {
        return NULL;
    }
    const unsigned long index = getIndex(*map, key);
    Ek_LinkedList *node = map->mapArr[index];
    if (map->mapArr[index] == NULL) {
        return NULL;
    }
    while (node->next && strcmp(node->key, key) != 0) {
        node = node->next;
    }
    return node->value;
}

Ek_Map* map_new(const int cap) {
    Ek_Map* map = malloc(sizeof(Ek_Map));
    map->capacity = cap;
    map->size = 0;
    map->mapArr = malloc(sizeof(void*) * cap);
    return map;
}

void map_put(Ek_Map *map, char *key, void *value) {
    if (map == NULL || key == NULL || value == NULL) {
        return;
    }
    const unsigned long mapIndex = getIndex(*map, key);
    Ek_LinkedList *node = map->mapArr[mapIndex];
    if (node == NULL) {
        node = malloc(sizeof(Ek_LinkedList));
        strcpy(node->key, key);
        node->value = value;
        map->mapArr[mapIndex] = node;
    } else {
        Ek_LinkedList *next = malloc(sizeof(Ek_LinkedList));
        strcpy(next->key, key);
        next->value = value;
        node->next = next;
    }
    map->size++;
}

char** map_keys(const Ek_Map *map) {
    if (map == NULL) {
        return NULL;
    }
    char** keys = malloc(sizeof(char*) * map->size);
    int keypos = 0;
    for (int i = 0; i < map->size; i++) {
        Ek_LinkedList* node = map->mapArr[i];
        if (node != NULL) {
            keys[keypos++] = node->key;
            while ((node = node->next) != NULL) {
                keys[keypos++] = node->key;
            }
        }
    }
    return keys;
}

void map_destroy(const Ek_Map *map) {
    if (map == NULL) {
        return;
    }
    for (int i = 0; i < map->size; i++) {
        if (map->mapArr[i] != NULL) {
            destroyLinkedList(map->mapArr[i]);
        }
    }
}

int main(void) {
    Ek_Map* map = map_new(3);
    typedef struct {
        int x;
        int y;
    } Point;
    Point p2 = {3, 4};
    map_put(map, "p2", &p2);
    map_put(map, "p1", "hello world");
    map_put(map, "p3", "hello world");
    map_put(map, "p4", "hello world");
    Point *np = map_get(map, "p2");
    Point npc = *np;
    char** keys = map_keys(map);

    printf("%d %d\n", npc.x, npc.y);

    map_destroy(map);

    // Ek_List* list = list_new(5);
    // list_add(list, "hello world");
    // list_add(list, "hello again 1");
    // list_add(list, "hello again 2");
    // list_add(list, "hello again 3");
    // list_add(list, "hello again 4");
    // list_add(list, "hello again 4");
    // list_add(list, "hello again 5");
    //
    // list_deleteIndex(list, 0);
    // printf("%d\n", list->capacity);
    //
    // for (int i = 0; i < list->size; i++) {
    //     printf("%d: %s\n", i, list->arr[i]);
    // }
    // printf("%d\n", list->capacity);
    // free(list);
    return 0;
}
