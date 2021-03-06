/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvements copyright (C) 1992, 1993 by Michael         *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.                     *
 *                                                                         *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and        * 
 *  Mitchell Tse.                                                          *
 *                                                                         *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in        *
 *  'license.txt', as well as the Envy license in 'license.nvy'.           *
 *  In particular, you may not remove either of these copyright notices.   *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include "merc.h"

/* opposite directions */
const int opposite_dir [6] = { DIR_SOUTH, DIR_WEST, DIR_NORTH, DIR_EAST, DIR_DOWN, DIR_UP };

/* command procedures needed */


void do_sstat( CHAR_DATA *ch, char *argument )
/* sstat by Garion */
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH * 4];
    int sn;
    int col;
 
    if ( !authorized( ch, "sstat" ) )
	return;

    one_argument( argument, arg );
    col = 0;
 
    if ( arg[0] == '\0' )
    {
        send_to_char("Sstat whom\n\r?", ch);
        return;
    }
 
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char("That person isn't logged on.\n\r", ch);
        return;
    }
 
    if ( IS_NPC( victim ) )
    {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }
 
    buf2[0] = '\0';
 
    for ( sn = 0; sn < MAX_SKILL ; sn++ )
    {
        if ( skill_table[sn].name == NULL )
            break;
        if ( victim->level < skill_table[sn].skill_level[victim->class] )
            continue;
        sprintf( buf1, "%18s %3d %% ", skill_table[sn].name,
                victim->pcdata->learned[sn] );
        strcat( buf2, buf1 );
        if ( ++col %3 == 0 )
            strcat( buf2, "\n\r" );
    }
    if ( col % 3 != 0 )
         strcat( buf2, "\n\r" );
    sprintf( buf1, "%s has %d practice sessions left.\n\r", victim->name,
                victim->practice );
    strcat( buf2, buf1 );
    send_to_char( buf2, ch );
    return;
 
}

void do_update( CHAR_DATA *ch, char *argument )         /* by Maniac */
{
        char    arg[MAX_INPUT_LENGTH];
	int	value;

        if ( !authorized( ch, "update" ) )
                return;

        if ( argument[0] == '\0' )      /* No options ??? */
        {
                send_to_char( "Update, call some game functions\n\r\n\r", ch );
                send_to_char( "bank [value]: Update the share_value.\n\r", ch );
		send_to_char( "time [value]: Set time hour to <value>.\n\r", ch);
		send_to_char( "dumpdb [value]: Set OLC database dunp time to <value>.\n\r", ch );
                return;
        }

        argument = one_argument(argument, arg);

        if (!str_prefix(arg, "bank" ) )
        {
		one_argument(argument, arg);
		value = 0;
		if (arg)
		{
			value = atoi(arg);
			if (value)
				share_value = value;
		}
		else
			bank_update ( );
			
                sprintf(arg, "Ok...bank updated, share_value is now: %d\n\r", share_value );
		send_to_char( arg, ch);
                return;
        }
	if (!str_prefix(arg, "dumpdb" ) )
	{
		one_argument(argument,arg );
		value = 0;
		if (arg)
			value = atoi(arg);
		if (value)
			pulse_db_dump = value;
		sprintf (arg, "Ok... db dump time is now %d\n\r", pulse_db_dump );
		send_to_char(arg, ch);
		return;
	}
	if (!str_prefix(arg, "time" ) )
	{
		one_argument(argument, arg);
		if (arg)
			value = atoi(arg);
		else
		{
			sprintf (arg, "Current hour is %d.\n\r", time_info.hour);
			send_to_char(arg, ch);
			return;
		}
		if (!value)
			return;
		if ((value >= 0) && (value < 24))
			time_info.hour = value;
		return;
	}
        return;
}

/* Display some simple Immortal-only information to an Immortal. */
void do_iscore( CHAR_DATA *ch, char *argument )
{
   CHAR_DATA *victim;
   char buf[MAX_STRING_LENGTH];
   char buf1[MAX_STRING_LENGTH];
   extern bool wizlock;
   extern int numlock;
   char arg1[MAX_INPUT_LENGTH];
   int col = 0;
   int cmd;

   if ( !authorized( ch, "iscore" ) )
        return;

   argument = one_argument( argument, arg1 );

   if ( arg1[0] == '\0' )
	victim = ch;
   else
   {
	if ( !( victim = get_char_world( ch, arg1) ) )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}
	if ( IS_NPC( victim ) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}
   }
 
   sprintf( buf, "Bamfin:  %s\n\r",
      (victim->pcdata != NULL && victim->pcdata->bamfin[0] != '\0')
      ? victim->pcdata->bamfin : "Not changed/Switched." );
   send_to_char( buf, ch );
 
   sprintf( buf, "Bamfout: %s\n\r",
      (victim->pcdata != NULL && victim->pcdata->bamfout[0] != '\0' )
      ? victim->pcdata->bamfout : "Not changed/Switched." );
   send_to_char( buf, ch );

   /*
    * imortall skills listing added by Canth (canth@xs4all.nl)
    */
   sprintf( buf, "Imortal skills set for %s:\n\r", victim->name );
   send_to_char( buf, ch );
   buf1[0] = '\0';
   for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
   {
	if( cmd_table[cmd].level < LEVEL_HERO
	    || str_infix( cmd_table[cmd].name, victim->pcdata->immskll ) )
	    continue;
	sprintf( buf, "%-10s", cmd_table[cmd].name );
	strcat( buf1, buf );
	if( ++col % 8 == 0 )
	    strcat( buf1, "\n\r" );
   }
   if( col % 8 != 0 )
	strcat( buf1, "\n\r" );
   send_to_char( buf1, ch );
 
   if ( wizlock )
     send_to_char( "The Mud is currently Wizlocked.\n\r", ch );

   /*
    * Numlock check added by Canth (canth@xs4all.nl)
    */
   if ( numlock )
   {
	sprintf( buf, "The Mud is currently Numlocked at level %d.\n\r", numlock );
	send_to_char( buf, ch );
   }
 
   return;
}

void do_smite( CHAR_DATA *ch, char *argument ) 		/* by Garion */
{
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    if ( !authorized( ch, "smite" ) )
	return;

    one_argument( argument, arg );
 
    if ( arg[0] == '\0' )
    {
        send_to_char("Smite whom?\n\r", ch );
        return;
    }
 
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        send_to_char("That person is not here.\n\r", ch);
        return;
    }
 
    if ( victim == ch )
    {
        send_to_char("Take it somewhere else, Jack.\n\r", ch );
        return;
    }

    if ( !IS_NPC( victim ) && get_trust(victim) > get_trust(ch) )
    {
        send_to_char("You failed.\n\r", ch);
        act("$n tried to smite you.", ch, NULL, victim, TO_VICT );
        return;
    }
 
    if ( victim->fighting != NULL )
        stop_fighting( victim, TRUE );

sprintf(buf,"       *     W     W   H  H   AA    M   M   !!     *       \n\r");
strcat(buf, "     *****    W W W    HHHH  AAAA   M M M   !!   *****     \n\r");
strcat(buf, "       *      W   W    H  H  A  A  M     M  !!     *       \n\r");
strcat( buf, "\n\r" );
send_to_char( buf, victim );
 
    act("$n raises $s hand and smites you!", ch, NULL, victim, TO_VICT );
    act("$n raises $s hand and smites $N!", ch, NULL, victim, TO_NOTVICT );
    act("You raise your hand and smite $N!", ch, NULL, victim, TO_CHAR );
 
    if ( victim->hit >= 2 )
        victim->hit /= 2;

    if ( ( obj = get_eq_char( victim, WEAR_FEET ) ) != NULL )
    {
    obj_from_char( obj );
    send_to_char("You are blown out of your shoes and right onto your ass!\n\r",
        victim );
    act("$N is blown out of $s shoes and right onto $s ass!", ch, NULL,
        victim, TO_NOTVICT );
    act("$N is blown out of $s shoes and right onto $s ass!", ch, NULL,
        victim, TO_CHAR );
    obj_to_room( obj, victim->in_room );
    }
    else
    {
        send_to_char("You are knocked on your ass!\n\r", victim );
        act("$N is knocked on his ass!", ch, NULL, victim, TO_NOTVICT );
        act("$N is knocked on his ass!", ch, NULL, victim, TO_CHAR );
    }
 
    victim->position = POS_RESTING;
    return;
}
 

