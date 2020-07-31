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

#include <stdio.h>
#include <inttypes.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>

bool shm_is_created(key_t key, size_t size);

// only creates if already not created!
bool shm_create(key_t key, size_t size);

bool shm_destroy(key_t key, size_t size);

void *shm_attach(key_t key, size_t size);

bool shm_dettach(key_t key, size_t size, void *ptr);
