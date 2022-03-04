/*
 * m_pedit_eth.c	packet editor: ETH header
 *
 *		This program is free software; you can distribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:  Amir Vadai (amir@vadai.me)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "utils.h"
#include "tc_util.h"
#include "m_pedit.h"

static int
parse_eth(int *argc_p, char ***argv_p,
	  struct m_pedit_sel *sel, struct m_pedit_key *tkey)
{
	int res = -1;
	int argc = *argc_p;
	char **argv = *argv_p;

	if (argc < 2)
		return -1;

	if (!sel->extended)
		return -1;

	tkey->htype = TCA_PEDIT_KEY_EX_HDR_TYPE_ETH;

	if (strcmp(*argv, "type") == 0) {
		NEXT_ARG();
		tkey->off = 12;
		res = parse_cmd(&argc, &argv, 2, TU32, RU16, sel, tkey);
		goto done;
	}

	if (strcmp(*argv, "dst") == 0) {
		NEXT_ARG();
		tkey->off = 0;
		res = parse_cmd(&argc, &argv, 6, TMAC, RU32, sel, tkey);
		goto done;
	}

	if (strcmp(*argv, "src") == 0) {
		NEXT_ARG();
		tkey->off = 6;
		res = parse_cmd(&argc, &argv, 6, TMAC, RU32, sel, tkey);
		goto done;
	}

	return -1;

done:
	*argc_p = argc;
	*argv_p = argv;
	return res;
}

struct m_pedit_util p_pedit_eth = {
	NULL,
	"eth",
	parse_eth,
};