void do_dog( CHAR_DATA *ch, char *argument )
{
   /* A real silly command which switches the (mortal) victim into
    * a mob.  As the victim is mortal, they won't be able to use
    * return ;P  So will have to be released by someone...
    * -S-
    */
 
    ROOM_INDEX_DATA *location;
    MOB_INDEX_DATA  *pMobIndex;
    CHAR_DATA       *mob;
    CHAR_DATA       *victim;
    char             buf [ MAX_STRING_LENGTH ];
 
    if ( !authorized( ch, "dog" ) )
	return;

    if ( argument[0] == '\0' )
    {
       send_to_char( "Turn WHO into a little doggy?\n\r", ch );
       return;
    }
 
    if ( ( victim = get_char_world( ch, argument ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
 
    if ( IS_NPC(victim) )
    {
       send_to_char( "Cannot do this to mobs, only pcs.\n\r",ch);
       return;
    }
 
    if ( ( pMobIndex = get_mob_index( MOB_VNUM_DOGGY ) ) == NULL )
    {
       send_to_char( "Couldn't find the doggy's vnum!!\n\r", ch );
       return;
    }
 
    if ( victim->desc == NULL )
    {
       send_to_char( "Already switched, like.\n\r", ch );
       return;
    }

    if ( get_trust(victim) >= get_trust(ch) )
    {
	send_to_char( "You cannot dog your peer nor your superior.\n\r", ch );
	return;
    }

    mob = create_mobile( pMobIndex );
    location = victim->in_room;         /* Remember where to load doggy! */
    char_from_room( victim );
 
    char_to_room( victim, get_room_index( ROOM_VNUM_LIMBO ) );
    char_to_room( mob, location );
 
 
    /* Instead of calling do switch, just do the relevant bit here */
    victim->desc->character = mob;
    victim->desc->original  = victim;
    mob->desc               =  victim->desc;
    victim->desc            = NULL;
 
    act( "$n is suddenly turned into a small doggy!!", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You suddenly turn into a small doggy!", victim );
    send_to_char( "Ok.\n\r", ch );
    sprintf( buf, "%s was turned into a little doggy by %s",
	victim->desc->original->name, ch->name );
    wiznet( ch, WIZ_SWITCHES, get_trust( ch ), buf );
    return;
}

/* Idea from Talen of Vego's do_where command */
void do_owhere( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA  *obj;
    OBJ_DATA  *in_obj;
    CHAR_DATA *rch;
    char       buf  [ MAX_STRING_LENGTH   ];
    char       buf1 [ MAX_STRING_LENGTH*6 ];
    char       arg  [ MAX_INPUT_LENGTH    ];
    int        obj_counter = 1;
    bool       found = FALSE;
 
    rch = get_char( ch );
 
    if ( !authorized( rch, "owhere" ) )
        return;
 
    one_argument( argument, arg );
 
    if( arg[0] == '\0' )
    {
        send_to_char( "Syntax:  owhere <object>.\n\r", ch );
        return;
    }
    else
    {
        buf1[0] = '\0';
        for ( obj = object_list; obj; obj = obj->next )
        {
            if ( !can_see_obj( ch, obj ) || !is_name( arg, obj->name ) )
                continue;
 
            found = TRUE;
 
            for ( in_obj = obj; in_obj->in_obj;
                 in_obj = in_obj->in_obj )
                ;
 
            if ( in_obj->carried_by )
            {
                if ( !can_see( ch, in_obj->carried_by ) )
                    continue;
                sprintf( buf, "[%2d] %s carried by %s at [%4d].\n\r",
                        obj_counter, obj->short_descr,
                        PERS( in_obj->carried_by, ch ),
                        in_obj->carried_by->in_room->vnum );
            }
            else
            {
                sprintf( buf, "[%2d] %s in %s at [%4d].\n\r", obj_counter,
                        obj->short_descr, ( !in_obj->in_room ) ?
                        "somewhere" : in_obj->in_room->name,
                        ( !in_obj->in_room ) ?
                        0 : in_obj->in_room->vnum );
            }
 
            obj_counter++;
            buf[0] = UPPER( buf[0] );
            strcat( buf1, buf );
 
            /* Only see the first 101 */
            if ( obj_counter > 100 )
                break;
        }
 
        send_to_char( buf1, ch );
    }
 
    if ( !found )
        send_to_char(
                "Nothing like that in hell, earth, or heaven.\n\r" , ch );
 
    return;
 
 
}
 
void do_numlock( CHAR_DATA *ch, char *argument )  /*By Globi*/
{
           CHAR_DATA *rch;
           char       buf  [ MAX_STRING_LENGTH ];
           char       arg1 [ MAX_INPUT_LENGTH  ];
    extern int        numlock;
           int        temp;
 
    rch = get_char( ch );
 
    if ( !authorized( rch, "numlock" ) )
        return;
 
    one_argument( argument, arg1 );
 
    temp = atoi( arg1 );
 
    if ( arg1[0] == '\0' ) /* Prints out the current value */
    {
        sprintf( buf, "Current numlock setting is:  %d.\n\r", numlock );
        send_to_char( buf, ch );
        return;
    }
 
    if ( ( temp < 0 ) || ( temp > LEVEL_HERO ) )
    {
        sprintf( buf, "Level must be between 0 and %d.\n\r", LEVEL_HERO );
        send_to_char( buf, ch );
        return;
    }
 
    numlock = temp;  /* Only set numlock if arg supplied and within range */
 
    if ( numlock != 0 )
    {
        sprintf( buf, "Game numlocked to levels %d and below.\n\r", numlock );
        send_to_char( buf, ch );
    }
    else
        send_to_char( "Game now open to all levels.\n\r", ch );
 
    return;
 
}
 
void do_newlock( CHAR_DATA *ch, char *argument )
{
           CHAR_DATA *rch;
    extern int        numlock;
           char       buf [ MAX_STRING_LENGTH ];
 
    rch = get_char( ch );
 
    if ( !authorized( rch, "newlock" ) )
        return;
 
    if ( numlock != 0 && get_trust( ch ) < L_SEN )
    {
        send_to_char( "You may not change the current numlock setting\n\r",
                     ch );
        sprintf( buf, "Game numlocked to levels %d and below.\n\r", numlock );
        send_to_char( buf, ch );
        return;
    }
 
    if ( numlock != 0 )
    {
        sprintf( buf, "Game numlocked to levels %d and below.\n\r", numlock );
        send_to_char( buf, ch );
        send_to_char( "Changing to: ", ch );
    }
 
    numlock = 1;
    send_to_char( "Game locked to new characters.\n\r", ch );
    return;
 
}
 
void do_sstime( CHAR_DATA *ch, char *argument )
{
           CHAR_DATA *rch;
    extern time_t     down_time;
    extern time_t     warning1;
    extern time_t     warning2;
    extern bool       Reboot;
           char       buf  [ MAX_STRING_LENGTH ];
           char       arg1 [ MAX_INPUT_LENGTH  ];
           int        number;
 
    rch = get_char( ch );
 
    if ( !authorized( rch, "sstime" ) )
        return;
 
    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
 
    if ( !str_cmp( arg1, "reboot" ) )
    {
        Reboot = !Reboot;
        sprintf( buf, "Reboot is %s.\n\r", Reboot ? "on" : "off" );
        send_to_char( buf, ch );
        return;
    }
 
    number   = atoi( arg1 );
 
    if ( arg1[0] == '\0' || !is_number( arg1 ) || number < 0 )
    {
        send_to_char( "Syntax: sstime <value>/reboot\n\r",                ch );
        send_to_char( "\n\r",                                             ch );
        send_to_char( "Value is number of minutes till reboot/shutdown.", ch );
        send_to_char( "  Or 0 to turn off.\n\r",                          ch );
        send_to_char( "Reboot will toggle reboot on or off.\n\r",         ch );
        send_to_char( "\n\r",                                             ch );
        if ( down_time > 0 )
        {
            sprintf( buf, "1st warning:  %d minutes (%d seconds).\n\r",
                    UMAX( ( (int) warning1 - (int) current_time ) / 60, 0 ),
                    UMAX( ( (int) warning1 - (int) current_time ), 0 ) );
            send_to_char( buf,                                            ch );
            sprintf( buf, "2nd warning:  %d minutes (%d seconds).\n\r",
                    UMAX( ( (int) warning2 - (int) current_time ) / 60, 0 ),
                    UMAX( ( (int) warning2 - (int) current_time ), 0 ) );
            send_to_char( buf,                                            ch );
            sprintf( buf, "%s%d minutes (%d seconds).\n\r",
                    Reboot ? "Reboot:       " : "Shutdown:     ",
                    UMAX( ( (int) down_time - (int) current_time ) / 60, 0 ),
                    UMAX( ( (int) down_time - (int) current_time ), 0 ) );
            send_to_char( buf,                                            ch );
        }
        else
            send_to_char( "Automatic reboot/shutdown:  off.\n\r",         ch );
        return;
    }
 
    /* Set something */
 
    if ( number > 0 )
    {
        down_time = current_time + ( number * 60 );
        if ( number < 6 )
        {
            warning2  = down_time - 150;
            warning1  = warning2  - 75;
        }
        else
        {
            warning2  = down_time - 150;
            warning1  = warning2  - 150;
        }
        sprintf( buf, "%s will be in %d minutes (%d seconds).\n\r",
                Reboot ? "Reboot" : "Shutdown",
                ( (int) down_time - (int) current_time ) / 60,
                ( (int) down_time - (int) current_time ) );
        send_to_char( buf, ch );
        sprintf( buf, "1st Warning will be in %d minutes (%d seconds).\n\r",
                ( (int) warning1 - (int) current_time ) / 60,
                ( (int) warning1 - (int) current_time ) );
        send_to_char( buf, ch );
        sprintf( buf, "2nd Warning will be in %d minutes (%d seconds).\n\r",
                ( (int) warning2 - (int) current_time ) / 60,
                ( (int) warning2 - (int) current_time ) );
        send_to_char( buf, ch );
    }
    else
    {
        down_time = 0;
        sprintf( buf, "Auto%s is now off.\n\r",
                Reboot ? "reboot" : "shutdown" );
        send_to_char( buf, ch );
    }
 
    return;
 
}

/*
 * Modifications to old imtlset by:
 * Canth (canth@xs4all.nl)
 * Vego (v942429@si.hhs.nl)
 */
void do_imtlset( CHAR_DATA *ch, char *argument )
{
 
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char       arg1 [ MAX_INPUT_LENGTH  ];
    char       buf  [ MAX_STRING_LENGTH ];
    char       buf1 [ MAX_STRING_LENGTH ];
    char      *buf2;
    char      *buf3 = NULL;
    char      *skill;
    int        cmd;
    int        col = 0;
    int        i = 0;
 
    rch = get_char( ch );
 
    if ( !authorized( rch, "imtlset" ) )
        return;
 
    argument = one_argument( argument, arg1 );
 
    if ( arg1[0] == '\0' )
    {
        send_to_char( "Syntax: imtlset <victim> +|- <immortal skill>\n\r", ch );
	send_to_char( "or:     imtlset <victim> +|- all\n\r",              ch );
	send_to_char( "or:     imtlset <victim>\n\r",                      ch );
        return;
    }
 
    if ( !( victim = get_char_world( rch, arg1 ) ) )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
 
    if ( IS_NPC( victim ) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }
 
    if ( get_trust(rch) < get_trust(victim) && rch != victim )
    {
        send_to_char( "You may not imtlset your superior.\n\r",
                     ch );
        return;
    }

    if (( rch == victim ) && (rch->level != L_DIR) )
    {
	send_to_char( "You may not set your own immortal skills.\n\r", ch );
	return;
    }
 
    if ( argument[0] == '+' || argument[0] == '-' )
    {
        buf[0] = '\0';
        smash_tilde( argument );
        if ( argument[0] == '+' )
        {
		argument++;
		if ( !str_cmp( "all", argument ) )
		/*
		 * Imtlset <victim> + all by:
		 * Canth (canth@xs4all.nl)
		 */
		{
			for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
			{
				if ( cmd_table[cmd].level > get_trust( rch ) )
				     continue;
				if ( cmd_table[cmd].level <= victim->level && cmd_table[cmd].level >= LEVEL_HERO )
				{
		     			strcat(buf, cmd_table[cmd].name);
					strcat(buf, " ");
				}
			}
		}
		else
		{
			if ( victim->pcdata->immskll )
				strcat( buf, victim->pcdata->immskll );
			while ( isspace( *argument ) )
				argument++;
			for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
			{
				if ( cmd_table[cmd].level > get_trust( rch ) )
					continue;
				if ( !str_cmp( argument, cmd_table[cmd].name ) )
					break;
			}
			if ( cmd_table[cmd].name[0] == '\0' )
			{
				send_to_char("That is not an immskill.\n\r",ch);
				return;
			}
			if ( !str_infix( argument, victim->pcdata->immskll ) )
			{
				send_to_char("That skill has already been set.\n\r",ch);
				return;
			}
		strcat( buf, argument );
		strcat( buf, " " );
		}
	}

        if ( argument[0] == '-' )
        {
	    argument++;
	    one_argument( argument, arg1 );
	    if ( !str_cmp( "all", arg1 ) )
	    {
		free_string( victim->pcdata->immskll );
		victim->pcdata->immskll = str_dup( "" );
		send_to_char( "All immskills have been deleted.\n\r", ch );
		return;
	    }
	    else if ( arg1 )
	    {
		/*
		 * Cool great imtlset <victim> - <skill> code...
		 * Idea from Canth (canth@xs4all.nl)
		 * Code by Vego (v942429@si.hhs.nl)
		 * Still needs memory improvements.... (I think)
		 */
		buf2 = str_dup( victim->pcdata->immskll );
		buf3 = buf2;
		if ( (skill = strstr( buf2, arg1 ) ) == NULL )
		{
		    send_to_char( "That person doesn't have that immskill", ch);
		    return;
		}
		else
		{
		    while ( buf2 != skill )
			buf[i++] = *(buf2++);
		    while ( !isspace ( *(buf2++) ) );
		    buf[i] = '\0';
		    strcat ( buf, buf2 );
		}
	    }
	    else
	    {
		send_to_char ( "That's not an immskill\n\r", ch );
		return;
	    }
        }
 
	free_string( buf3 );
	skill = buf2 = buf3 = NULL;
        free_string( victim->pcdata->immskll );
        victim->pcdata->immskll = str_dup( buf );
    }
 
    sprintf( buf, "Immortal skills set for %s:\n\r", victim->name );
    send_to_char( buf, ch );
    buf1[0] = '\0';
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level < LEVEL_HERO
            || str_infix( cmd_table[cmd].name, victim->pcdata->immskll ) )
            continue;
 
        sprintf( buf, "%-10s", cmd_table[cmd].name );
        strcat( buf1, buf );
        if ( ++col % 8 == 0 )
            strcat( buf1, "\n\r" );
    }
 
    if ( col % 8 != 0 )
        strcat( buf1, "\n\r" );
    send_to_char( buf1, ch );
 
    return;
 
}

/* ======================================================================== */
/* Show all connected user sockets                                          */
/* buf set to 256*80 (256 is max connections, 80 chars per line)            */
/* ======================================================================== */
void do_users( CHAR_DATA *ch, char *arg )
{
    DESCRIPTOR_DATA      *d;
    CHAR_DATA            *wch;
    char                  buf[21000];
    int                   count= 0;
 
    if ( !authorized( ch, "users" ) )
	return;

    buf[0]      = '\0';
 
    /* Show all descriptors, even if character not assigned yet. */
    /* --------------------------------------------------------- */
 
    for ( d= descriptor_list; d; d= d->next )
    {
        wch = d->original ? d->original : d->character;
 
        if ( ( arg[0] == '\0' && ( wch == NULL || can_see( ch, wch ) ) )
        ||   ( arg[0] != '\0' &&  wch && is_name( arg, wch->name ) ) )
        {
            count++;
            sprintf( buf + strlen(buf), "[%3d %2d] %s@%s\n\r",
                d->descriptor,
                d->connected,
                wch  ? wch->name : "(none)",
                d->host
                );
        }
    }
 
    /* Status messages */
    /* --------------- */
 
    if ( count == 0 )
    {
        sprintf( buf, "You didn't find any user with the name %s.\n\r", arg );
        send_to_char( buf, ch);
    }
    else
    {
        sprintf( buf + strlen(buf), "%d connections.\n\r", count );
        send_to_char( buf, ch );
    }
}

void do_notestat( CHAR_DATA *ch, char *argument )
/*   notestat by Garion */
{
    char buf1[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    NOTE_DATA *note;
    int vnum;
 
    if ( !authorized( ch, "notestat" ) )
	return;

    one_argument( argument, arg );
 
    if ( arg[0] == '\0' )
    {
        send_to_char("Notestat whom?\n\r", ch);
        return;
    }
 
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char("There's no such person Jack.\n\r", ch);
        return;
    }
 
    if (( get_trust(victim) >= get_trust(ch) ) && ch != victim )
    {
	send_to_char("You may not notestat your peer nor your superior.\n\r", ch );
	return;
    }

    buf1[0] = '\0';
    buf2[0] = '\0';
    vnum = 0;
 
    for ( note = note_list; note != NULL; note = note->next )
    {
        if ( is_note_to( victim, note ) )
        {
            sprintf( buf1, "[%3d%s] %s: %s\n\r", vnum,
                ( note->date_stamp > victim->last_note
                && str_cmp( note->sender, victim->name ) ) ? "N" : " ",
                note->sender, note->subject );
            strcat(buf2, buf1 );
            vnum++;
        }
    }
 
   send_to_char( buf2, ch );
   return;
}

void do_wizpwd( CHAR_DATA *ch, char *argument )
{
    char *pArg;
    char *pwdnew;
    char *p;
    CHAR_DATA *victim;
    char  arg1 [ MAX_INPUT_LENGTH ];
    char  arg2 [ MAX_INPUT_LENGTH ];
    char  cEnd;
    if ( IS_NPC( ch ) )
        return;

    if ( !authorized( ch, "wizpwd" ) )
        return;

    one_argument(argument, arg1);
    one_argument(argument, arg2);

    if ( arg1[0] == '\0' || arg2[0] == '\0')
    {
        send_to_char( "Syntax: wizpwd player newpassword.\n\r", ch );
        return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
        send_to_char("That person isn't logged on.\n\r", ch);
        return;
    }        

    if ( IS_NPC( victim ) )
    {
        send_to_char("Not on NPC's.\n\r", ch);
        return;
    }

    /*
     * Level check added by Canth (canth@xs4all.nl)
     */
    if ( get_trust(victim) >= get_trust(ch) )
    {
	send_to_char("You may not wizpwd your peer nor your superior.\n\r",ch);
	return;
    }

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace( *argument ) )
        argument++;
    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
        cEnd = *argument++;
    while ( *argument != '\0' )
    {
        if ( *argument == cEnd )
        {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';
    pArg = arg2;
    while ( isspace( *argument ) )
        argument++;
    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
        cEnd = *argument++;
    while ( *argument != '\0' )     
    {
        if ( *argument == cEnd )
        {
            argument++;
            break;
        }
        *pArg++ = *argument++;
    }
    *pArg = '\0';
    *argument = '\0';
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Syntax: wizpwd player newpassword.\n\r", ch );
        return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, victim->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
        if ( *p == '~' )
        {
            send_to_char(
                "New password not acceptable, try again.\n\r", ch );
            return;
        }
    }
    free_string( victim->pcdata->pwd );
    victim->pcdata->pwd = str_dup( pwdnew );
    /*
     * save_char_obj changed from ch to victim by Canth (canth@xs4all.nl)
     */
    save_char_obj( victim );
    send_to_char( "Ok.\n\r", ch );
    return;
} 

/* RT set replaces sset, mset, oset, rset and lset */
void do_set( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
        send_to_char("Syntax:\n\r",ch);
        send_to_char("  set mob   <name> <field> <value>\n\r",ch);
        send_to_char("  set obj   <name> <field> <value>\n\r",ch);
        send_to_char("  set room  <room> <field> <value>\n\r",ch);
        send_to_char("  set skill <name> <spell or skill> <value>\n\r",ch);
	send_to_char("	set lang  <name> <language> <value>\n\r",ch);
        return;
    }

    if (!str_prefix(arg,"mobile") || !str_prefix(arg,"character"))
    {
        do_mset(ch,argument);
        return;
    }

    if (!str_prefix(arg,"skill") || !str_prefix(arg,"spell"))
    {
        do_sset(ch,argument);
        return;
    }

    if (!str_prefix(arg,"language"))
    {
        do_lset(ch,argument);
        return;
    }

    if (!str_prefix(arg,"object"))
    {
        do_oset(ch,argument);
        return;
    }

    if (!str_prefix(arg,"room"))
    {
        do_rset(ch,argument);
        return;
    }
    /* echo syntax */
    do_set(ch,"");
}

/* RT to replace the 3 stat commands */
/* Maniac added lstat */
void do_stat ( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char *string;
   OBJ_DATA *obj;
   ROOM_INDEX_DATA *location;
   CHAR_DATA *victim;

   string = one_argument(argument, arg);
   if ( arg[0] == '\0')
   {
        send_to_char("Syntax:\n\r",ch);
        send_to_char("  stat <name>\n\r",ch);
        send_to_char("  stat obj <name>\n\r",ch);
        send_to_char("  stat mob <name>\n\r",ch);
        send_to_char("  stat room <number>\n\r",ch);
	send_to_char("  stat lang <name>\n\r",ch);
        return;
   }

   if (!str_cmp(arg,"room"))
   {
        do_rstat(ch,string);
        return;
   }

   if (!str_cmp(arg,"obj"))
   {
        do_ostat(ch,string);
        return;
   }

   if (!str_cmp(arg, "lang") || !str_cmp(arg, "language"))
   {
	do_lstat(ch, string);
	return;
   }

   if(!str_cmp(arg,"char")  || !str_cmp(arg,"mob"))
   {
        do_mstat(ch,string);
        return;
   }

   /* do it the old way */
   obj = get_obj_world(ch,argument);
   if (obj != NULL)
   {
     do_ostat(ch,argument);
     return;
   }

  victim = get_char_world(ch,argument);
  if (victim != NULL)
  {
    do_mstat(ch,argument);
    return;
  }

  location = find_location(ch,argument);
  if (location != NULL)
  {
    do_rstat(ch,argument);
    return;
  }

  send_to_char("Nothing by that name found anywhere.\n\r",ch);
}

/* ofind and mfind replaced with vnum, vnum skill also added */
void do_vnum(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char *string;

    string = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
        send_to_char("Syntax:\n\r",ch);
        send_to_char("  vnum obj <name>\n\r",ch);
        send_to_char("  vnum mob <name>\n\r",ch);
        send_to_char("  vnum skill <skill or spell>\n\r",ch);
        return;
    }

    if (!str_cmp(arg,"obj"))
    {
        do_ofind(ch,string);
        return;
    }

    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))
    {
        do_mfind(ch,string);
        return;
    }

    if (!str_cmp(arg,"skill") || !str_cmp(arg,"spell"))
    {
        do_slookup(ch,string);
        return;
    }
    /* do both */
    do_mfind(ch,argument);
    do_ofind(ch,argument);
}

