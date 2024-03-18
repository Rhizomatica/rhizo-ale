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

/**
 * @file ale_shm.c
 * @author Rafael Diniz
 * @date 14 Aug 2020
 * @brief Shared memory routines
 *
 * Shared memory routines
 *
 */


#include "ale_shm.h"

bool shm_is_created(key_t key, size_t size)
{
    int shmid = shmget(key, size, 0);

    if (shmid == -1)
    {
        return false;
    }

    return true;
}

// check of key is already not created before calling this!
bool shm_create(key_t key, size_t size)
{
    int shmid = shmget(key, size, 0666 | IPC_CREAT | IPC_EXCL);

    if (shmid == -1)
    {
        return false;
    }

    return true;
}

bool shm_destroy(key_t key, size_t size)
{
    int shmid = shmget(1, size, 0);

    if (shmid == -1)
    {
        return false;
    }

    shmctl(shmid,IPC_RMID,NULL);

    return true;
}

void *shm_attach(key_t key, size_t size)
{
    int shmid = shmget(key, size, 0);

    if (shmid == -1)
    {
        return NULL;
    }

    return shmat(shmid, NULL,0);
}

bool shm_dettach(key_t key, size_t size, void *ptr)
{
    int shmid = shmget(key, size, 0);

    if (shmid == -1)
    {
        return false;
    }

    shmdt(ptr);

    return true;
}
