# kvec

This repo contains generic and typesafe C23 containers.

> [!NOTE] Out convenience, we compile under C++ too.

Everything that is reasonable to be (or must be) is a macro.

## Syntax
The syntax pattern was inspired by [klib](https://github.com/attractivechaos/klib).
```c
example_t(int) name;
ex_dothing(name);
```

## Usecase
Our datastructures have been optimized for:
* speed on a single thread
* programmer convenience
They were **not** optimized for:
* space
* multihreading

## Contents
| lib    | Description |
| :----- | :---------- |
| kvec   | Simple and fast vector. It never shrinks through its life-time. |
| klist  | Linked list which uses kvec as its underlying storage. Removal only unlinks, it does not free any memory. Only destroying the list releases resources. |
| kstack | Trivial static stack. More readable then doing it by hand and prevents dumb off-by-one-errors. |

