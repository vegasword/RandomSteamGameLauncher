#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

int main(int argc, char **argv)
{
  if (argc == 1)
  {
    fprintf(stderr, "Usage: RandomSteamLauncher \"YourDisk:/Path/To/Steam/steamapps/libraryfolders.vdf\"");
    return 1;
  }

  srand((unsigned int)time(NULL));
  
  long fileSize = 0;
  FILE *steamLibraryFile = fopen(argv[1], "r");
  if (steamLibraryFile)
  {
    fseek(steamLibraryFile, 0, SEEK_END);
    fileSize = ftell(steamLibraryFile);
    rewind(steamLibraryFile);
  }
  else
  {
    fprintf(stderr, "Failed to open \"%s\" !\n", argv[1]);
    perror("Reason");
    return 1;
  }

  char *steamLibraryfileBuffer = malloc(fileSize + 1);
  fread(steamLibraryfileBuffer, fileSize, 1, steamLibraryFile);
  fclose(steamLibraryFile);
  
  int appsCount = 0;
  int appIds[4096];
  {
    char *apps = strstr(steamLibraryfileBuffer, "\"apps\"") + 6;
    char idBuffer[10];
  
    int endOfParsing = 0;
    while (*apps != '}')
    {
      int doubleQuoteSkipCount = (appsCount > 0) ? 4 : 1;
      while (doubleQuoteSkipCount--)
      {
        while (*apps != '"')
        {
          endOfParsing = (*apps == '}');
          if (endOfParsing) break;
          apps++;
        }
        if (endOfParsing) break;
        apps++;
      }

      if (endOfParsing) break;
      
      char *nextDoubleQuote = apps;
      while (*nextDoubleQuote != '"') nextDoubleQuote++;

      int numberLength = (int)(nextDoubleQuote - apps);
      memcpy(idBuffer, apps, numberLength);
      appIds[appsCount] = atoi(idBuffer);
      appsCount++;  
    }
  }
  
  int randomGame = appIds[rand() % (appsCount + 1)];
  char command[64];
#ifdef _WIN32
  snprintf(command, sizeof(command), "start \"\" \"steam://rungameid/%d\"", randomGame);
#elif defined(__APPLE__) //TODO: Test it
  snprintf(command, sizeof(command), "open \"steam://rungameid/%d\"", randomGame);
#elif defined(__linux__) //TODO: Test it
  snprintf(command, sizeof(command), "xdg-open \"steam://rungameid/%d\"", randomGame);
#else
  fprintf(stderr, "Unsupported operating system :'(\n");
  return 1;
#endif
  system(command);
  
  free(steamLibraryfileBuffer);
  return 0;
}
