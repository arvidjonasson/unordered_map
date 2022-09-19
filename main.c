//
//  main.c
//  unordered_map
//
//  Created by Arvid Jonasson on 2022-09-17.
//

#include <stdio.h>
#include "unordered_map.h"


int main() {
    
    struct foo {
        short bar;
        struct foo * foobar;
    };
    
    struct foo A;
    struct foo B;
    
    A.bar = (short)5;
    A.foobar = &B;
    
    B.bar = (short)3;
    B.foobar = NULL;
    
    
    unordered_map(foo_map, char, struct foo *);
    //unordered map is created with the name foo_map, key type char, and value type struct foo *
    
    unordered_map_push(foo_map, 'a', &A);
    unordered_map_push(foo_map, 'b', &B);
    
    if(unordered_map_count(foo_map, 'b')) printf("B is in the map\n");
    if(unordered_map_count(foo_map, 'c')) printf("C is in the map\n");
    //unordered_map_count returns 1 if a value with matching key is found, otherwise 0
    
    int val = (int)(unordered_map_at(foo_map, 'a')->foobar->bar);
    //unordered_map_at returns the stored value, the returned value will be recognized as the value you specified
    
    printf("Value = %d\n", val);
    
    free(foo_map);
    //To free the memory allocated to the map. Simply use free(map). All the memory is stored at the same pointer as to avoid unnessecary malloc() calls
    
    unordered_map(foo_map2, int, struct foo);
    //It is also possible to store complex types by value instead of reference
    
    unordered_map_push(foo_map2, 15, A);
    
    A.bar = (short)60;
    //We can change the value of our copy
    
    printf("Value is %d\n", (int)(unordered_map_at(foo_map2, 15).bar));
    //But the value at the copy stored in the map will stay the same
    
    free(foo_map2);
    
    return 0;
}
