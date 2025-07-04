# kvec

This repo contains generic and typesafe C containers using macros.
The syntax pattern is inspired by [klib](https://github.com/attractivechaos/klib)
and is as follows:
```c
example_t(int) name;
ex_dothing(name);
```

Our datastructures have been optimized for:
* speed on a single thread
* programmer convenience
They were **not** optimized for:
* space
* multihreading

## Contents
### Kvec
Klib's original kvec.
It is a simple and fast vector.
It never shrinks through its life-time.

### Klist
Approximately like the original klist, except its not shit.

The original is a fragmented linked list which defines effectively the same operations as kvec.

This version is a relative linked list which uses kvec as its underlying storage
and defines removal and insertion.

Removal only unlinks, it does not free any memory.
Only destroying the list releases resources.

### Kstack
Very trivial static stack.
Slightly more readable then doing it by hand and prevents dumb off-by-one-errors.
