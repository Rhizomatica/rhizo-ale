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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>

#include <osmocom/core/stats.h>
#include <osmocom/core/select.h>
#include <osmocom/core/msgb.h>
#include <osmocom/core/signal.h>
#include <osmocom/core/talloc.h>
#include <osmocom/core/utils.h>
#include <osmocom/core/logging.h>
#include <osmocom/core/application.h>

#include <osmocom/vty/vty.h>
#include <osmocom/vty/stats.h>
#include <osmocom/vty/command.h>
#include <osmocom/vty/ports.h>
#include <osmocom/vty/telnet_interface.h>
#include <osmocom/vty/logging.h>
#include <osmocom/vty/misc.h>

#include "internal.h"

static void *tall_ale_ctx;

static const struct log_info_cat log_info_cat[] = {
    [ALE] = {
        .name = "ALE",
        .description = "Rhizomatica HF ALE System",
        .color = "\033[1;31m",
        .enabled = 1,
        .loglevel = LOGL_NOTICE,
    },
};

static const struct log_info log_info = {
    .cat = log_info_cat,
    .num_cat = ARRAY_SIZE(log_info_cat),
};

static const char ale_copyright[] =
        "Copyright (C) 2020 by Rafael Diniz <rafael@rhizomatica.org>\r\n"
        "License AGPLv3+: GNU Affero GPL Version 3 or later <http://gnu.org/licenses/agpl-3.0.html>\r\n"
        "This is free software: you are free ot change and redistribute it.\r\n"
        "There is NO WARRANTY, to the extent permitted by law.\r\n\r\n"
        "Free Software lives by contribution.  If you use this, please contribute!\r\n";

static struct vty_app_info vty_info = {
    .name = "RhizoALE",
    .copyright = ale_copyright,
    .version = PACKAGE_VERSION,
};

static struct {
    bool daemonize;
    const char *config_file;
    int vty_port;
} cmdline_config = {
    .daemonize = false,
    .config_file = "rhizo-ale.cfg",
    .vty_port = RHIZO_VTY_PORT_ALE,
};

static void print_help(void)
{
    printf( "Options:\n"
            "  -h	--help		This text\n"
            "  -c	--config-file 	Specify the filename of the config "
            "file\n"
            "  -v	--vty-port 	Specify the vty port (Default: %d) "
            "\n"
            "  -D	--daemonize	Fork the process into a background "
            "daemon\n"
            "  -V	--version	Show current version\n", RHIZO_VTY_PORT_ALE
        );
}

static void handle_options(int argc, char **argv)
{
    while (1) {
        int option_index = 0, c;
        static const struct option long_options[] = {
            { "help", 0, 0, 'h' },
            { "daemonize", 0, 0, 'D' },
            { "config-file", 1, 0, 'c' },
            { "vty-port", 1, 0, 'v' },
            { "version", 0, 0, 'V' },
            { NULL, 0, 0, 0 }
        };

        c = getopt_long(argc, argv, "hDc:v:V", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'h':
            print_help();
            exit(0);
            break;
        case 'D':
            cmdline_config.daemonize = true;
            break;
        case 'c':
            cmdline_config.config_file = optarg;
            break;
        case 'v':
            cmdline_config.vty_port = atoi(optarg);
            break;
        case 'V':
            print_version(1);
            exit(0);
            break;
        default:
            fprintf(stderr, "Error in command line options. Exiting\n");
            exit(1);
            break;
        }
    }

    if (argc > optind) {
        fprintf(stderr, "Unsupported positional arguments on command line\n");
        exit(2);
    }
}

static void signal_handler(int signal)
{
    fprintf(stdout, "signal %d received\n", signal);

    switch (signal){
    case SIGUSR1:
        talloc_report(tall_vty_ctx, stderr);
        talloc_report_full(tall_ale_ctx, stderr);
        break;
    case SIGUSR2:
        talloc_report_full(tall_vty_ctx, stderr);
        break;
    default:
        break;
    }
}

int main(int argc, char **argv)
{
    int rc;

    tall_ale_ctx = talloc_named_const(NULL, 1, "rhizo-ale");
    msgb_talloc_ctx_init(tall_ale_ctx, 0);
    osmo_init_logging2(tall_ale_ctx, &log_info);
    log_enable_multithread();
    osmo_stats_init(tall_ale_ctx);
    vty_init(&vty_info);

    ale_vty_init();

    handle_options(argc, argv);

    logging_vty_add_cmds();
    osmo_fsm_vty_add_cmds();

    rc = vty_read_config_file(cmdline_config.config_file, NULL);
    if (rc < 0) {
        fprintf(stderr, "Failed ot parse the config file '%s'\n",
                cmdline_config.config_file);
        exit(1);
    }

    rc = telnet_init_dynif(tall_ale_ctx, NULL, vty_get_bind_addr(), cmdline_config.vty_port);
    if (rc < 0) {
        perror("Error binding VTY port\n");
        exit(1);
    }

    signal(SIGUSR1, &signal_handler);
    signal(SIGUSR2, &signal_handler);
    osmo_init_ignore_signals();

    if (cmdline_config.daemonize) {
        rc = osmo_daemonize();
        if (rc < 0) {
            perror("Error during daemonize");
            exit(1);
        }
    }

    while (1) {
        rc = osmo_select_main(0);
        if (rc < 0)
            exit(3);
    }
}
