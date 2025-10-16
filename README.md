# Haste containers
> Family of generic and typesafe C23 containers.

> [!NOTE] Out of convenience, we compile under C++ too.

## Syntax
Every container is a template type like macro, with an interface resembling the following:
```c
example_t(int) e;               // declare
e = ex_null;                    // initialize
ex_dothing(e);                  // perform action
ex_A(e, 1) = 1;                 // perform unsafe access (may over index)
ex_a(e, 1) = 1;                 // perform safe access (may trigger resize)
ex_foreach(auto i, e) { ; }     // iterate
/* dynamic container only */
ex_size(e);         // get number of elements
ex_add(e, 2)        // append element
ex_resize(e, 10);   // pre-allocate to 10 elements
ex_delete(e);       // release memory
```

Concrete example:
```c
hmap(char*, int) my_hashmap;
hm_init(my_hashmap);

hm_a(my_hashmap, "HW") = 1;     // OK

hm_a(my_hashmap, "HW") = "one"; // NOT OK
/* error: assignment to ‘int’ from ‘char *’ makes integer from pointer without a cast [-Wint-conversion]
 *  8 |     hm_a(my_hashmap, "HW") = "one";
 *    |                            ^
 */
```

> [!TIP] Consult the headers and tests for more examples.

## Usecase
Our datastructures have been optimized for:
* programmer convenience
* speed on a single thread
They were **not** optimized for:
* space
* multihreading
* lifetime ownership

## Contents
| lib           | Description |
| :------------ | :---------- |
| hvec          | Simple and fast vector. It never shrinks through its life-time. |
| hlist         | Linked list which uses hvec as its underlying storage. Removal only unlinks, it does not free any memory. Only destroying the list releases resources. |
| hstack        | Trivial static stack. More readable then doing it by hand and prevents dumb off-by-one-errors. |
| hringbuffer.h | Trivial static ringbuffer. |
| hmap.h        | Reasonably fast hash table. |

## Notes
Everything that is reasonable to be (or must be) is a macro.

This project was inspired by [klib](https://github.com/attractivechaos/klib)
and the [journalism of Daniel Hooper](https://danielchasehooper.com/posts/typechecked-generic-c-data-structures/).
