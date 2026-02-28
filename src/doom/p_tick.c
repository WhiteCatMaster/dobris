//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Archiving: SaveGame I/O.
//	Thinker, Ticker.
//


#include "z_zone.h"
#include "p_local.h"
#include <stdio.h>
#include "doomstat.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

static int udp_sock = -1;
static struct sockaddr_in udp_addr;

static int debug_counter = 0;

typedef struct
{
    int x;
    int y;
    int angle;
    int health;
} NetPlayerState;

typedef struct
{
    int id;
    int x;
    int y;
    int type;
    int health;
} NetEntityState;
int	leveltime;

//
// THINKERS
// All thinkers should be allocated by Z_Malloc
// so they can be operated on uniformly.
// The actual structures will vary in size,
// but the first element must be thinker_t.
//



// Both the head and tail of the thinker list.
thinker_t	thinkercap;


//
// P_InitThinkers
//
void P_InitThinkers (void)
{
    thinkercap.prev = thinkercap.next  = &thinkercap;
}




//
// P_AddThinker
// Adds a new thinker at the end of the list.
//
void P_AddThinker (thinker_t* thinker)
{
    thinkercap.prev->next = thinker;
    thinker->next = &thinkercap;
    thinker->prev = thinkercap.prev;
    thinkercap.prev = thinker;
}



//
// P_RemoveThinker
// Deallocation is lazy -- it will not actually be freed
// until its thinking turn comes up.
//
void P_RemoveThinker (thinker_t* thinker)
{
  // FIXME: NOP.
  thinker->function.acv = (actionf_v)(-1);
}



//
// P_AllocateThinker
// Allocates memory and adds a new thinker at the end of the list.
//
void P_AllocateThinker (thinker_t*	thinker)
{
}



//
// P_RunThinkers
//
void P_RunThinkers (void)
{
    thinker_t *currentthinker, *nextthinker;

    currentthinker = thinkercap.next;
    while (currentthinker != &thinkercap)
    {
	if ( currentthinker->function.acv == (actionf_v)(-1) )
	{
	    // time to remove it
            nextthinker = currentthinker->next;
	    currentthinker->next->prev = currentthinker->prev;
	    currentthinker->prev->next = currentthinker->next;
	    Z_Free(currentthinker);
	}
	else
	{
	    if (currentthinker->function.acp1)
		currentthinker->function.acp1 (currentthinker);
            nextthinker = currentthinker->next;
	}
	currentthinker = nextthinker;
    }
}



//
// P_Ticker
//

void P_Ticker (void)
{
    int i;

    // run the tic
    if (paused)
        return;

    // pause if in menu and at least one tic has been run
    if (!netgame
        && menuactive
        && !demoplayback
        && players[consoleplayer].viewz != 1)
    {
        return;
    }

    // ---- LÓGICA ORIGINAL DEL JUEGO ----

    for (i = 0; i < MAXPLAYERS; i++)
        if (playeringame[i])
            P_PlayerThink(&players[i]);

    P_RunThinkers();
    P_UpdateSpecials();
    P_RespawnSpecials();

    // ---- INICIALIZAR SOCKET UNA SOLA VEZ ----

    if (udp_sock == -1)
    {
        udp_sock = socket(AF_INET, SOCK_DGRAM, 0);

        memset(&udp_addr, 0, sizeof(udp_addr));
        udp_addr.sin_family = AF_INET;
        udp_addr.sin_port = htons(5000);           // Puerto destino
        udp_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Enviar a localhost
    }

    // ---- ENVIAR ESTADO 5 VECES POR SEGUNDO ----

    static int send_counter = 0;
    send_counter++;

    if (send_counter % 7 == 0)   // 35 ticks / 7 ≈ 5 envíos por segundo
    {
        if (players[0].mo != NULL)
        {
            char buffer[2048];
            int offset = 0;

            thinker_t *th;

            // --- PLAYER ---
            int16_t px = players[0].mo->x >> 16;
            int16_t py = players[0].mo->y >> 16;
            int16_t hp = players[0].health;

            memcpy(buffer + offset, &px, sizeof(int16_t)); offset += sizeof(int16_t);
            memcpy(buffer + offset, &py, sizeof(int16_t)); offset += sizeof(int16_t);
            memcpy(buffer + offset, &hp, sizeof(int16_t)); offset += sizeof(int16_t);

            // --- ENTITY COUNT PLACEHOLDER ---
            int16_t entity_count = 0;
            int entity_count_offset = offset;
            offset += sizeof(int16_t);

            // --- ENTITIES ---
            for (th = thinkercap.next; th != &thinkercap; th = th->next)
            {
                if (th->function.acp1 == (actionf_p1) P_MobjThinker)
                {
                    mobj_t *mo = (mobj_t *) th;

                    if (mo == players[0].mo)
                        continue;

                    if (mo->health <= 0)
                        continue;

                    int dx = (mo->x - players[0].mo->x) >> 16;
                    int dy = (mo->y - players[0].mo->y) >> 16;

                    int dist = dx*dx + dy*dy;

                    if (dist > 1500*1500)
                        continue;

                    int16_t ex = mo->x >> 16;
                    int16_t ey = mo->y >> 16;
                    int16_t type = mo->type;
                    int16_t ehp = mo->health;

                    memcpy(buffer + offset, &ex, sizeof(int16_t)); offset += sizeof(int16_t);
                    memcpy(buffer + offset, &ey, sizeof(int16_t)); offset += sizeof(int16_t);
                    memcpy(buffer + offset, &type, sizeof(int16_t)); offset += sizeof(int16_t);
                    memcpy(buffer + offset, &ehp, sizeof(int16_t)); offset += sizeof(int16_t);

                    entity_count++;
                }
            }

            // Escribir entity_count en su sitio
            memcpy(buffer + entity_count_offset, &entity_count, sizeof(int16_t));

            // Enviar paquete
            sendto(udp_sock,
                   buffer,
                   offset,
                   0,
                   (struct sockaddr*)&udp_addr,
                   sizeof(udp_addr));
        }
    }

    // ---- CONTADOR DE TIEMPO ----

    leveltime++;
}
