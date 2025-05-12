## GF2ASI
This repository contains the source code for GF2ASI, an asi hook for "The Godfather II", using the steam executable.
Please note that we are working on making this more friendly with open source. Some code may be hardcoded to work locally prior to being open source.
This has been in development on and off for the last few years, it has some pretty solid foundations. There are three main reasons as to why this has become open source:
- To provide others to contribute to the asi hook.
- To try and help other projects using the EARS (EA Redwood Shores) engine as the framework is similar (between 2005 and 2013)
- Prove that this mod is in fact **not** a virus of Nexus Mods decides to block the ability to download.

Currently I'm working towards reverse engineering the State Machines, which is on a seperate branch.

## How to build
1. Edit premake5.lua so 'targetdir' and 'debugcommand' is updated to match your gf2 directory (never submit these changes though!)
2. Run 'build.cmd' to generate the Visual Studio solution and project.
3. Try and compile
4. Ensure that the GF2ASI.asi file is stored in the 'scripts' folder of your gf2 install
5. Try to run the game using the gf2 executable. Running the game via Visual Studio will cause a perpetual black screen.

## Contributing
All are welcome to make changes to the asi mod, feel free to open a PR and I will review.
