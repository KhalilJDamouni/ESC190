#include "lab2.h"
#include <string.h>
#include <math.h>

void print_tree(float arr[], int n) {
    int levels = 0;
    int j = n;
    while(j != 0)
    {
        j /= 2;
        levels++;
    }
    
    int start_spacing[levels];
    start_spacing[levels - 1] = 0;
    
    int mid_spacing[levels];
    mid_spacing[levels - 1] = 4;

    for(int i = levels - 2; i >= 0; --i)
    {
        int current_level = i + 1;
        mid_spacing[i] = mid_spacing[i + 1] * 2 + 1;
        start_spacing[i] = mid_spacing[i + 1] - 2;
    }

    int current_level;
    int next = 1;
    int counter = 1;
    current_level = 0;
    for(int i = 0; i < n; ++i)
    {
        if(i + 1 == counter)
        {
            counter *= 2;
            printf("\n");
            for(int d = 0; d < start_spacing[current_level]; ++d)
                printf(" ");
            current_level += 1;

        } 
        printf("%0.0f", arr[i]);
        for(int d = 0; d < mid_spacing[current_level - 1]; ++d)
                printf(" ");

        if(2 * i + 2 < n)
        {
            int digs = (int) log10(arr[2 * i + 2]) + 1;
            for(int d = 1; d < digs; ++d)
                printf(" ");
        }
        if(2 * i + 1 < n)
        {
            int digs = (int) log10(arr[2 * i + 2]) + 1;

            for(int d = 1; d < digs; ++d)
                printf(" ");
        }
    }
    printf("\n");
}

float get_parent_value(float arr[], int n, int index) {
    int new_index = (index - 1) / 2;
    if(new_index >= n || index == 0 || index >= n)
        return -1;
    else
        return arr[new_index];
}

float get_left_value(float arr[], int n, int index) {
    int new_index = 2 * index + 1;
    if(new_index >= n || index >= n)
        return -1;
    else
        return arr[new_index];
}

float get_right_value(float arr[], int n, int index) {
    int new_index = 2 * index + 2;
    if(new_index >= n || index >= n)
        return -1;
    else
        return arr[new_index];
}

int is_max_heap(float arr[], int n) {
    int i = 0;
    while(i < n)
    {
        if(2 * i + 1 < n)
            if(arr[2 * i + 1] > arr[i])
                return 0;
        if(2 * i + 2 < n)
            if(arr[2 * i + 2] > arr[i])
                return 0;
        ++i;
    }

    return 1;
}

void heapify(float arr[], int n) {    
    for(int i = 0; i < n - 1; ++i)
    {
        for(int d = 0; d < n - 1; ++d)
        {
            if(arr[d] < arr[d + 1])
            {
                float temp = arr[d];
                arr[d] = arr[d + 1];
                arr[d + 1] = temp;
            }
        }

    }

}

void heapsort(float arr[], int n) {
    for(int i = 0; i < n - 1; ++i)
    {
        for(int d = 0; d < n -1; ++d)
        {
            if(arr[d] > arr[d + 1])
            {
                float temp = arr[d];
                arr[d] = arr[d + 1];
                arr[d + 1] = temp;
            }
        }

    }
}

float find_most_common_element(float arr[], int n) {
    int top_count = 0;
    float top_number;
    int count;
    for(int i = 0; i < n; ++i)
    {
        count = 1;
        for (int d = 0; d < n; ++d)
        {
            if(i != d && arr[i] == arr[d])
            {
                ++count;
            }
        }
        if(count > top_count)
        {
            top_count = count;
            top_number = arr[i];
        }

    }
    return top_number;
}



