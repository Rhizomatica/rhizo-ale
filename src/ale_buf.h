/* Rhizomatica ALE HF controller */

/* (C) 2020 by Rafael Diniz <rafael@rhizomatica.org>
 * All Rights Reserved
 *
 * SPDX-License-Identifier: AGPL-3.0+
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <stdbool.h>

struct circular_buf_t_aux {
    size_t head;
    size_t tail;
    size_t max; //of the buffer
    bool full;
    atomic_flag acquire;
};

struct circular_buf_t {
    struct circular_buf_t_aux *internal;
    uint8_t *buffer;
};

/// Opaque circular buffer structure
//typedef struct circular_buf_t circular_buf_t;

/// Handle type, the way users interact with the API
typedef struct circular_buf_t* cbuf_handle_t;

/// Pass in a storage buffer and size, returns a circular buffer handle
/// Requires: buffer is not NULL, size > 0
/// Ensures: cbuf has been created and is returned in an empty state
cbuf_handle_t circular_buf_init(uint8_t *buffer, size_t size);

cbuf_handle_t circular_buf_init_shm(size_t size, key_t key);

cbuf_handle_t circular_buf_connect_shm(size_t size, key_t key);

/// Free a circular buffer structure
/// Requires: cbuf is valid and created by circular_buf_init
/// Does not free data buffer; owner is responsible for that
void circular_buf_free(cbuf_handle_t cbuf);

void circular_buf_free_shm(cbuf_handle_t cbuf, size_t size, key_t key);

/// Reset the circular buffer to empty, head == tail. Data not cleared
/// Requires: cbuf is valid and created by circular_buf_init
void circular_buf_reset(cbuf_handle_t cbuf);

/// Put a value in the buffer
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns 0 on success, -1 if buffer is full
int circular_buf_put(cbuf_handle_t cbuf, uint8_t data);

/// Retrieve a value from the buffer
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns 0 on success, -1 if the buffer is empty
int circular_buf_get(cbuf_handle_t cbuf, uint8_t * data);

/// CHecks if the buffer is empty
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns true if the buffer is empty
bool circular_buf_empty(cbuf_handle_t cbuf);

/// Checks if the buffer is full
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns true if the buffer is full
bool circular_buf_full(cbuf_handle_t cbuf);

/// Check the capacity of the buffer
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns the maximum capacity of the buffer
size_t circular_buf_capacity(cbuf_handle_t cbuf);

/// Check the number of elements stored in the buffer
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns the current number of elements in the buffer
size_t circular_buf_size(cbuf_handle_t cbuf);

size_t circular_buf_free_size(cbuf_handle_t cbuf);

/// Check the number of free elements in the buffer
/// Requires: cbuf is valid and created by circular_buf_init
/// Returns the current number of free elements in the buffer
size_t circular_buf_free_size(cbuf_handle_t cbuf);

int circular_buf_get_range(cbuf_handle_t cbuf, uint8_t *data, size_t len);

int circular_buf_put_range(cbuf_handle_t cbuf, uint8_t * data, size_t len);