void do_string( CHAR_DATA *ch, char *argument )
{
    char type [MAX_INPUT_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( !authorized( ch, "string" ) )
	return;

    smash_tilde( argument );
    argument = one_argument( argument, type );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if (type[0]=='\0' || arg1[0]=='\0' || arg2[0]=='\0' || arg3[0]=='\0')
    {
        send_to_char("Syntax:\n\r",ch);
        send_to_char("  string char <name> <field> <string>\n\r",ch);
        send_to_char("    fields: name short long desc title spec game\n\r",ch);
        send_to_char("  string obj  <name> <field> <string>\n\r",ch);
        send_to_char("    fields: name short long extended\n\r",ch);
        return;
    }

    if (!str_prefix(type,"character") || !str_prefix(type,"mobile"))
    {
        if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
        {
            send_to_char( "They aren't here.\n\r", ch );
            return;
        }

        /* string something */
        if ( !str_prefix( arg2, "name" ) )
        {
            if ( !IS_NPC(victim) )
            {
                send_to_char( "Not on PC's.\n\r", ch );
                return;
            }

            free_string( victim->name );
            victim->name = str_dup( arg3 );
            return;
        }

        if ( !str_prefix( arg2, "description" ) )
        {
            free_string(victim->description);
            victim->description = str_dup(arg3);
            return;
            return;
        }
        if ( !str_prefix( arg2, "short" ) )
        {
            free_string( victim->short_descr );
            victim->short_descr = str_dup( arg3 );
            return;
        }

        if ( !str_prefix( arg2, "long" ) )
        {
            free_string( victim->long_descr );
            strcat(arg3,"\n\r");
            victim->long_descr = str_dup( arg3 );
            return;
        }

        if ( !str_prefix( arg2, "title" ) )
        {
            if ( IS_NPC(victim) )
            {
                send_to_char( "Not on NPC's.\n\r", ch );
                return;
            }
            set_title( victim, arg3 );
            return;
        }

        if ( !str_prefix( arg2, "spec" ) )
        {
            if ( !IS_NPC(victim) )
            {
                send_to_char( "Not on PC's.\n\r", ch );
                return;
            }
            if ( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
            {
                send_to_char( "No such spec fun.\n\r", ch );
                return;
            }

            return;
        }

        if ( !str_prefix( arg2, "game" ) )
        {
            if ( !IS_NPC(victim) )
            {
                send_to_char( "Not on PC's.\n\r", ch );
                return;
            }
            if ( ( victim->game_fun = game_lookup( arg3 ) ) == 0 )
            {
                send_to_char( "No such game fun.\n\r", ch );
                return;
            }

            return;
        }

    }

    if (!str_prefix(type,"object"))
    {
        /* string an obj */

        if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
        {
            send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
            return;
        }

        if ( !str_prefix( arg2, "name" ) )
        {
            free_string( obj->name );
            obj->name = str_dup( arg3 );
            return;
        }

        if ( !str_prefix( arg2, "short" ) )
        {
            free_string( obj->short_descr );
            obj->short_descr = str_dup( arg3 );
            return;
        }

        if ( !str_prefix( arg2, "long" ) )
        {
            free_string( obj->description );
            obj->description = str_dup( arg3 );
            return;
        }

        if ( !str_prefix( arg2, "ed" ) || !str_prefix( arg2, "extended"))
        {
            EXTRA_DESCR_DATA *ed;

            argument = one_argument( argument, arg3 );
            if ( argument == NULL )
            {
                send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r",
                    ch );
                return;
            }

            strcat(argument,"\n\r");

            if ( extra_descr_free == NULL )
            {
                ed                      = alloc_perm( sizeof(*ed) );
            }
            else
            {
                ed                      = extra_descr_free;
                extra_descr_free        = ed->next;
            }

            ed->keyword         = str_dup( arg3     );
            ed->description     = str_dup( argument );
            ed->next            = obj->extra_descr;
            obj->extra_descr    = ed;
            return;
        }
    }


    /* echo bad use message */
    do_string(ch,"");
}



void do_setkill( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;
    CHAR_DATA *victim;
    char       arg1 [ MAX_INPUT_LENGTH  ];
    char       arg2 [ MAX_INPUT_LENGTH  ];
    char       buf  [ MAX_STRING_LENGTH ];

    rch = get_char( ch );

    if ( !authorized( rch, "setkill" ) )
	return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "Syntax: setkill <character> <killer|thief|PK>.\n\r", ch );
        return;
    }

    if ( !( victim = get_char_world( ch, arg1 ) ) )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }

    if ( IS_NPC( victim ) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

    if ( !str_cmp( arg2, "killer" ) )
    {
        if (!IS_SET( victim->act, PLR_KILLER ) )
        {
            SET_BIT( victim->act, PLR_KILLER );
            send_to_char( "Killer flag set.\n\r", ch );
            send_to_char( "You are a KILLER.\n\r", victim );
	    sprintf( buf, "%s was set KILLER by %s", victim->name, ch->name );
	    wiznet( ch, WIZ_FLAGS, get_trust( ch ), buf );
        }
        return;
    }

    if ( !str_cmp( arg2, "thief" ) )
    {
        if (!IS_SET( victim->act, PLR_THIEF ) )
        {
            SET_BIT( victim->act, PLR_THIEF );
            send_to_char( "Thief flag set.\n\r", ch );
            send_to_char( "You are a THIEF.\n\r", victim );
	    sprintf( buf, "%s was set THIEF by %s", victim->name, ch->name );
	    wiznet( ch, WIZ_FLAGS, get_trust( ch ), buf );
        }
        return;
    }

    if ( !str_cmp( arg2, "pk" ) )
    {
	if (!IS_SET( victim->act, PLR_REGISTER ) )
	{
	    SET_BIT( victim->act, PLR_REGISTER );
	    send_to_char( "PK flag set.\n\r", ch );
	    send_to_char( "You are a PK.\n\r", victim );
	    sprintf( buf, "%s was set PK by %s", victim->name, ch->name );
	    wiznet( ch, WIZ_FLAGS, get_trust( ch ), buf );
	}
	return;
    }

    do_setkill( ch, "" );
    return;
}

