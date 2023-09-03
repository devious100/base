/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

6/20/2003 - Modified by Vertigo Software, Inc.
11/17/2007 - Modified by Greg Dolley.
*/
// console.c

#include "client.h"

//#define _MANAGED 1 // ***GREGS_VC9_PORT_MOD*** -- added this line because the IDE messes up sometimes with the syntax highlighting and thinks the symbol is undefined if you define it in the project settings

#if _MANAGED
	// display the CLR version on the main Quake window
	#using <mscorlib.dll>
	#include <vcclr.h>
	using namespace System;
	char clrversion[100];
#endif


console_t	con;

cvar_t		*con_notifytime;


#define		MAXCMDLINE	256
extern	char	key_lines[32][MAXCMDLINE];
extern	int		edit_line;
extern	int		key_linepos;


		

void DrawString (int x, int y, char *s)
{
	while (*s)
	{
		re.DrawChar (x, y, *s);
		x+=8;
		s++;
	}
}

void DrawAltString (int x, int y, char *s)
{
	while (*s)
	{
		re.DrawChar (x, y, *s ^ 0x80);
		x+=8;
		s++;
	}
}


void Key_ClearTyping (void)
{
	key_lines[edit_line][1] = 0;	// clear any typing
	key_linepos = 1;
}

/*
================
Con_ToggleConsole_f
================
*/
void Con_ToggleConsole_f (void)
{
	SCR_EndLoadingPlaque ();	// get rid of loading plaque

	if (cl.attractloop)
	{
		Cbuf_AddText ("killserver\n");
		return;
	}

	if (cls.state == ca_disconnected)
	{	// start the demo loop again
		Cbuf_AddText ("d1\n");
		return;
	}

	Key_ClearTyping ();
	Con_ClearNotify ();

	if (cls.key_dest == key_console)
	{
		M_ForceMenuOff ();
		Cvar_Set ("paused", "0");
	}
	else
	{
		M_ForceMenuOff ();
		cls.key_dest = key_console;	

		if (Cvar_VariableValue ("maxclients") == 1 
			&& Com_ServerState ())
			Cvar_Set ("paused", "1");
	}
}

/*
================
Con_ToggleChat_f
================
*/
void Con_ToggleChat_f (void)
{
	Key_ClearTyping ();

	if (cls.key_dest == key_console)
	{
		if (cls.state == ca_active)
		{
			M_ForceMenuOff ();
			cls.key_dest = key_game;
		}
	}
	else
		cls.key_dest = key_console;
	
	Con_ClearNotify ();
}

/*
================
Con_Clear_f
================
*/
void Con_Clear_f (void)
{
	memset (con.text, ' ', CON_TEXTSIZE);
}

						
/*
================
Con_Dump_f

Save the console contents out to a file
================
*/
void Con_Dump_f (void)
{
	int		l, x;
	char	*line;
	FILE	*f;
	char	buffer[1024];
	char	name[MAX_OSPATH];

	if (Cmd_Argc() != 2)
	{
		Com_Printf ("usage: condump <filename>\n");
		return;
	}

	Com_sprintf (name, sizeof(name), "%s/%s.txt", FS_Gamedir(), Cmd_Argv(1));

	Com_Printf ("Dumped console text to %s.\n", name);
	FS_CreatePath (name);
	/*f = */fopen_s(&f, name, "w");
	if (!f)
	{
		Com_Printf ("ERROR: couldn't open.\n");
		return;
	}

	// skip empty lines
	for (l = con.current - con.totallines + 1 ; l <= con.current ; l++)
	{
		line = con.text + (l%con.totallines)*con.linewidth;
		for (x=0 ; x<con.linewidth ; x++)
			if (line[x] != ' ')
				break;
		if (x != con.linewidth)
			break;
	}

	// write the remaining lines
	buffer[con.linewidth] = 0;
	for ( ; l <= con.current ; l++)
	{
		line = con.text + (l%con.totallines)*con.linewidth;
		strncpy_s (buffer, line, con.linewidth); // ***GREGS_VC9_PORT_MOD*** -- changed to safe version of CRT function
		for (x=con.linewidth-1 ; x>=0 ; x--)
		{
			if (buffer[x] == ' ')
				buffer[x] = 0;
			else
				break;
		}
		for (x=0; buffer[x]; x++)
			buffer[x] &= 0x7f;

		fprintf (f, "%s\n", buffer);
	}

	fclose (f);
}

						
/*
================
Con_ClearNotify
================
*/
void Con_ClearNotify (void)
{
	int		i;
	
	for (i=0 ; i<NUM_CON_TIMES ; i++)
		con.times[i] = 0;
}

						
/*
================
Con_MessageMode_f
================
*/
void Con_MessageMode_f (void)
{
	chat_team = false;
	cls.key_dest = key_message;
}

