// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The Music Player Daemon Project

#include "sticker.h"
#include "util.h"

#include <mpd/client.h>

#include <string.h>
#include <stdio.h>

static void
recv_print_stickers(struct mpd_connection *connection)
{
	struct mpd_pair *pair;

	while ((pair = mpd_recv_sticker(connection)) != NULL) {
		printf("%s=%s\n", pair->name, pair->value);
		mpd_return_sticker(connection, pair);
	}
}

static void
recv_print_stickers2(struct mpd_connection *connection)
{
	struct mpd_pair *pair;
	bool first = true;

	while ((pair = mpd_recv_pair(connection)) != NULL) {
		if (strcmp(pair->name, "file") == 0) {
			if (first)
				first = false;
			else
				putchar('\n');
			printf("%s:", pair->value);
		} else if (!first && strcmp(pair->name, "sticker") == 0)
			printf(" %s", pair->value);

		mpd_return_pair(connection, pair);
	}

	if (!first)
		putchar('\n');
}

int
cmd_sticker(int argc, char **argv, struct mpd_connection *conn)
{
	if(!strcmp(argv[1], "set"))
	{
		if(argc < 4)
		{
			fputs("syntax: sticker <uri> set <name> <value>\n", stderr);
			return 0;
		}

		mpd_send_sticker_set(conn, "song", argv[0], argv[2], argv[3]);
		my_finishCommand(conn);
		return 0;
	}
	else if(!strcmp(argv[1], "get"))
	{
		if(argc < 3)
		{
			fputs("syntax: sticker <uri> get <name>\n", stderr);
			return 0;
		}

		mpd_send_sticker_get(conn, "song", argv[0], argv[2]);
		recv_print_stickers(conn);
		my_finishCommand(conn);
	}
	else if(!strcmp(argv[1], "find"))
	{
		if(argc < 3)
		{
			fputs("syntax: sticker <dir> find <name>\n", stderr);
			return 0;
		}

		mpd_send_sticker_find(conn, "song", argv[0], argv[2]);
		recv_print_stickers2(conn);
		my_finishCommand(conn);
	} else if (strcmp(argv[1], "delete") == 0) {
		if(argc < 2)
		{
			fputs("syntax: sticker <uri> delete [name]\n", stderr);
			return 0;
		}

		mpd_send_sticker_delete(conn, "song", argv[0], argc > 2 ? argv[2] : NULL);
		my_finishCommand(conn);
	}
	else if(!strcmp(argv[1], "list"))
	{
		if(argc < 2)
		{
			fputs("syntax: sticker <uri> list\n", stderr);
			return 0;
		}

		mpd_send_sticker_list(conn, "song", argv[0]);
		recv_print_stickers(conn);
		my_finishCommand(conn);
	}
	else
		fputs("error: unknown command.\n", stderr);

	return 0;
}

int
cmd_stickernames(int argc, char **argv, struct mpd_connection *conn)
{
	(void)argc; // silence warning about unused argument
	(void)argv; // silence warning about unused argument
	struct mpd_pair *pair;

	mpd_send_stickernames(conn);

	while ((pair = mpd_recv_pair(conn)) != NULL) {

		if (!strcmp(pair->name, "name")) {
			printf("%s\n", pair->value);
		}
		mpd_return_pair(conn, pair);
	}

	my_finishCommand(conn);
	return 0;
}

int
cmd_stickertypes(int argc, char **argv, struct mpd_connection *conn)
{
	(void)argc; // silence warning about unused argument
	(void)argv; // silence warning about unused argument
	struct mpd_pair *pair;

	mpd_send_stickertypes(conn);

	while ((pair = mpd_recv_pair(conn)) != NULL) {

		if (!strcmp(pair->name, "stickertype")) {
			printf("%s\n", pair->value);
		}
		mpd_return_pair(conn, pair);
	}

	my_finishCommand(conn);
	return 0;
}

int
cmd_stickernamestypes(int argc, char **argv, struct mpd_connection *conn)
{
	(void)argc; // silence warning about unused argument
	const char* type = argv[0];
	struct mpd_pair *pair;

	mpd_send_stickernamestypes(conn, type);

	while ((pair = mpd_recv_pair(conn)) != NULL) {

		if (!strcmp(pair->name, "name")) {
			printf("%s\n", pair->value);
		}
		mpd_return_pair(conn, pair);
	}

	my_finishCommand(conn);
	return 0;
}
