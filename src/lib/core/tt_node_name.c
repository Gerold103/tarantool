/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright 2010-2023, Tarantool AUTHORS, please see AUTHORS file.
 */
#include "tt_node_name.h"

#include <ctype.h>

int
tt_node_name_set_str_n(struct tt_node_name *dst, const char *src, size_t len)
{
	if (len == 0) {
		*dst->data = 0;
		return 0;
	}
	if (len > TT_NODE_NAME_LEN_MAX || !isalpha(*src))
		return -1;
	const char *pos = src;
	const char *end = pos + len;
	char *out = dst->data;
	while (pos < end) {
		char c = *(pos++);
		if (!isalnum(c) && c != '-')
			return -1;
		if (tolower(c) != c)
			return -1;
		*(out++) = c;
	}
	*out = 0;
	return 0;
}