void do_pwhere( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA	*victim;
	DESCRIPTOR_DATA	*d;
	char		buf [ MAX_STRING_LENGTH ];

	if ( !authorized( ch, "pwhere" ) )
		return;

	send_to_char( "Player locations:\n\r", ch );
	for ( d = descriptor_list; d; d = d->next )
	{
		if ( d->connected == CON_PLAYING
			&& ( victim = d->character )
			&& !IS_NPC( victim )
			&& victim->in_room )
		{
			sprintf( buf, "%-28s %d %s\n\r",
				victim->name, victim->in_room->vnum, victim->in_room->name );
			send_to_char( buf, ch );
		}
        }
}

/* Expand the name of a character into a string that identifies THAT
   character within a room. E.g. the second 'guard' -> 2. guard */   
const char * name_expand (CHAR_DATA *ch)
{
	int count = 1;
	CHAR_DATA *rch;
	char name[MAX_INPUT_LENGTH]; /*  HOPEFULLY no mob has a name longer than THAT */

	static char outbuf[MAX_INPUT_LENGTH];	
	
	if (!IS_NPC(ch))
		return ch->name;
		
	one_argument (ch->name, name); /* copy the first word into name */
	
	if (!name[0]) /* weird mob .. no keywords */
	{
		strcpy (outbuf, ""); /* Do not return NULL, just an empty buffer */
		return outbuf;
	}
		
	for (rch = ch->in_room->people; rch && (rch != ch);rch = rch->next_in_room)
		if (is_name (name, rch->name))
			count++;
			

	sprintf (outbuf, "%d.%s", count, name);
	return outbuf;
}

