# RandomSteamGameLauncher

## Usage

After compiling `src/main.c` with your preferred C compiler, locate the `libraryfolders.vdf` file in your Steam directory.

Next, navigate to the root directory of the executable, open your command line, and type:

```bash
RandomSteamGameLauncher "YourDisk:/Path/To/Steam/steamapps/libraryfolders.vdf"
```

To blacklist non-game items (e.g., software, modding tools, or invisible games), obtain their AppId from your Steam client (Properties > Updates) and add them to `steam_app_blacklist.txt`, separated by commas.

> [!WARNING]
This program has only been tested on Windows. Please let me know if it does not work on Linux or macOS!

> [!TIP]
On Windows, to avoid typing the command repeatedly, create a shortcut with `run.bat` as the target.
