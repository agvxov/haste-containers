#ifndef KSTACK_H
#define KSTACK_H

/* Example:
 *      #include <dictate.h>
 *      #include "kstack.h"
 *
 *      signed main(void) {
 *          kstack_t(int, 12) s;
 *          ks_init(s);
 *
 *          ks_push(s, 1);
 *          ks_push(s, 2);
 *          ks_push(s, 3);
 *          ks_push(s, 4);
 *
 *          while (ks_size(s)) {
 *              dictate(ks_pop(s), "\n");
 *          }
 *
 *          return 0;
 *      }
 *
 * Notes:
 *  + "et" stands for "empty top"
 */

#define kstack_t(type, max) struct { type a[max]; size_t et; }
#define ks_init(s)          ((s).et = 0)
#define ks_size(s)          ((s).et)
#define ks_push(s, i)       ((s).a[((s).et)++] = i)
#define ks_pop(s)           ((s).a[--((s).et)])
#define ks_top(s)           ((s).a[et-1])
#define ks_A(s, i)          ((s).a[i])

#endif