/*
 * For by Erwin S. Andreasen (4u2@aabc.dk)
 */
void do_for (CHAR_DATA *ch, char *argument)
{
	char range[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
	bool fGods = FALSE, fMortals = FALSE, fMobs = FALSE, fEverywhere = FALSE, found;
	ROOM_INDEX_DATA *room, *old_room;
	CHAR_DATA *p, *p_next;
	int i;

	if ( !authorized( ch, "for" ) )
		return;

	argument = one_argument (argument, range);
	
	if (!range[0] || !argument[0]) /* invalid usage? */
	{
		do_help (ch, "for");
		return;
	}

	if (!str_prefix("quit", argument))
	{
		send_to_char ("Are you trying to crash the MUD or something?\n\r",ch);
		return;
	}


	if (!str_cmp (range, "all"))
	{
		fMortals = TRUE;
		fGods = TRUE;
	}
	else if (!str_cmp (range, "gods"))
		fGods = TRUE;
	else if (!str_cmp (range, "mortals"))
		fMortals = TRUE;
	else if (!str_cmp (range, "mobs"))
		fMobs = TRUE;
	else if (!str_cmp (range, "everywhere"))
		fEverywhere = TRUE;
	else
		do_help (ch, "for"); /* show syntax */

	/* do not allow # to make it easier */
	/* Don't allow it, it causes crashes on 'for mobs tell # hi' Maniac */
	if (fEverywhere && strchr (argument, '#'))
	{
		send_to_char ("Cannot use FOR EVERYWHERE with the # thingie.\n\r",ch);
		return;
	}

	if (fMobs && strchr (argument, '#'))
        {
                send_to_char ("Cannot use FOR MOBS with the # thingie.\n\r",ch);
                return;
        }

	if (strchr (argument, '#')) /* replace # ? */
	{ 
		for (p = char_list; p ; p = p_next)
		{
			p_next = p->next; /* In case someone DOES try to AT MOBS SLAY # */
			found = FALSE;
			
			if (!(p->in_room) || room_is_private(p->in_room) || (p == ch))
				continue;
			
			if (IS_NPC(p) && fMobs)
				found = TRUE;
			else if (!IS_NPC(p) && p->level >= LEVEL_IMMORTAL && fGods)
				found = TRUE;
			else if (!IS_NPC(p) && p->level < LEVEL_IMMORTAL && fMortals)
				found = TRUE;

			/* It looks ugly to me.. but it works :) */				
			if (found) /* p is 'appropriate' */
			{
				char *pSource = argument; /* head of buffer to be parsed */
				char *pDest = buf; /* parse into this */
				
				while (*pSource)
				{
					if (*pSource == '#') /* Replace # with name of target */
					{
						const char *namebuf = name_expand (p);
						
						if (namebuf) /* in case there is no mob name ?? */
							while (*namebuf) /* copy name over */
								*(pDest++) = *(namebuf++);

						pSource++;
					}
					else
						*(pDest++) = *(pSource++);
				} /* while */
				*pDest = '\0'; /* Terminate */
				
				/* Execute */
				old_room = ch->in_room;
				char_from_room (ch);
				char_to_room (ch,p->in_room);
				interpret (ch, buf);
				char_from_room (ch);
				char_to_room (ch,old_room);
				
			} /* if found */
		} /* for every char */
	}
	else /* just for every room with the appropriate people in it */
	{
		for (i = 0; i < MAX_KEY_HASH; i++) /* run through all the buckets */
			for (room = room_index_hash[i] ; room ; room = room->next)
			{
				found = FALSE;
				
				/* Anyone in here at all? */
				if (fEverywhere) /* Everywhere executes always */
					found = TRUE;
				else if (!room->people) /* Skip it if room is empty */
					continue;
					
					
				/* Check if there is anyone here of the requried type */
				/* Stop as soon as a match is found or there are no more ppl in room */
				for (p = room->people; p && !found; p = p->next_in_room)
				{

					if (p == ch) /* do not execute on oneself */
						continue;
						
					if (IS_NPC(p) && fMobs)
						found = TRUE;
					else if (!IS_NPC(p) && (p->level >= LEVEL_IMMORTAL) && fGods)
						found = TRUE;
					else if (!IS_NPC(p) && (p->level <= LEVEL_IMMORTAL) && fMortals)
						found = TRUE;
				} /* for everyone inside the room */
						
				if (found && !room_is_private(room)) /* Any of the required type here AND room not private? */
				{
					/* This may be ineffective. Consider moving character out of old_room
					   once at beginning of command then moving back at the end.
					   This however, is more safe?
					*/
				
					old_room = ch->in_room;
					char_from_room (ch);
					char_to_room (ch, room);
					interpret (ch, argument);
					char_from_room (ch);
					char_to_room (ch, old_room);
				} /* if found */
			} /* for every room in a bucket */
	} /* if strchr */
} /* do_for */

/*
 * Rename by Erwin S Andreasen (4u2@aabc.dk)
 */
/*
 * do_rename renames a player to another name.
 * PCs only. Previous file is deleted, if it exists.
 * Char is then saved to new file.
 * New name is checked against std. checks, existing offline players and
 * online players. 
 * .gz files are checked for too, just in case.
 */

bool check_parse_name (char* name);  /* comm.c */
char *initial( const char *str );    /* comm.c */

void do_rename (CHAR_DATA* ch, char* argument)
{
	char old_name[MAX_INPUT_LENGTH], 
	     new_name[MAX_INPUT_LENGTH],
	     strsave [MAX_INPUT_LENGTH];

	CHAR_DATA* victim;
	FILE* file;

	if ( !authorized( ch, "rename" ) )
		return;

	argument = one_argument(argument, old_name); /* find new/old name */
	one_argument (argument, new_name);

	/* Trivial checks */
	if (!old_name[0])
	{
		send_to_char ("Rename who?\n\r",ch);
		return;
	}

	victim = get_char_world (ch, old_name);

	if (!victim)
	{
		send_to_char ("There is no such a person online.\n\r",ch);
		return;
	}
	
	if (IS_NPC(victim))
	{   
		send_to_char ("You cannot use Rename on NPCs.\n\r",ch);
		return;
	}

	/* allow rename self new_name,but otherwise only lower level */	
	if ( (victim != ch) && (get_trust (victim) >= get_trust (ch)) )
	{
		send_to_char ("You failed.\n\r",ch);
		return;
	}
	
	if (!victim->desc || (victim->desc->connected != CON_PLAYING) )
	{
		send_to_char ("This player has lost his link or is inside a pager or the like.\n\r",ch);
		return;
	}

	if (!new_name[0])
	{
		send_to_char ("Rename to what new name?\n\r",ch);
		return;
	}
	
	/* Insert check for clan here!! */
	/*

	if (victim->clan)
	{
		send_to_char ("This player is member of a clan, remove him from there first.\n\r",ch);
		return;
	}
	*/
	
	if (!check_parse_name(new_name))
	{
		send_to_char ("The new name is illegal.\n\r",ch);
		return;
	}

	/* First, check if there is a player named that off-line */	
#if !defined(machintosh) && !defined(MSDOS)
    sprintf( strsave, "%s%s%s%s", PLAYER_DIR, initial( new_name ),
	    	 "/", capitalize( new_name ) );
#else
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( new_name ) );
#endif

	fclose (fpReserve); /* close the reserve file */
	file = fopen (strsave, "r"); /* attempt to to open pfile */
	if (file)
	{
		send_to_char ("A player with that name already exists!\n\r",ch);
		fclose (file);
    	fpReserve = fopen( NULL_FILE, "r" ); /* is this really necessary these days? */
		return;		
	}
   	fpReserve = fopen( NULL_FILE, "r" );  /* reopen the extra file */

	/* Check .gz file ! */
#if !defined(machintosh) && !defined(MSDOS)
    sprintf( strsave, "%s%s%s%s.gz", PLAYER_DIR, initial( new_name ),
	    	 "/", capitalize( new_name ) );
#else
    sprintf( strsave, "%s%s.gz", PLAYER_DIR, capitalize( new_name ) );
#endif

	fclose (fpReserve); /* close the reserve file */
	file = fopen (strsave, "r"); /* attempt to to open pfile */
	if (file)
	{
		send_to_char ("A player with that name already exists in a compressed file!\n\r",ch);
		fclose (file);
    	fpReserve = fopen( NULL_FILE, "r" ); 
		return;		
	}
   	fpReserve = fopen( NULL_FILE, "r" );  /* reopen the extra file */

	if (get_char_world(ch,new_name)) /* check for playing level-1 non-saved */
	{
		send_to_char ("A player with the name you specified already exists!\n\r",ch);
		return;
	}

	/* Save the filename of the old name */

#if !defined(machintosh) && !defined(MSDOS)
    sprintf( strsave, "%s%s%s%s", PLAYER_DIR, initial( victim->name ),
	    	 "/", capitalize( victim->name ) );
#else
    sprintf( strsave, "%s%s", PLAYER_DIR, capitalize( victim->name ) );
#endif


	/* Rename the character and save him to a new file */
	/* NOTE: Players who are level 1 do NOT get saved under a new name */

	free_string (victim->name);
	victim->name = str_dup (capitalize(new_name));
	
	save_char_obj (victim);
	
	/* unlink the old file */
	unlink (strsave); /* unlink does return a value.. but we do not care */

	/* That's it! */
	
	send_to_char ("Character renamed.\n\r",ch);

	victim->position = POS_STANDING; /* I am laaazy */
	act ("$n has renamed you to $N!",ch,NULL,victim,TO_VICT);
			
}

