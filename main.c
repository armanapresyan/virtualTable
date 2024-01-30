#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Serializable Serializable;
typedef struct Person Person;
typedef struct Serializable_Vtbl Serializable_Vtbl;
typedef struct Person_Vtbl Person_Vtbl;
typedef struct TI TI;

struct TI {
    char type_name[50];
    char bases[50];
};

struct Serializable_Vtbl {
    struct TI* type_info;
    char* (*serialize)(const Serializable* serializable);
    void (*deserialize)(const Serializable* serializable);
};

struct Serializable {
    struct Serializable_Vtbl* vtbl;
};

char* serialize_Serialiazable(const Serializable* serializable) {
   printf("This is pure virtual function\n");
   return NULL;
}

void deserialize_Serialiazable(const Serializable* serialize) {
    printf("This is pure virtual function\n");
}

struct Person_Vtbl {
    struct TI* type_info;
    char* (*serialize)(const Person* person);
    void (*deserialize)(Person* person, const char* str);
};

struct Person {
    struct Serializable subobj;
    struct Person_Vtbl* vtbl;
    int m_age;
    char m_name[50];
};

char* serialize_Person(const Person* person) {
    char* serializeData = (char*)malloc(100);
    if (serializeData == NULL) {
        return NULL; // Handle memory allocation failure
    }

    strcpy(serializeData, person->m_name);
    strcat(serializeData, "|");
    char ageStr[10];
    snprintf(ageStr, sizeof(ageStr), "%d", person->m_age);
    strcat(serializeData, ageStr);

    return serializeData;
}

void deserialize_person(Person* person, const char* str) {
    const char* pos = strchr(str, '|');
    if (pos != NULL) {
        int nameLen = pos - str;
        strncpy(person->m_name, str, nameLen);
        person->m_name[nameLen] = '\0';
        int age = atoi(pos + 1);
        person->m_age = age;
    } else {
        person->m_name[0] = '\0';
        person->m_age = 0;
    }
}

void init( Person* person, int age, const char* name) {
    person->m_age = age;
    strcpy(person->m_name, name);
}

int main() {
    TI* type = (TI*)malloc(sizeof(TI));
    strcpy(type->type_name, "Person");
    strcpy(type->bases, "Serializable");
    Person* person = (Person*)malloc(sizeof(Person));
    Person_Vtbl* vt = (Person_Vtbl*)malloc(sizeof(Person_Vtbl));
    vt->type_info = type;
    vt->serialize = serialize_Person;
    vt->deserialize = deserialize_person;
    init(person, 45, "Alica");
    person->vtbl = vt;
    char* str = person->vtbl->serialize(person);
    printf("%s", str);
    person->vtbl->deserialize(person, str);
    free(type);
    free(vt);
    free(str);

    return 0;
}
