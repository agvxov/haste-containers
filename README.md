# kvec

This repo contains generic and typesafe C23 containers.

> [!NOTE] Out of convenience, we compile under C++ too.

Everything that is reasonable to be (or must be) is a macro.

## Syntax
The syntax pattern was inspired by [klib](https://github.com/attractivechaos/klib).
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
khash(char*, int) my_hash = 
```

## Usecase
Our datastructures have been optimized for:
* speed on a single thread
* programmer convenience
They were **not** optimized for:
* space
* multihreading

## Contents
| lib           | Description |
| :------------ | :---------- |
| kvec          | Simple and fast vector. It never shrinks through its life-time. |
| klist         | Linked list which uses kvec as its underlying storage. Removal only unlinks, it does not free any memory. Only destroying the list releases resources. |
| kstack        | Trivial static stack. More readable then doing it by hand and prevents dumb off-by-one-errors. |
| kringbuffer.h | Trivial static ringbuffer. |
| khash.h       | Reasonably fast hash table. |