/*
================
Con_MessageMode2_f
================
*/
void Con_MessageMode2_f (void)
{
	chat_team = true;
	cls.key_dest = key_message;
}

/*
================
Con_CheckResize

If the line width has changed, reformat the buffer.
================
*/
void Con_CheckResize (void)
{
	int		i, j, width, oldwidth, oldtotallines, numlines, numchars;
	char	tbuf[CON_TEXTSIZE];

	width = (viddef.width >> 3) - 2;

	if (width == con.linewidth)
		return;

	if (width < 1)			// video hasn't been initialized yet
	{
		width = 38;
		con.linewidth = width;
		con.totallines = CON_TEXTSIZE / con.linewidth;
		memset (con.text, ' ', CON_TEXTSIZE);
	}
	else
	{
		oldwidth = con.linewidth;
		con.linewidth = width;
		oldtotallines = con.totallines;
		con.totallines = CON_TEXTSIZE / con.linewidth;
		numlines = oldtotallines;

		if (con.totallines < numlines)
			numlines = con.totallines;

		numchars = oldwidth;
	
		if (con.linewidth < numchars)
			numchars = con.linewidth;

		memcpy (tbuf, con.text, CON_TEXTSIZE);
		memset (con.text, ' ', CON_TEXTSIZE);

		for (i=0 ; i<numlines ; i++)
		{
			for (j=0 ; j<numchars ; j++)
			{
				con.text[(con.totallines - 1 - i) * con.linewidth + j] =
						tbuf[((con.current - i + oldtotallines) %
							  oldtotallines) * oldwidth + j];
			}
		}

		Con_ClearNotify ();
	}

	con.current = con.totallines - 1;
	con.display = con.current;
}


/*
================
Con_Init
================
*/
void Con_Init (void)
{
	con.linewidth = -1;

	Con_CheckResize ();
	
	Com_Printf ("Console initialized.\n");

//
// register our commands
//
	con_notifytime = Cvar_Get ("con_notifytime", "3", 0);

	Cmd_AddCommand ("toggleconsole", Con_ToggleConsole_f);
	Cmd_AddCommand ("togglechat", Con_ToggleChat_f);
	Cmd_AddCommand ("messagemode", Con_MessageMode_f);
	Cmd_AddCommand ("messagemode2", Con_MessageMode2_f);
	Cmd_AddCommand ("clear", Con_Clear_f);
	Cmd_AddCommand ("condump", Con_Dump_f);
	con.initialized = true;
		
	#if _MANAGED
		// copy the CLR version into the string so it can be displayed later
		memset (clrversion, 0, sizeof(clrversion));
		//Version *ver = System::Environment::Version;
		//System::Object *o[] = {ver->Major.ToString(), ver->Minor.ToString(), ver->Build.ToString()};
		//String *v = String::Format("Running on the .NET Framework {0}.{1}.{2}", o);
		Version ^ver = System::Environment::Version; // ***GREGS_VC9_PORT_MOD*** -- Changed "*" to "^"
		array<System::Object ^> ^o = {ver->Major.ToString(), ver->Minor.ToString(), ver->Build.ToString()}; // ***GREGS_VC9_PORT_MOD** -- Changed to array<> semantics
		String ^v = String::Format("Running on the .NET Framework {0}.{1}.{2}", o); // ***GREGS_VC9_PORT_MOD*** -- Took out "S" qualifier in string because it's obsolete and changed "*" to "^"
		//String ^v = String::Format("Running on the .NET Framework 3.5 Pre-Release Version"); // ***GREGS_VC9_PORT_MOD*** -- Took out "S" qualifier in string because it's obsolete and changed "*" to "^"

		// copy string
		/*const wchar_t __pin**/ interior_ptr<const wchar_t> p = PtrToStringChars(v); // ***GREGS_VC9_PORT_MOD*** -- changed to current pinning pointer semantics in the Managed Extensions for C++ .NET 2005 specification
		for (int i=0; i < v->Length; i++)
			clrversion[i] = p[i];
	#endif
}


/*
===============
Con_Linefeed
===============
*/
void Con_Linefeed (void)
{
	con.x = 0;
	if (con.display == con.current)
		con.display++;
	con.current++;
	memset (&con.text[(con.current%con.totallines)*con.linewidth]
	, ' ', con.linewidth);
}

