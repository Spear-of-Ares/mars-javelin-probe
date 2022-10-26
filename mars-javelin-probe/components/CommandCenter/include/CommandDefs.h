/*********************************************************************************
*     File Name           :     /components/CommandCenter/include/CommandDefs.h
*     Created By          :     jon
*     Creation Date       :     [2022-10-26 00:51]
*     Last Modified       :     [2022-10-26 01:29]
*     Description         :     Definitions for commands that are send to the command component
**********************************************************************************/

#ifndef __COMMAND_DEFS_H__
#define __COMMAND_DEFS_H__

/*
    Task notify is a 32 bit integer, meaning there can be 32 individual commands sent, one for each bit.
    This way multiple commands could be sent at the same time.
*/
#define CMD_CUT_DWN         0x01
#define CMD_SELF_TEST       0x02        // Wont be actuall implemented yet 2022-10-26

#define CMD_000             0x04
#define CMD_001             0x08

#endif
