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

#include <osmocom/core/fsm.h>

#include "internal.h"

#define S(x)	(1 << (x))

// timer definitions here...
#define T_KEEPALIVE			1
#define T_KEEPALIVE_SECS		30

enum ale_state {
	ALE_S_INIT,
	ALE_S_CONN_PENDING,
	ALE_S_KEEPALIVE_PENDING,
	ALE_S_IDLE,
};


static const struct value_string ale_event_names[] = {
	{ ALE_E_INIT, "ALE init" },
	{ ALE_E_CONN_PENDING, "ALE Conn Pending" },
	{ ALE_E_KEEPALIVE_PENDING, "ALE Keep-Alive Pending" },
	{ ALE_E_IDLE, "ALE Idle" },
	{ 0, NULL }
};


static void ale_init(struct osmo_fsm_inst *fi, uint32_t event, void *data)
{
    osmo_fsm_inst_state_chg(fi, ALE_S_IDLE, 0, 0);
}



 // example code from osmo-cbc
static void ale_event_handler(struct osmo_fsm_inst *fi, uint32_t event, void *data)
{
	switch (event) {
            // TODO
	case ALE_E_INIT:
		osmo_fsm_inst_state_chg(fi, ALE_S_IDLE, 0, 0);
		break;
	default:
		OSMO_ASSERT(0);
	}
}

static int ale_conn_cb(struct osmo_fsm_inst *fi)
{
    return 1;
}

static const struct osmo_fsm_state ale_fsm_states[] = {
	[ALE_S_INIT] = {
		.name = "INIT",
		.in_event_mask = 0,
		.out_state_mask = S(ALE_S_IDLE),
		.action = ale_init,
	},
};

struct osmo_fsm ale_fsm = {
	.name = "ALE-HF-Controller",
	.states = ale_fsm_states,
	.num_states = ARRAY_SIZE(ale_fsm_states),
//	.timer_cb = ale_fsm_timer_cb,
	.log_subsys = ALE,
	.event_names = ale_event_names,
};

static __attribute__((constructor)) void on_dso_load_cbsp_srv_fsm(void)
{
	osmo_fsm_register(&ale_fsm);
}

