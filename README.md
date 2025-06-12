# kvec

This repo contains a copy of kvec from [klib](https://github.com/attractivechaos/klib).

It only exists because there is no gentoo package for klib,
I personally only utalize kvec on a regular basis
and it is significantly easier to maintain an ebuild this way.

There is also klist, which is approximately like the original klist,
except its not shit.
The original is a fragmented linked list which defines effectively the same operations as kvec.
This version is a relative linked list which uses kvec as its underlying storage
and defines removal and insertion.
