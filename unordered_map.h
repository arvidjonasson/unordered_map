//
//  unordered_map.h
//  unordered_map
//
//  Created by Arvid Jonasson on 2022-09-17.
//

#ifndef unordered_map_h
#define unordered_map_h

#include <string.h> //for memset
#include <stdlib.h> //for malloc

#define _splice_key(_name) _name ## _key_type
#define _splice_val(_name) _name ## _val_type

#define unordered_map_size(_name) _name[0]
#define unordered_map_max_size(_name) _name[1]
#define unordered_map_key_size(_name) _name[2]
#define unordered_map_val_size(_name) _name[3]

#define unordered_map(_name, _key_type, _val_type)                                                      \
                                                                                                        \
    typedef _key_type _splice_key(_name);                                                               \
    typedef _val_type _splice_val(_name);                                                               \
    int* _name = malloc(sizeof(int) * 4 + (sizeof(_key_type) + sizeof(_val_type) + sizeof(int)) * 97);  \
    memset(_name, 0, sizeof(int) * 4 + (sizeof(_key_type) + sizeof(_val_type) + sizeof(int)) * 97);     \
    _name[1] = 97;                                                                                      \
    _name[2] = sizeof(_key_type);                                                                       \
    _name[3] = sizeof(_val_type);



#define unordered_map_resize(_name)                                                                                                 \
do {                                                                                                                                \
    int _next_size = unordered_map_max_size(_name) * 2 + 1;                                                                         \
                                                                                                                                    \
    while (1) {                                                                                                                     \
        if(_next_size % 2 == 0 || _next_size % 3 == 0) {++_next_size; continue;}                                                    \
                                                                                                                                    \
        for(int _i = 5; _i * _i <= _next_size; _i = _i + 6)                                                                         \
            if(_next_size % _i == 0 || _next_size % (_i + 2) == 0) {++_next_size; continue;}                                        \
                                                                                                                                    \
        break;                                                                                                                      \
    }                                                                                                                               \
                                                                                                                                    \
    int _key_size = unordered_map_key_size(_name);                                                                                  \
    int _val_size = unordered_map_val_size(_name);                                                                                  \
    int _max_size = unordered_map_max_size(_name);                                                                                  \
                                                                                                                                    \
                                                                                                                                    \
    int* _next_map = malloc(sizeof(int) * 4 + (_key_size + _val_size + sizeof(int)) * _next_size);                                  \
    memset(_next_map, 0, sizeof(int) * 4 + (_key_size + _val_size + sizeof(int)) * _next_size);                                     \
                                                                                                                                    \
    _next_map[1] = _next_size;                                                                                                      \
    _next_map[2] = _key_size;                                                                                                       \
    _next_map[3] = _val_size;                                                                                                       \
                                                                                                                                    \
    int* _usedptr = _name + 4;                                                                                                      \
    _name ## _key_type * _keyptr = (_name ## _key_type *)(_usedptr + _max_size);                                                    \
    _name ## _val_type * _valptr = (_name ## _val_type *)(_keyptr + _max_size);                                                     \
                                                                                                                                    \
    int* _next_usedptr = _next_map + 4;                                                                                             \
    _name ## _key_type * _next_keyptr = (_name ## _key_type *)(_next_usedptr + _next_size);                                         \
    _name ## _val_type * _next_valptr = (_name ## _val_type *)(_next_keyptr + _next_size);                                          \
                                                                                                                                    \
    for(int _i = 0; _i < _max_size; ++_i) {                                                                                         \
        if(_usedptr[_i] == 1) {                                                                                                     \
            _name ## _key_type _key = _keyptr[_i];                                                                                  \
            int _key_hash = comp_hash(_next_map, _key);                                                                             \
                                                                                                                                    \
            int _j = 0;                                                                                                             \
            int _mod = _next_size;                                                                                                  \
            while(_next_usedptr[ (_key_hash + _j * _j) % _mod ] != 0 && _next_keyptr[ (_key_hash + _j * _j) % _mod ] != _key)       \
                ++_j;                                                                                                               \
                                                                                                                                    \
            _next_usedptr[ (_key_hash + _j * _j) % _mod ] = 1;                                                                      \
            _next_keyptr[ (_key_hash + _j * _j) % _mod ] = _keyptr[_i];                                                             \
            _next_valptr[ (_key_hash + _j * _j) % _mod ] = _valptr[_i];                                                             \
        }                                                                                                                           \
    }                                                                                                                               \
                                                                                                                                    \
    free(_name);                                                                                                                    \
    _name = _next_map;                                                                                                              \
    _next_map = NULL;                                                                                                               \
} while(0)




