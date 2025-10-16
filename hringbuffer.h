#ifndef HRINGBUFFER_H
#define HRINGBUFFER_H

#define hringbuffer_t(type, max) struct { type a[max]; size_t h; }
#define hrb_init(b)     ((b).h = 0)
#define hrb_size(b)     (sizeof((b).a) / sizeof((b).a[0]))
#define hrb_A(b, i)     ((b).a[i])
#define hrb_a(b, i)     ((b).a[(i) % hrb_size(b)])
#define hrb_top(b)      hrb_a(b, b.h + hrb_size(b) - 1)
#define hrb_push(b, i) (               \
    (b).a[(b).h] = i,                  \
    (b).h = ((b).h + 1) % hrb_size(b)  \
)

#endif
