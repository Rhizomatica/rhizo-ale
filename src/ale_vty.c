#include <string.h>
#include <stdlib.h>

#include <osmocom/core/utils.h>

#include <osmocom/vty/command.h>
#include <osmocom/vty/buffer.h>
#include <osmocom/vty/vty.h>

enum ale_vty_node {
	ALE_NODE = _LAST_OSMOVTY_NODE + 1
};

static struct cmd_node ale_node = {
	ALE_NODE,
	"%s(config-ale)# ",
	1,
};

DEFUN(cfg_ale, cfg_ale_cmd,
	"ale",
	"HF ALE Controller\n")
{
	vty->node = ALE_NODE;
	return CMD_SUCCESS;
}

static int config_write_ale(struct vty *vty)
{
	vty_out(vty, "ale%s", VTY_NEWLINE);
	return CMD_SUCCESS;
}

void ale_vty_init(void)
{
	install_element(CONFIG_NODE, &cfg_ale_cmd);
	install_node(&ale_node, config_write_ale);

}
