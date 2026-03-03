#include <stdio.h>

#define MAX_SIZE 100

typedef struct {
    int key;
    char data[64];
} Elem;

void set_table(Elem table[], Elem origin[], int n){
    for(int i = 0; i < n; ++i){
        table[i] = origin[i];
    }
}

void swap(Elem* a, Elem *b){
    Elem tmp = *a;
    *a = *b;
    *b = tmp;
}

void reverse(Elem test[], int n){
    for(int i = 0; i < n/2; ++i){
        swap(&test[i], &test[n-1-i]);
    }
}

void print_table(Elem table[], int n){
    printf("--------\n");
    for(int i = 0; i < n; ++i){
        printf("[%d] key: %d, Data: %s\n", i, table[i].key, table[i].data);
    }
    printf("--------\n");
}

void bubble_sort(Elem table[], int n){
    int swapped;
    for(int i = 0; i < n - 1; ++i){
        swapped = 0;
        for(int j = 0; j < n - i - 1; ++j){
            if(table[j].key > table[j+1].key){
                swap(&table[j], &table[j+1]);
                swapped = 1;
            }
        }
        if(swapped == 0) break;
    }
}

int bin_search(Elem table[], int n, int search_key){
    int low = 0;
    int high = n - 1;
    int mid = (low + high) / 2;
    while (low <= high){
        if(search_key == table[mid].key){
            return mid;
        }
        if(search_key > table[mid].key){
            low = mid + 1;
        } else {
            high = mid - 1;
        }
        mid = (low + high) / 2;
    }
    return -1;
}

int main(void){
    int n;
    printf("Введите размер таблицы: ");
    if(scanf("%d", &n) != 1 || n < 11){
        printf("Минимум 11 элементов\n");
        return 1;
    }

    Elem test[MAX_SIZE];
    Elem origin[MAX_SIZE];

    printf("Введите %d строк вида (ключ значение):\n", n);
    for(int i = 0; i < n; ++i){
        scanf("%d %s", &origin[i].key, &origin[i].data);
    }

    for(int c = 1; c <= 3; ++c){
        printf("Тест кейс: %d\n", c);
        set_table(test, origin, n);
        if(c == 1){
            bubble_sort(test, n);
        }
        else if(c == 2){
            bubble_sort(test, n);
            reverse(test, n);
        }
        printf("Изначальное состояние:\n");
        print_table(test, n);
        
        bubble_sort(test, n);

        printf("После сортировки:\n");
        print_table(test, n);
        int search_key = -1;
        printf("Введите ключ для поиска (0 - стоп): \n");
        while(search_key != 0){
            scanf("%d", &search_key);
            if(search_key == 0) {
                printf("Стоп\n");
                break;
            }
            int idx = bin_search(test, n, search_key);
            if(idx != -1){
                printf("Ключ %d найден. Значение: %s (индекс %d)\n", 
                    search_key, test[idx].data, idx);
            } else {
                printf("Ключ %d не найден\n", search_key);
            }
        }
        printf("--------\n");
    }

    return 0;
}