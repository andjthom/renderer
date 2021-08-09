#ifndef STRETCHY_H
#define STRETCHY_H

#include <stdlib.h>

#define SB_FREE(b) ((b) ? free(__SB_HDR(b)), 0 : 0)
#define SB_PUSH(b, x) (__SB_FIT(b, 1) ? ((b)[__SB_LEN(b)++] = (x), __SB_LEN(b) - 1) : 0)
#define SB_LEN(b) ((b) ? __SB_LEN(b) : 0)
#define SB_CAP(b) ((b) ? __SB_CAP(b) : 0)

#define __SB_HDR(b) ((int *)(b) - 2)
#define __SB_LEN(b) (__SB_HDR(b)[0])
#define __SB_CAP(b) (__SB_HDR(b)[1])
#define __SB_FITS(b, n) ((b) == 0 || (__SB_LEN(b) + (n) >= __SB_CAP(b)))
#define __SB_FIT(b, n) (__SB_FITS(b, n) ? (__SB_GROW(b, n) != NULL) : 1)
#define __SB_GROW(b, n) ((b) = __SB_GrowImpl(b, n, sizeof(*(b))))

static void *__SB_GrowImpl(void *b, const int n, const int elem_size)
{
	int len = SB_LEN(b);
	int new_len = len + n;
	int cap = SB_CAP(b);
	int new_cap = 3 * cap / 2;
	int *result;

	if (new_cap < new_len) {
		new_cap = new_len;
	}

	new_cap = (new_cap + 15) & ~15u;

	result = realloc(b ? __SB_HDR(b) : 0, (size_t)elem_size * new_cap + 2 * sizeof(int));
	if (result == NULL) {
		return NULL;
	}

	result[0] = len;
	result[1] = new_cap;
	return result + 2;
}

#endif /* !STRETCHY_H */
