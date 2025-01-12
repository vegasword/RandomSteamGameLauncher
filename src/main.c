#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#define MAX_GAMES 1000
#define APP_ID_BUFFER_SIZE 16

char *ReadEntireFile(const char *path, const char *mode)
{
  FILE *file = fopen(path, mode);
  if (file)
  {
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *fileBuffer = (char *)malloc(fileSize + 1);
    if (fileBuffer)
    {
      fread(fileBuffer, fileSize, 1, file);
      fileBuffer[fileSize] = '\0';
    }
    fclose(file);
    return fileBuffer;
  }
  else
  {
    fprintf(stderr, "Failed to open \"%s\" !\n", path);
    perror("Reason");
    return NULL;
  }
}

int main(int argc, char **argv)
{
  if (argc == 1)
  {
    fprintf(stderr, "Usage: RandomSteamGameLauncher \"YourDisk:/Path/To/Steam/steamapps/libraryfolders.vdf\"\n"
                    "You can blacklist a non-game (e.g., software, modding tool or invisible games) by getting its AppId"
                    "from your Steam client (Properties > Updates) and adding it to steam_app_blacklist.txt, comma-separated.");
    return 1;
  }

  srand((unsigned int)time(NULL));

  char appIdBuffer[APP_ID_BUFFER_SIZE];

  char *steamLibrary = ReadEntireFile(argv[1], "r");
  if (!steamLibrary) return 1;

  char *blacklist = ReadEntireFile("steam_app_blacklist.txt", "r");
  if (!blacklist) return 1;

  int blacklistedAppsCount = 0;
  int blacklistedAppsIds[MAX_GAMES] = {0};
  if (blacklist)
  {
    char *token = strtok(blacklist, ",");
    while (token != NULL)
    {
      strncpy(appIdBuffer, token, APP_ID_BUFFER_SIZE - 1);
      appIdBuffer[APP_ID_BUFFER_SIZE - 1] = '\0';
      blacklistedAppsIds[blacklistedAppsCount] = atoi(appIdBuffer);
      blacklistedAppsCount++;
      token = strtok(NULL, ",");
    }
  }

  int appsCount = 0;
  int installedAppsIds[MAX_GAMES] = {0};
  char *apps = strstr(steamLibrary, "\"apps\"");
  if (apps)
  {
    apps += 6;
    for (;;)
    {
      int doubleQuoteSkipCount = (appsCount > 0) ? 4 : 1;
      while (doubleQuoteSkipCount--)
      {
        while (*apps != '"')
        {
          apps++;
          if (*apps == '}') goto pick_random_game;
        }
        apps++;
      }

      char *nextDoubleQuote = apps;
      while (*nextDoubleQuote != '"') nextDoubleQuote++;
      strncpy(appIdBuffer, apps, (int)(nextDoubleQuote - apps));
      appIdBuffer[(int)(nextDoubleQuote - apps)] = '\0';

      int appId = atoi(appIdBuffer);
      int blacklisted = 0;
      for (int i = 0; i < blacklistedAppsCount; ++i)
      {
        if (appId == blacklistedAppsIds[i])
        {
          blacklisted = 1;
          break;
        }
      }

      if (!blacklisted)
      {
        installedAppsIds[appsCount] = appId;
        appsCount++;
      }
    }
  }

pick_random_game:
  if (appsCount > 0)
  {
    int randomGame = installedAppsIds[rand() % appsCount];
    printf("Launching %d... (If it didn't launch, consider adding this id to the blacklist)\n", randomGame);

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
  }
  else
  {
    printf("No games found to launch.\n");
  }

  return 0;
}