#define unordered_map_push(_name, _key, _val)                                                               \
do {                                                                                                        \
    if(unordered_map_size(_name) > unordered_map_max_size(_name) / 2)                                       \
        unordered_map_resize(_name);                                                                        \
                                                                                                            \
    int _key_hash = comp_hash(_name, _key);                                                                 \
    int* _usedptr = _name + 4;                                                                              \
    _name ## _key_type * _keyptr = (_name ## _key_type *)(_usedptr + unordered_map_max_size(_name));        \
    _name ## _val_type * _valptr = (_name ## _val_type *)(_keyptr + unordered_map_max_size(_name));         \
                                                                                                            \
    int u_map_push = 0;                                                                                     \
    int _mod = unordered_map_max_size(_name);                                                               \
    while(!(_usedptr[ (_key_hash + u_map_push * u_map_push) % _mod ] == 0                                   \
        || _keyptr[ (_key_hash + u_map_push * u_map_push) % _mod ] == _key))                                \
        ++u_map_push;                                                                                       \
                                                                                                            \
    if(_usedptr[ (_key_hash + u_map_push * u_map_push) % _mod ] == 0) {                                     \
        unordered_map_size(_name) += 1;                                                                     \
        _usedptr[ (_key_hash + u_map_push * u_map_push) % _mod ] = 1;                                       \
    }                                                                                                       \
    _keyptr[ (_key_hash + u_map_push * u_map_push) % _mod ] = _key;                                         \
    _valptr[ (_key_hash + u_map_push * u_map_push) % _mod ] = _val;                                         \
} while(0)

#define comp_hash(_name, _key) ((abs(_key) + 1) % unordered_map_max_size(_name))

#define unordered_map_at(_name, _key)                                                                               \
({                                                                                                                  \
    int _key_hash = comp_hash(_name, _key);                                                                         \
                                                                                                                    \
    int* _usedptr = _name + 4;                                                                                      \
    _name ## _key_type * _keyptr = (_name ## _key_type *)(_usedptr + unordered_map_max_size(_name));                \
    _name ## _val_type * _valptr = (_name ## _val_type *)(_keyptr + unordered_map_max_size(_name));                 \
                                                                                                                    \
    int _i = 0;                                                                                                     \
    int _mod = unordered_map_max_size(_name);                                                                       \
    while(_usedptr[ (_key_hash + _i * _i) % _mod ] == 1 && _keyptr[ (_key_hash + _i * _i) % _mod ] != _key) {       \
        ++_i;                                                                                                       \
    }                                                                                                               \
    (_valptr[ (_key_hash + _i * _i) % _mod ]);                                                                      \
})

#define unordered_map_count(_name, _key)\
({                                                                                                                  \
    int _key_hash = comp_hash(_name, _key);                                                                         \
                                                                                                                    \
    int* _usedptr = _name + 4;                                                                                      \
    _name ## _key_type * _keyptr = (_name ## _key_type *)(_usedptr + unordered_map_max_size(_name));                \
                                                                                                                    \
    int _i = 0;                                                                                                     \
    int _mod = unordered_map_max_size(_name);                                                                       \
    while(_usedptr[ (_key_hash + _i * _i) % _mod ] == 1 && _keyptr[ (_key_hash + _i * _i) % _mod ] != _key) {       \
        ++_i;                                                                                                       \
    }                                                                                                               \
    (_key == _keyptr[ (_key_hash + _i * _i) % _mod ]);                                                              \
})

#define unordered_map_free(_name) free(_name)


#define unordered_map_erase(_name, _key) unordered_map_push(_name, _key, 0)




#endif /* unordered_map_h */
