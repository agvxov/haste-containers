#ifndef KRINGBUFFER_H
#define KRINGBUFFER_H

#define kringbuffer_t(type, max) struct { type a[max]; size_t h; }
#define krb_init(b)     ((b).h = 0)
#define krb_size(b)     (sizeof((b).a) / sizeof((b).a[0]))
#define krb_A(b, i)     ((b).a[i])
#define krb_a(b, i)     ((b).a[(i) % krb_size(b)])
#define krb_top(b)      krb_a(b, b.h + krb_size(b) - 1)
#define krb_push(b, i) (               \
    (b).a[(b).h] = i,                  \
    (b).h = ((b).h + 1) % krb_size(b)  \
)

#endif