/* get the 'short' name of an area (e.g. MIDGAARD, MIRROR etc. */
/* assumes that the filename saved in the AREA_DATA struct is something like midgaard.are */
char * area_name (AREA_DATA *pArea)
{
	static char buffer[64]; /* short filename */
	char  *period;

	assert (pArea != NULL);
	
	strncpy (buffer, pArea->filename, 64); /* copy the filename */	
	period = strchr (buffer, '.'); /* find the period (midgaard.are) */
	if (period) /* if there was one */
		*period = '\0'; /* terminate the string there (midgaard) */
		
	return buffer;	
}

typedef enum {exit_from, exit_to, exit_both} exit_status;

/* depending on status print > or < or <> between the 2 rooms */
void room_pair (ROOM_INDEX_DATA* left, ROOM_INDEX_DATA* right, exit_status ex, char *buffer)
{
	char *sExit;
	
	switch (ex)
	{
		default:
			sExit = "??"; break; /* invalid usage */
		case exit_from:
			sExit = "< "; break;
		case exit_to:
			sExit = " >"; break;
		case exit_both:
			sExit = "<>"; break;
	}
	
sprintf (buffer, "%5d %-26.26s %s%5d %-26.26s(%-8.8s)\n\r",
			  left->vnum, left->name, 
			  sExit,
			  right->vnum, right->name,
			  area_name(right->area)
	    );
}

