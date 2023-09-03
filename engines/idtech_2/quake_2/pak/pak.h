/***************************************************************************
                          pak.h  -  description
                             -------------------
    begin                : Fre Feb 21 2003
    copyright            : (C) 2003-2004 by Dominik Jall
    email                : hackebeil20@web.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_SDL			//optional: used if functions for BMP and WAV handling are needed
#include "SDL.h"
#endif


typedef unsigned char byte;


typedef struct pakheader_t
{
  char head[4];
  unsigned int dir_offset;
  unsigned int dir_length;
} pakheader;


typedef struct dirsection_t
{
  char filename[56];
  unsigned int file_position;
  unsigned int file_length;
} dirsection;

typedef struct pakfile_t
{
  FILE *handle;
  pakheader header;
} pakfile;

void createPak(char* filename);					//creates a fresh new pak file
int extractPak(char *file, char *output, pakfile *fp);		//gets a file from the pak into outputfile
int insertPak(char* file, char* inpak, pakfile *fp);		//inserts a file into the pak
pakfile *openPak(char* filename);				//opens a pak and verifies it. returns a filepointer
int closePak(pakfile *fp);					//closes the opened pak
int checkPak(pakfile *fp);					//checks the pak (internally used only)
byte *readFile(char *filename, unsigned int *size);		//reads a generic file into a buffer
dirsection *inPak(char *file, pakfile *fp);			//checks if a file exists within a pakfile
dirsection *loadDirSection(pakfile *fp, int *x);        	//loads the dirsection of the given pakfile. x = number of files
void printDirTree(pakfile *fp);					// cheap helper function that printf's out the files in a pak
void printHelp();						// prints a help for command line use
int checkCmd(char* arr[]);					// checks if calling syntax is valid
int getLongestFile(dirsection *files, int n);

#ifdef USE_SDL
SDL_Surface *loadBMP(char* file, pakfile *fp);			//loads a BMP from a PAK into SDL_Surface directly
byte *extractMem(char *file, pakfile* fp, unsigned int *size);	//loads a file from PAK to RAM
#endif