/*
================
Con_Print

Handles cursor positioning, line wrapping, etc
All console printing must go through this in order to be logged to disk
If no console is visible, the text will appear at the top of the game window
================
*/
void Con_Print (char *txt)
{
	int		y;
	int		c, l;
	static int	cr;
	int		mask;

	if (!con.initialized)
		return;

	if (txt[0] == 1 || txt[0] == 2)
	{
		mask = 128;		// go to colored text
		txt++;
	}
	else
		mask = 0;


	while ( (c = *txt) )
	{
	// count word length
		for (l=0 ; l< con.linewidth ; l++)
			if ( txt[l] <= ' ')
				break;

	// word wrap
		if (l != con.linewidth && (con.x + l > con.linewidth) )
			con.x = 0;

		txt++;

		if (cr)
		{
			con.current--;
			cr = false;
		}

		
		if (!con.x)
		{
			Con_Linefeed ();
		// mark time for transparent overlay
			if (con.current >= 0)
				con.times[con.current % NUM_CON_TIMES] = cls.realtime;
		}

		switch (c)
		{
		case '\n':
			con.x = 0;
			break;

		case '\r':
			con.x = 0;
			cr = 1;
			break;

		default:	// display character and advance
			y = con.current % con.totallines;
			con.text[y*con.linewidth+con.x] = c | mask | con.ormask;
			con.x++;
			if (con.x >= con.linewidth)
				con.x = 0;
			break;
		}
		
	}
}


/*
==============
Con_CenteredPrint
==============
*/
void Con_CenteredPrint (char *text)
{
	int		l;
	char	buffer[1024];

	l = (int)strlen(text);
	l = (con.linewidth-l)/2;
	if (l < 0)
		l = 0;
	memset (buffer, ' ', l);
	strcpy_s(buffer+l, 1024, text); // ***GREGS_VC9_PORT_MOD*** -- changed to safe version of strcpy
	strcat_s (buffer, "\n"); // ***GREGS_VC9_PORT_MOD*** -- changed to safe version of CRT function
	Con_Print (buffer);
}

/*
==============================================================================

DRAWING

==============================================================================
*/


/*
================
Con_DrawInput

The input line scrolls horizontally if typing goes beyond the right edge
================
*/
void Con_DrawInput (void)
{
	int		y;
	int		i;
	char	*text;

	if (cls.key_dest == key_menu)
		return;
	if (cls.key_dest != key_console && cls.state == ca_active)
		return;		// don't draw anything (always draw if not active)

	text = key_lines[edit_line];
	
// add the cursor frame
	text[key_linepos] = 10+((int)(cls.realtime>>8)&1);
	
// fill out remainder with spaces
	for (i=key_linepos+1 ; i< con.linewidth ; i++)
		text[i] = ' ';
		
//	prestep if horizontally scrolling
	if (key_linepos >= con.linewidth)
		text += 1 + key_linepos - con.linewidth;
		
// draw it
	y = con.vislines-16;

	for (i=0 ; i<con.linewidth ; i++)
		re.DrawChar ( (i+1)<<3, con.vislines - 22, text[i]);

// remove cursor
	key_lines[edit_line][key_linepos] = 0;
}


/*
================
Con_DrawNotify

Draws the last few lines of output transparently over the game top
================
*/
void Con_DrawNotify (void)
{
	int		x, v;
	char	*text;
	int		i;
	int		time;
	char	*s;
	int		skip;

	v = 0;
	for (i= con.current-NUM_CON_TIMES+1 ; i<=con.current ; i++)
	{
		if (i < 0)
			continue;
		time = con.times[i % NUM_CON_TIMES];
		if (time == 0)
			continue;
		time = cls.realtime - time;
		if (time > con_notifytime->value*1000)
			continue;
		text = con.text + (i % con.totallines)*con.linewidth;
		
		for (x = 0 ; x < con.linewidth ; x++)
			re.DrawChar ( (x+1)<<3, v, text[x]);

		v += 8;
	}


	if (cls.key_dest == key_message)
	{
		if (chat_team)
		{
			DrawString (8, v, "say_team:");
			skip = 11;
		}
		else
		{
			DrawString (8, v, "say:");
			skip = 5;
		}

		s = chat_buffer;
		if (chat_bufferlen > (viddef.width>>3)-(skip+1))
			s += chat_bufferlen - ((viddef.width>>3)-(skip+1));
		x = 0;
		while(s[x])
		{
			re.DrawChar ( (x+skip)<<3, v, s[x]);
			x++;
		}
		re.DrawChar ( (x+skip)<<3, v, 10+((cls.realtime>>8)&1));
		v += 8;
	}


	#if _MANAGED
		// display CLR version if there are not any pending messages to display
		if (!cl_timedemo->value && !v)
		{
			int line_pos = 0;

			// vc++ message
			static char* msg1 = "Built with MSVC++ v9.0 Beta 2 (\"Orcas\") & Managed Extensions for C++ .NET 2005";
			int len = (int)strlen(msg1);
			for (x = 0 ; x < len ; x++)
				re.DrawChar ( (x+1)<<3, v, msg1[x]);

			line_pos += 10;

			// [Greg Dolley] - I commented out these next three lines because for some strange reason it was
			//                 always displaying the wrong version 2.0.50727
			// clr version message
			//len = (int)strlen(clrversion);
			//for (x = 0 ; x < len ; x++)
			//	re.DrawChar ( (x+1)<<3, v+line_pos, clrversion[x]);

			//line_pos += 10;
			
			// vertigo message
			static char* msg3 = "Ported by Greg Dolley";
			len = (int)strlen(msg3);
			for (x = 0 ; x < len ; x++)
				re.DrawChar ( (x+1)<<3, v+line_pos, msg3[x]);
		}
	#endif
		
	if (v)
	{
		SCR_AddDirtyPoint (0,0);
		SCR_AddDirtyPoint (viddef.width-1, v);
	}
}