/* for every exit in 'room' which leads to or from pArea but NOT both, print it */
void checkexits (ROOM_INDEX_DATA *room, AREA_DATA *pArea, char* buffer)
{
	char buf[MAX_STRING_LENGTH];
	int i;
	EXIT_DATA *exit;
	ROOM_INDEX_DATA *to_room;
	
	strcpy (buffer, "");
	for (i = 0; i < 6; i++)
	{
		exit = room->exit[i];
		if (!exit)
			continue;
		else
			to_room = exit->to_room;
		
		if (to_room)  /* there is something on the other side */

			if ( (room->area == pArea) && (to_room->area != pArea) )
			{ /* an exit from our area to another area */
			  /* check first if it is a two-way exit */
			
				if ( to_room->exit[opposite_dir[i]] &&
					to_room->exit[opposite_dir[i]]->to_room == room )
					room_pair (room,to_room,exit_both,buf); /* <> */
				else
					room_pair (room,to_room,exit_to,buf); /* > */
				
				strcat (buffer, buf);				
			}			
			else			
			if ( (room->area != pArea) && (exit->to_room->area == pArea) )
			{ /* an exit from another area to our area */

				if  (!
			    	 (to_room->exit[opposite_dir[i]] &&
				      to_room->exit[opposite_dir[i]]->to_room == room )
					)
				/* two-way exits are handled in the other if */
				{						
					room_pair (to_room,room,exit_from,buf);
					strcat (buffer, buf);
				}
				
			} /* if room->area */
			
	} /* for */
	
}

/* for now, no arguments, just list the current area */
void do_exlist (CHAR_DATA *ch, char * argument)
{
	AREA_DATA* pArea;
	ROOM_INDEX_DATA* room;
	int i;
	char buffer[MAX_STRING_LENGTH];
	
	pArea = ch->in_room->area; /* this is the area we want info on */
	for (i = 0; i < MAX_KEY_HASH; i++) /* room index hash table */
	for (room = room_index_hash[i]; room != NULL; room = room->next)
	/* run through all the rooms on the MUD */
	
	{
		checkexits (room, pArea, buffer);
		send_to_char (buffer, ch);
	}
}


/*
 * Code to create rings for married people.... Used in do_marry to give
 * the newly weds their initial rings, but can be used by imms also to
 * reimburse lost rings (for whatever reason)
 * Usage: newring <char>
 * The ring will be given to char automatically.
 * This is part of the marriage code by Canth (canth@xs4all.nl)
 * of Mythran
 */
void do_rings ( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *	spouse1;
    CHAR_DATA *	spouse2;
    char	buf  [ MAX_STRING_LENGTH ];
    OBJ_DATA  * ring;
    EXTRA_DESCR_DATA * ed;

    if ( !authorized( ch, "newring" ) )
	return;

    /* Don't be scared off by the humoungous code... It's not all that
	tricky... Honest */

    if (   !( spouse1 = get_char_world( ch, argument ) )
	|| !( spouse1->pcdata->spouse )
	|| !( spouse2 = get_char_world( ch, spouse1->pcdata->spouse ) ) )
    {
	send_to_char ( "They both need to be logged on to create the ring\n\r", ch );
	return;
    }

    /* Ok, they're both logged on... Let's see what sex spous1 is,
       and give them appropriate ring, and string it properly */
    switch( spouse1->sex )
    {
	case SEX_FEMALE:
	{	/* Diamond ring for females */
	    ring = create_object( get_obj_index( OBJ_VNUM_DIAMOND_RING ), 0 );
	    /* Now for sex of spouse to do inscription */
	    switch( spouse2->sex )
	    {
		case SEX_FEMALE:
		{
		    sprintf( buf, "This is the beautiful diamond ring given to you by your lovely\n\rwife %s at your wedding. It signifies your eternal love for eachother.", spouse2->name );
		    ring->description = str_dup( buf );
		    break;
		}
		case SEX_MALE:
		{
		    sprintf( buf, "This is the beautiful diamond ring given to you by your handsome\n\rhusband %s at your wedding. It signifies your eternal love for eachother.", spouse2->name );
		    ring->description = str_dup( buf );
		    break;
		}
		case SEX_NEUTRAL:
		default:
		{
		    sprintf( buf, "This is the beautiful diamond ring given to you by your\n\rspouse %s at your wedding. It signifies your eternal love for eachother.", spouse2->name );
		    ring->description = str_dup( buf );
		    break;
		}
	    }
	    /* Extra info for "look inscription" */
	    if ( !extra_descr_free )
	    {
		ed = alloc_perm( sizeof( *ed ) );
	    }
	    else
	    {
		ed = extra_descr_free;
		extra_descr_free = extra_descr_free->next;
	    }

	    ed->keyword = str_dup( "inscription" );
	    sprintf( buf, "The inscription reads:\n\rTo my lovely wife, yours forever, %s", spouse2->name );
	    ed->description = str_dup( buf );
	    ed->deleted = FALSE;
	    ed->next = ring->extra_descr;
	    ring->extra_descr = ed;
	    break;
	}
	case SEX_MALE:
	case SEX_NEUTRAL:
	default:
	{	/* Wedding band for males and neuters */
	    ring = create_object( get_obj_index( OBJ_VNUM_WEDDING_BAND ), 0 );
	    switch( spouse2->sex )
	    {	/* Description of the ring, depending on sex of spouse */
		case SEX_FEMALE:
		{
		    sprintf( buf, "This is the ring given to you by your beautifull wife %s\n\rat your wedding. It signifies your eternal love for eachother.", spouse2->name );
		    ring->description = str_dup( buf );
		    break;
		}
		case SEX_MALE:
		{
		    sprintf( buf, "This is the ring given to you by your handsome husband %s\n\rat your wedding. It signifies your eternal love for eachother.", spouse2->name );
		    ring->description = str_dup( buf );
		    break;
		}
		case SEX_NEUTRAL:
		default:
		{
		    sprintf( buf, "This is the ring given to you by your spouse %s at\n\ryour wedding. It signifies your eternal love for eachother.", spouse2->name );
		    ring->description = str_dup( buf );
		    break;
		}
	    }

	    /* Extra info for "look inscription" */

	    if ( !extra_descr_free )
	    {
		ed = alloc_perm( sizeof( *ed ) );
	    }
	    else
	    {
		ed = extra_descr_free;
		extra_descr_free = extra_descr_free->next;
	    }

	    ed->keyword = str_dup( "inscription" );
	    ed->deleted = FALSE;
	    ed->next = ring->extra_descr;
	    ring->extra_descr = ed;


	    switch( spouse1->sex )
	    {
		default:
		case SEX_MALE:
		{
		    sprintf( buf, "The inscription reads:\n\rTo my handsome prince... Forever yours, %s", spouse2->name );
		    ed->description = str_dup( buf );
		    break;
		}
		case SEX_NEUTRAL:
		{
		    sprintf( buf,"The inscription reads:\n\rForever love, %s", spouse2->name );
		    ed->description = str_dup( buf );
		    break;
		}
	    }
	}
    }

    obj_to_char ( ring, spouse1 );

    return;
}


