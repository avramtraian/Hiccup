# Hiccup

Welcome to Hiccup source code!

Hiccup is a rendering technology used to create photorealistic images and game environments. Our target is to create a unified architecture for creating CG content.

## Branches

The source code is organized into multiple branches:
*    At the moment, the *latest-stable* release can be found in the [***release***](https://github.com/avramtraian/Hiccup/tree/release) branch. This is the branch that *most* users should clone.
*    Once per year, we release the [***LTS***](https://github.com/avramtraian/Hiccup/tree/LTS/2022) (*long-term support*) version of the engine. These can be found in the ***LTS/{year}*** branches.
*    If you want to use more experimental code and help test the engine, [***master***](https://github.com/avramtraian/Hiccup/tree/master) is the branch where all the features merge first. This is not meant for use in any commercial project.
*    Branches whose names start with ***dev/***, ***test/*** or ***feature/*** are only for internal development purposes and are not useful for other users. Some commits might not even compile.

## Features

*    Comming soon!
*    ...

## Downloading, Compiling and Running
 
 These steps take you through the process of downloading, compiling and running the editor yourself.
 
### Windows
1.    Install a Git client, such as [***Fork***](https://git-fork.com/) or [***Github for Windows***](https://desktop.github.com/), then ***fork*** and ***clone*** this repository. Or, you can use the command-line instead, with the following command:
      
      ```
      git clone --recursive -b release https://github.com/avramtraian/Hiccup.git
      ```
      Either one you choose, make sure to download all the submodules.
      
2.    Install **Visual Studio 2022**. Hiccup *requires* Visual Studio. All desktop editions of Visual Studio 2022 can build Blueberry Engine, including [*Visual Studio Community 2022*](https://visualstudio.microsoft.com/vs/community/), which is free to use.
      In the *Visual Studio Installer*, you need to install the following components:
      * **Desktop development with C++**
      * **Game development with C++**
      * **Windows 10 SDK**
      
3.    Open the engine folder, and execute **Win64-GenProjectFiles.bat**. This will create the Visual Studio project files. It shouldn't take more than a few seconds.
4.    Open **Hiccup.sln**. Select the **Release** configuration, **Win64** platform. Right-click on the solution target and press **Build Solution** (*Ctrl+Shift+B*).
5.    After the compilation finishes, you can run the editor by pressing ***F5***.
### Mac
Currently, ***MacOS*** is not available as a build target.
### Linux
Currently, ***Linux*** is not available as a build target.