/*
================
Con_DrawConsole

Draws the console with the solid background
================
*/
void Con_DrawConsole (float frac)
{
	int				i, j, x, y, n;
	int				rows;
	char			*text;
	int				row;
	int				lines;
	char			version[64];
	char			dlbar[1024];

	lines = viddef.height * frac;
	if (lines <= 0)
		return;

	if (lines > viddef.height)
		lines = viddef.height;

// draw the background
	re.DrawStretchPic (0, -viddef.height+lines, viddef.width, viddef.height, "conback");
	SCR_AddDirtyPoint (0,0);
	SCR_AddDirtyPoint (viddef.width-1,lines-1);

	Com_sprintf (version, sizeof(version), "v%4.2f", VERSION);
	for (x=0 ; x<5 ; x++)
		re.DrawChar (viddef.width-44+x*8, lines-12, 128 + version[x] );
		
// draw the text
	con.vislines = lines;
	
#if 0
	rows = (lines-8)>>3;		// rows of text to draw

	y = lines - 24;
#else
	rows = (lines-22)>>3;		// rows of text to draw

	y = lines - 30;
#endif

// draw from the bottom up
	if (con.display != con.current)
	{
	// draw arrows to show the buffer is backscrolled
		for (x=0 ; x<con.linewidth ; x+=4)
			re.DrawChar ( (x+1)<<3, y, '^');
	
		y -= 8;
		rows--;
	}
	
	row = con.display;
	for (i=0 ; i<rows ; i++, y-=8, row--)
	{
		if (row < 0)
			break;
		if (con.current - row >= con.totallines)
			break;		// past scrollback wrap point
			
		text = con.text + (row % con.totallines)*con.linewidth;

		for (x=0 ; x<con.linewidth ; x++)
			re.DrawChar ( (x+1)<<3, y, text[x]);
	}

//ZOID
	// draw the download bar
	// figure out width
	if (cls.download) {
		if ((text = strrchr(cls.downloadname, '/')) != NULL)
			text++;
		else
			text = cls.downloadname;

		x = con.linewidth - ((con.linewidth * 7) / 40);
		y = x - (int)strlen(text) - 8;
		i = con.linewidth/3;
		if (strlen(text) > i) {
			y = x - i - 11;
			strncpy_s(dlbar, text, i); // ***GREGS_VC9_PORT_MOD*** -- changed to safe version of CRT function
			dlbar[i] = 0;
			strcat_s(dlbar, "..."); // ***GREGS_VC9_PORT_MOD*** -- changed to safe version of CRT function
		} else
			strcpy_s(dlbar, text); // ***GREGS_VC9_PORT_MOD*** -- changed to safe version of CRT function
		strcat_s(dlbar, ": "); // ***GREGS_VC9_PORT_MOD*** -- changed to safe version of CRT function
		i = (int)strlen(dlbar);
		dlbar[i++] = '\x80';
		// where's the dot go?
		if (cls.downloadpercent == 0)
			n = 0;
		else
			n = y * cls.downloadpercent / 100;
			
		for (j = 0; j < y; j++)
			if (j == n)
				dlbar[i++] = '\x83';
			else
				dlbar[i++] = '\x81';
		dlbar[i++] = '\x82';
		dlbar[i] = 0;

		sprintf_s(dlbar + strlen(dlbar), 1024-strlen(dlbar), " %02d%%", cls.downloadpercent); // ***GREGS_VC9_PORT_MOD*** -- changed to safe version of sprintf

		// draw it
		y = con.vislines-12;
		for (i = 0; i < strlen(dlbar); i++)
			re.DrawChar ( (i+1)<<3, y, dlbar[i]);
	}
//ZOID

// draw the input prompt, user text, and cursor if desired
	Con_DrawInput ();
}