/*
 * Marriage code... Marries and divorces 2 ppl.
 * the chars have to be in the same room as the implementor using the command.
 * This is part of the marriage code by Canth (canth@xs4all.nl)
 * of Mythran
 */
void do_marry( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *	spouse1;
    CHAR_DATA *	spouse2;
    char	arg1 [ MAX_INPUT_LENGTH ];
    char	arg2 [ MAX_INPUT_LENGTH ];
    char	buf  [ MAX_STRING_LENGTH ];

    if ( !authorized( ch, "marry" ) )
	return;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    /* Make sure two arguments are given */
    if ( arg2[0] == '\0' )
    {	/* Yep, none or only one name given */
	send_to_char( "Syntax:\n\r", ch );
	send_to_char( "marry <person1> <person2> [undo]\n\r", ch );
	send_to_char( "This marries person1 and person2\n\r", ch );
	send_to_char( "If undo is added, this is treated as a divorce.\n\r", ch );
	return;
    }

    /* Make sure 2 characteres are given */
    if ( !(spouse1 = get_char_room( ch, arg1 ) )
      || !(spouse2 = get_char_room( ch, arg2 ) ) )
    {
	send_to_char( "But they are not both present here!\n\r", ch );
	return;
    }

    /* Check for NPC's... They cannot marry. Maybe in a later version */
    if ( IS_NPC( spouse1 ) || IS_NPC( spouse2 ) )
    {
	send_to_char( "Mobiles cannot marry.\n\r", ch );
	send_to_char( "Support for this might be build into a next version.\n\r", ch );
	return;
    }

    /* Let's not marry or divorce one person to themsleves */
    if ( spouse1 == spouse2 )
    {	/* Yep, both are the same *bonk*/
	send_to_char( "You have to enter two DIFFERENT names!\n\r", ch );
	return;
    }

    /* Ok, let's get the divorce bit out of the way, so it won't bother us */
    /* later on (saves us some long if statements :) */
    if ( !str_cmp( argument, "undo" ) )
    {	/* Yep, it's a divorce.. *sniff* */
	if ( !spouse1->pcdata->spouse
	  || !spouse2->pcdata->spouse )
	{	/* One of them isn't wed */
	    sprintf( buf, "But %s isn't even married yet!\n\r",
		!spouse1->pcdata->spouse ? spouse1->name
			: spouse2->name );
	    send_to_char( buf, ch );
	    return;
	}
	else if ( ( str_cmp( spouse1->name, spouse2->pcdata->spouse ) )
	       || ( str_cmp( spouse2->name, spouse1->pcdata->spouse ) ) )
		/* Are married, but not to each other */
	    send_to_char( "But they're not married with each other!\n\r", ch );

	/* Ok, they _are_ married to each other... Now let's divorce them finally */
	spouse1->pcdata->spouse = NULL;		/* Fix by Maniac */
	spouse2->pcdata->spouse = NULL;		/* Fix by Maniac */
	/* Let's inform everyone of this sad event now */
	sprintf( buf, "%s and %s just divorced each other.\n\r", spouse1->name,
		spouse2->name );
	send_to_all_char( buf );
	return;
    }

    /* Ok, a marriage :) */
    if ( spouse1->pcdata->spouse
      || spouse2->pcdata->spouse )
    {	/* One of them's married already... */
	sprintf( buf,
		"%s is already married to %s. No bigammy allowed here\n\r",
		!spouse1->pcdata->spouse
			? spouse2->name : spouse1->name,
		!spouse1->pcdata->spouse
			? spouse2->pcdata->spouse : spouse1->pcdata->spouse );
	send_to_char( buf, ch );
	return;
    }

    /* Ok, they're both here, neither of them are married... Let's just
	do it then */
    spouse1->pcdata->spouse = str_dup( spouse2->name );
    spouse2->pcdata->spouse = str_dup( spouse1->name );

    /* The rings.... *twiddle*... C'mon... You didn't forget them did you?
       *sigh* Why do they always lose or forget the rings? */
    do_rings( ch, spouse1->name );
    do_rings( ch, spouse2->name );

    /* Let's inform everyone of this happy event now */
    sprintf( buf, "%s and %s have just married each other.\n\r", spouse1->name,
	spouse2->name );
    send_to_all_char( buf );
    return;
    /* That's all folks! */
}


/*
 * divorce function... Just calls marry with undo added
 * This is part of the marriage code by Canth (canth@xs4all.nl)
 * of Mythran
 */
/* This function disabled.. marry <char> <char> undo works..
 * Calling it using the divorce function crashes the mud.
 * (str_dup crash in ssm.c) - Canth 7/6/97
void do_divorce ( CHAR_DATA *ch, char *argument )
{
    char *	buf;

    strcpy ( buf, argument );
    buf = strcat( buf, " undo" );
    do_marry( ch, buf );
    return;
}
*/

/* Recursive cloning. To ensure cloning items in bags in backpacks in ... */
void recursive_clone( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone )
{
    OBJ_DATA *c_obj, *t_obj;

    for( c_obj = obj->contains; c_obj != NULL; c_obj = c_obj->next_content )
    {
	t_obj = create_object( c_obj->pIndexData, 0 );
	clone_object( c_obj, t_obj );
	obj_to_obj( t_obj, clone );
	recursive_clone( ch, c_obj, t_obj );
    }
}


/*
 * Cloning of objects and mobs
 * Copied from ROM 2.4 (By Russ Taylor)
 * Converted to envy by Canth (canth@xs4all.nl)
 */
void do_clone ( CHAR_DATA *ch, char *argument )
{
    char       arg [ MAX_INPUT_LENGTH ];
    char      *arg2;
    CHAR_DATA *mob = NULL;
    OBJ_DATA  *obj = NULL;

    if( !authorized( ch, "clone" ) )
	return;

    arg2 = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Clone what?\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "object" ) )
    {
	if( !(obj = get_obj_here( ch, arg2 ) ) )
	{
	    send_to_char( "You don't see that here.\n\r", ch );
	    return;
	}
    }
    else if ( (!str_prefix( arg, "mobile" ) ) || (!str_prefix( arg, "character" ) ) )
    {
	if( !(mob = get_char_room( ch, arg2 ) ) )
	{
	    send_to_char( "You don't see that here.\n\r", ch );
	    return;
	}
    }
    else
    {
	if( ( !( obj = get_obj_here( ch, arg ) ) ) && ( !( mob = get_char_room( ch, arg ) ) ) )
	{
	    send_to_char( "You don't see that here.\n\r", ch );
	    return;
	}
    }

    /* If object given, clone it */
    if( obj )
    {
	OBJ_DATA *clone;

	clone = create_object( obj->pIndexData, 0 );
	clone_object( obj, clone );
	if( obj->carried_by )
	    obj_to_char( clone, ch );
	else
	    obj_to_room( clone, ch->in_room );
	recursive_clone( ch, obj, clone );

	act( "$n has cloned $p.", ch, clone, NULL, TO_ROOM );
	act( "You clone $p.", ch, clone, NULL, TO_CHAR );
	return;
    }
    else if( mob )
    {
	CHAR_DATA *clone;
	OBJ_DATA  *new_obj;

	if( !IS_NPC( mob ) )
	{
	    send_to_char( "You can only clone mobiles.\n\r", ch );
	    return;
	}

	clone = create_mobile( mob->pIndexData );
	clone_mobile( mob, clone );

	for( obj = mob->carrying; obj != NULL; obj = obj->next_content )
	{
	    new_obj = create_object( obj->pIndexData, 0 );
	    clone_object( obj, new_obj );
	    recursive_clone( ch, obj, new_obj );
	    obj_to_char( new_obj, clone );
	    new_obj->wear_loc = obj->wear_loc;
	}
	char_to_room( clone, ch->in_room );
	act( "$n has cloned $N.", ch, NULL, clone, TO_ROOM );
	act( "You have cloned $N.", ch, NULL, clone, TO_CHAR );
	return;
    }
}
