#ifndef KRINGBUFFER_H
#define KRINGBUFFER_H

#define kringbuffer_t(type, max) struct { type a[max]; size_t cur; }
#define krb_init(b)     ((b).cur = 0)
#define krb_cap(b)      (sizeof((b).a) / sizeof((b).a[0]))
#define krb_shr(b)      (((b).cur == krb_cap(b)) ? ((b).et = 0)            : ((b).et += 1))
#define krb_shl(b)      (((b).cur == 0)          ? ((b).et = krb_cap(b)-1) : ((b).et -= 1))
#define krb_top(b)      ((b).a[(b).cur])
#define krb_push(b, i) ( \
    krb_top(b) = i,      \
    krb_shr(),           \
)

#endif
