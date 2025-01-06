#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char **argv)
{
  if (argc == 1)
  {
    fprintf(stderr, "Usage: RandomSteamLauncher \"YourDisk:/Path/To/Steam/steamapps/libraryfolders.vdf\"");
    return 1;
  }

  char fullPath[_MAX_PATH];
  _fullpath(fullPath, argv[1], _MAX_PATH);
  
  long fileSize = 0;
  FILE *libraryFile = fopen(fullPath, "r");
  if (libraryFile && !ferror(libraryFile))
  {
    fseek(libraryFile, 0, SEEK_END);
    fileSize = ftell(libraryFile);
    rewind(libraryFile);
  }
  else
  {
    fprintf(stderr, "Failed to open \"%s\" !\n", fullPath);
    perror("Reason");
    return 1;
  }

  char *fileBuffer = malloc(fileSize + 1);
  fread(fileBuffer, fileSize, 1, libraryFile);
  
  char *apps = strstr(fileBuffer, "\"apps\"");
  printf("%s", apps);

  //TODO: Parse all the game id, filter with a banlist file and launch a ramdom game using steam://apprunid/{id}
  
  fclose(libraryFile);
  return 0;
}
