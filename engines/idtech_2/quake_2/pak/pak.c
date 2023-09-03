/***************************************************************************
                          pak.c  -  description
                             -------------------
    begin                : Fre Feb 21 17:42:09 CET 2003
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


#include "pak.h"

#define VERSION 0.3

#ifndef USE_SDL							//if you want to use SDL, you wouldn't want a main() here, since you have your own
int main(int argc, char* argv[])
{
	
	if(argc == 1)
	{
		printHelp();
		return 0;
	}
	if(argc == 2)
	{
		printf("pak: missing argument\n");
		return 1;
	}
	
	if(checkCmd(argv) == 0)
	{
		printf("pak: invalid command\n");
		return 1;
	}
	
	if(strncmp(argv[1], "c", 1) == 0 && argc == 3) //create a new PAK
	{
		createPak(argv[2]);
		//printf("Successfully created ");
		//printf(argv[2]);
		//printf("\n");
		return 0;
	}
		
	if(strncmp(argv[1], "p", 1) == 0 && argc == 3) //print contents of a pak
	{
				
		printDirTree(openPak(argv[2]));
		return 0;
	}
	
	if(strncmp(argv[1], "i", 1) == 0 && argc == 5) //insert file into pak
	{
			if(insertPak(argv[3], argv[4], openPak(argv[2])) != 0)
			{
				printf("pak: Couldn't insert given file.\n");
				return -1;
			} 
		
					
	} else if(strncmp(argv[1], "i", 1) == 0 && argc < 5)
		{
			printf("pak: missing argument\n");
			return 1;
		}
	
	if(strncmp(argv[1], "v", 1) == 0 && argc == 3) //verify PAK
	{
		
		switch(checkPak(openPak(argv[2])))
		{
			case -1:
			printf("pak: ");
			printf(argv[2]);
			printf(" --> Magic corrupt / Not a PAK file\n");
			return -1;
			
			case -2:
			printf("pak: ");
			printf(argv[2]);
			printf(" --> Header corrupt / PAK file broken\n");
			return -1;
			
			case 0:
				
			printf("pak: ");
			printf(argv[2]);
			printf(" --> OK\n");
						
		}
		
		
		return 0;
	}
		
	if(strncmp(argv[1], "e", 1) == 0 && argc == 5) //extract file from pak
	{
			if(extractPak(argv[3], argv[4], openPak(argv[2])) != 0)
			{
				printf("pak: Couldn't extract given file.\n");
				return -1;
			} 
		
					
	} else if(strncmp(argv[1], "e", 1) == 0 && argc < 5)
		{
			printf("pak: missing argument\n");
			return 1;
		}
		
		
	return 0;
		
}
#endif

void createPak(char* filename)
{

  pakfile mypak;
   
  mypak.header.head[0] = 'P';
  mypak.header.head[1] = 'A';
  mypak.header.head[2] = 'C';
  mypak.header.head[3] = 'K';
  
  mypak.header.dir_offset = sizeof(mypak.header.head) + 1;
  mypak.header.dir_length = 1;

  mypak.handle = fopen(filename, "wb");

  fwrite(&mypak.header,sizeof(mypak.header),1,mypak.handle);
  
  fclose(mypak.handle);
  
}

int checkPak(pakfile *fp)
{
  if( fp->header.head[0] != 'P' || fp->header.head[1] != 'A' || fp->header.head[2] != 'C' || fp->header.head[3] != 'K')
    return -1;  //ident corrupt

  if( fp->header.dir_offset < (sizeof(fp->header.head) + 1) || fp->header.dir_length < 1)
    return -2;  //header corrupt

    
  return 0;
}


pakfile *openPak(char* filename)
{
  pakfile *mypak;
  FILE *fp;

    
  fp = fopen(filename, "r+");
    

  mypak = malloc(sizeof(pakfile));
  mypak->handle = fp;

  
  
  if(mypak->handle == NULL)
     return NULL;

    
     fread(&mypak->header, sizeof(pakheader), 1, mypak->handle);
    
  return mypak;
}


int insertPak(char* file, char* inpak, pakfile *fp)
{

    unsigned int count;
    dirsection sect;
    dirsection *files;
    int x =0;
    
    if(sizeof(inpak) > 56) //to stay compatible to Quake 2 format, this may not be longer than 56 char
      return -1;

                  
    byte* buffer = readFile(file, &count);
       
    if(fp->header.dir_length == 1)
       {
         fseek(fp->handle, 4, SEEK_SET);  //go to directory offset-record
         fp->header.dir_offset = sizeof(fp->header) + sizeof(byte)*count;
         fp->header.dir_length = sizeof(dirsection);
         //write new offset and length
         fwrite(&fp->header.dir_offset, sizeof(fp->header.dir_offset), 1, fp->handle);
         fwrite(&fp->header.dir_length, sizeof(fp->header.dir_length), 1, fp->handle);

 
         //now write the raw binary data
         fwrite(buffer, sizeof(byte), count, fp->handle);

            
         strcpy(sect.filename, inpak);
         sect.file_position = 12;
         sect.file_length = sizeof(byte) * count;
         
         //now write the dirsection
         fwrite(&sect, sizeof(sect), 1, fp->handle);
       }
       else
       {
         //first check if the file already exists in the pak
         //for this purpose we read all the dirsections into a buffer and then check them


         if( fp->header.dir_length % 64 != 0 )  // corrupted file ?
              return -1;                      // oh my gosh, check yer HD !

         //now check how many files are in the pak
         x = fp->header.dir_length / 64;  //guaranteed to be even

         //allocate space for all the files including one new
         files = malloc((x+1) * sizeof(dirsection));

         //read all the dirsections into 'files'
         fseek(fp->handle, fp->header.dir_offset, SEEK_SET);  //go to directory offset

         fread(files, sizeof(dirsection), x, fp->handle);

         //now check if the file exists
         int i;
         for( i = 0; i < x; i++)
         {

           char *name = files[i].filename;

           if( strcmp(name, inpak) == 0 )
              return -1;  // already exists
         }

         //now calculate the new offset
         fseek(fp->handle, 4, SEEK_SET);  //go to directory offset-record
         fp->header.dir_offset = fp->header.dir_offset + count;
         fp->header.dir_length = fp->header.dir_length + sizeof(dirsection);
         //write new offset and length
         fwrite(&fp->header.dir_offset, sizeof(fp->header.dir_offset), 1, fp->handle);
         fwrite(&fp->header.dir_length, sizeof(fp->header.dir_length), 1, fp->handle);

         //now go to end of binary data and write the file
         fseek(fp->handle, (fp->header.dir_offset - count), SEEK_SET);

         fwrite(buffer, sizeof(byte), count, fp->handle);

         strcpy(files[x].filename, inpak);
        
         files[x].file_position = fp->header.dir_offset - count;
         files[x].file_length = sizeof(byte) * count;

         //now write the entire dirsection, since the old one has been overwritten by binary data
         fwrite(files, sizeof(dirsection), x+1, fp->handle);
                 
       }                     

       
       free(buffer);

return 0;
}



byte *readFile(char *filename, unsigned int *size)
{
    FILE *fil;
    byte c;
    int b;
    unsigned int count =0;

    
    fil = fopen(filename, "r");
    
    
    if( fil == NULL )
      return NULL;

    while(fread(  &c, sizeof(c), 1, fil) > 0)
      count++;
    
    //now we have the size of the file
    byte *buffer = malloc(count * sizeof(byte));
    
    rewind(fil);
    count = 0;
    //now read the data into the buffer
    while((b = fgetc(fil)) != EOF)
      {
        buffer[count] = b;
        count++;
      }
     
    fclose(fil);
   *size = count;
  return buffer;
}


int closePak(pakfile *fp)
{
  fclose(fp->handle);
  free(fp);
  return 0;
}

dirsection *inPak(char *file, pakfile *fp)
{

         dirsection *sect = malloc(sizeof(dirsection));
                   
         int x =0;

         dirsection *files = loadDirSection(fp, &x);
                   
         //now check if the file exists
         int i;
         for( i = 0; i < x; i++)
         {

           char *name = files[i].filename;
           unsigned int position = files[i].file_position;
           unsigned int length = files[i].file_length;
           
           if( strcmp(name, file) == 0 )
              {
                strcpy(sect->filename, name);
                sect->file_position = position;
                sect->file_length = length;

                free(files);return sect;}  // exists
         }

         free(files);
         return NULL; // does not exist
}



int extractPak(char *file, char *output, pakfile *fp)
{
  FILE *fil;

  if(fp == NULL)
	  return -1;
  //first, test if the file exists in the pak
  dirsection *mysect = inPak(file, fp);

  if( mysect == NULL )
      return -1;   //not found
      
      //open the output file
      fil = fopen(output, "wb");

      //now go to the address in the pak
      fseek(fp->handle, mysect->file_position, SEEK_SET);
           
      byte *buffer = malloc(mysect->file_length * sizeof(byte));

    
      //now read the data into the buffer
      fread(buffer, sizeof(byte), mysect->file_length, fp->handle);

            
      //now write the buffer to the output file
      fwrite(buffer, sizeof(byte), mysect->file_length, fil);
      fclose(fil);
      free(buffer);
      
 
return 0;
}

dirsection *loadDirSection(pakfile *fp, int *x)
{
      dirsection *files;
         

         //see how many files are in the pak
         *x = fp->header.dir_length / 64;  //guaranteed to be even

         //allocate space for all the files including one new (for possible insertion)
         files = malloc(*x * sizeof(dirsection));

         //read all the dirsections into 'files'
         fseek(fp->handle, fp->header.dir_offset, SEEK_SET);  //go to directory offset

         fread(files, sizeof(dirsection), *x, fp->handle);

       return files;
}
  
void printDirTree(pakfile *fp)
{
  int x;

	
  if(fp == NULL) { printf("pak: File not found.\n"); return;}
	
  dirsection *myfiles = loadDirSection(fp, &x);
  
  char *name;
  x--;
  while( x >= 0)
  {
   name = myfiles[x].filename;
  printf("%25s", name);                  //give out the pathname of the file in pak
  printf("\t\t");
  printf("%8i", myfiles[x].file_length); //give out the size in bytes
  printf("  Bytes\n");
  x--;
  }
}
void printHelp()
{
	printf("\nPAK "); printf("%g", VERSION); printf(" by Dominik Jall\n");
	printf("-----------------------\n\n");
	printf("Create and modify Quake I + II PAKfiles\n\n");
	printf("Call: pak [COMMAND] [FILE] [FILE] [FILE]\n");
	printf("\n\nCOMMANDs are:\n");
	printf("c\tcreate a new (empty) pakfile\n");
	printf("p\tprint out the directory of a pakfile\n");
	printf("i\tinsert a file into a pakfile\n");
	printf("v\tverify the integrity of a pakfile\n");
	printf("e\textract a file from a pakfile\n\n");
	printf("Hints for insertion and extraction:\n");
	printf("If you want to insert a file into a PAK you need to specify three filenames:\n");
	printf("1. The pakfile we are working with\n");
	printf("2. The file in the real filesystem (something like ~/myfile.txt)\n");
	printf("3. The file in the virtual filesystem of the PAK\n\n");
	printf("Examples:\n pak i ~/myPAK.pak ~/picture.xpm bitmaps/firstpic.xpm\n");
	printf(" pak e ~/myPAK.pak bitmaps/firstpic.xpm ~/picture.xpm\n");
	printf("\nNOTE: If you want to extract a file, the second argument is the file within the PAK\n\n");
	printf("Questions / Comments are very welcome. Yuh cyan mail me at hackebeil20@web.de\n\n");
	
	
}

int checkCmd(char* arr[])
{
	char a[] = {'c','p','i','v','e'};
	
	int b;
	int c = 0;
	
	for(b = 0; b < 5; b++)
	{
			
		int f =	strncmp(arr[1], &a[b], 1);
		
		if(f == 0)
			c=1;
		
	}
	
	
	return c; //if c == 0 then no valid command has been passed
}

int getLongestFile(dirsection *files, int n)
{
	int longest = 0;
	int curr;
	while( n >= 0)
	{
		curr = strlen(files[n].filename);
		if( curr > longest )
			longest = curr;
	n--;	
	}
	
return longest;
}	

#ifdef USE_SDL
SDL_Surface *loadBMP(char* file, pakfile *fp)
{
	unsigned int size;
	SDL_RWops *rw;
	SDL_Surface *plane;
	// this function uses the undocumented SDL_RWops-feature that allows
	// loading images from almost any source, like memorypointers, pipes etc.

	//first, read the file within the pak into a buffer
	byte *buffer = extractMem(file, fp, &size);
	
	if(buffer == NULL)
		return NULL;
	
	//now that we have the file as memory, we need to make a SDL_RWop-struct from it
	rw = SDL_RWFromMem(buffer, size);
	
	//now load the BMP in an ordinary way
	plane = SDL_LoadBMP_RW(rw, 0);
		
	//at last, free all unnecessary stuff
	free(buffer);
	SDL_FreeRW(rw);
	
	return plane;
}

byte *extractMem(char *file, pakfile* fp, unsigned int *size)
{
	 if(fp == NULL)
	  return NULL;
  	//test if the file exists in the pak
  	dirsection *mysect = inPak(file, fp);

  if( mysect == NULL )
      return NULL;   //not found
  
      //now go to the address in the pak
      fseek(fp->handle, mysect->file_position, SEEK_SET);
           
      byte *buffer = malloc(mysect->file_length * sizeof(byte));
    
      *size = mysect->file_length;
      
     //now read the data into the buffer
      fread(buffer, sizeof(byte), mysect->file_length, fp->handle);

    return buffer;
}        
#endif
