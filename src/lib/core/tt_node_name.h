/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright 2010-2023, Tarantool AUTHORS, please see AUTHORS file.
 */
#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

enum {
	TT_NODE_NAME_LEN_MAX = 63,
	TT_NODE_NAME_SIZE_MAX = TT_NODE_NAME_LEN_MAX + 1,
};

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/**
 * Name suitable for a node visible in the network. Its format matches the
 * sub-domain label in RFC 1035, section 2.3.1
 * (https://www.rfc-editor.org/rfc/rfc1035#section-2.3.1).
 *
 * It allows to use the node name as a sub-domain and a host name.
 *
 * The limitations are: max 63 symbols (not including term 0); only lowercase
 * letters, digits, and hyphen. Can start only with a letter. Note that the
 * sub-domain name rules say that uppercase is allowed but the names are
 * case-insensitive. In Tarantool the lowercase is enforced.
 */
struct tt_node_name {
	/** Name characters. */
	char data[TT_NODE_NAME_SIZE_MAX];
};

/**
 * The name might be coming from the user land. Do not check more symbols than
 * needed even if the full name is huge somewhy.
 */
static inline size_t
tt_node_name_safelen(const char *name)
{
	return name == NULL ? 0 : strnlen(name, TT_NODE_NAME_SIZE_MAX);
}

static inline bool
tt_node_name_is_nil(const struct tt_node_name *name)
{
	return name == NULL || *name->data == 0;
}

static inline void
tt_node_name_set_nil(struct tt_node_name *name)
{
	*name->data = 0;
}

/**
 * Set the string as a new node name. Explicit len is useful when work with
 * MessagePack strings.
 */
int
tt_node_name_set_str_n(struct tt_node_name *dst, const char *src, size_t len);

/** Set the string as a new node name. */
static inline int
tt_node_name_set_str(struct tt_node_name *dst, const char *src)
{
	return tt_node_name_set_str_n(dst, src, tt_node_name_safelen(src));
}

static inline void
tt_node_name_set(struct tt_node_name *dst, const struct tt_node_name *src)
{
	int rc = tt_node_name_set_str(dst, src != NULL ? src->data : NULL);
	assert(rc == 0);
	(void)rc;
}

/** Check if two node names are equal. */
static inline bool
tt_node_name_is_equal(const struct tt_node_name *n1,
		      const struct tt_node_name *n2)
{
	if (tt_node_name_is_nil(n1))
		return tt_node_name_is_nil(n2);
	if (tt_node_name_is_nil(n2))
		return false;
	return strcmp(n1->data, n2->data) == 0;
}

static inline const char *
tt_node_name_str(const struct tt_node_name *name)
{
	if (name == NULL || *name->data == 0)
		return "<no-name>";
	return name->data;
}

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
