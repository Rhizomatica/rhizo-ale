#pragma once

#include <osmocom/core/logging.h>
#include <osmocom/core/fsm.h>

#define RHIZO_VTY_PORT_ALE 6666

#define ALE 0

extern struct osmo_fsm ale_fsm;

enum ale_event {
    ALE_E_INIT,
    ALE_E_CONN_PENDING,
    ALE_E_KEEPALIVE_PENDING,
    ALE_E_IDLE,
};

/* ale_vty.c */
void ale_vty_init(void);
