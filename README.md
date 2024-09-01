## Prerequisites
1. Windows OS (this is going to change in the future to include linux)
2. Visual Studio (this will also change)
3. A C++20 Compiler
3. Python installed and in your system enviroment variable "Path" (there is an option to add this in the python installer)

## Building
1. First step is to clone the repository recursively using the command below.
```
git clone https://github.com/DevAlgos/TooGoodEngine-NEA.git --recursive 
```
2. After this navigate to `Scripts/` and run the script to generate project files.
3. A visual studio solution is generated in the root of the project you can simply double click to open and build.

There are a few configurations to be known about. The first few is your standard Debug and Release which is self explanatory,
and the third is Dist which is the build configuration used to build a project and run it in the runtime. It will strip a lot of
debug code such as timings (debating whether this would be useful in Dist), debug logging and a few others. If you
do use the Dist configuration whilst in the editor some stuff may not appear such as timings, logging etc... this is normal however
I would recommend staying in Release when using the editor.

## Cheat Sheet
TODO (will contain all functions available in scripting library and how to navigate and use all the gui elements).
Currently there is an example project located in the `Example/` directory, this wil also grow as I add new features.

## Attributions
1. <a href="https://www.flaticon.com/free-icons/obj" title="obj icons">Obj icons created by Freepik - Flaticon</a>
2. <a target="_blank" href="https://icons8.com/icon/53386/image">Image</a> icon by <a target="_blank" href="https://icons8.com">Icons8</a>
3. <a href="https://www.freepik.com/icon/fbx_8156690#fromView=keyword&page=1&position=21&uuid=02f13dd0-9263-4944-b050-a149ecf1d32e">Icon by Creatype</a>
4. <a href="https://www.flaticon.com/free-icons/left-arrow" title="left arrow icons">Left arrow icons created by Freepik - Flaticon</a>
5. <a href="https://www.freepik.com/icon/hdr_17563430#fromView=keyword&page=1&position=10&uuid=3988a741-ff4e-4be9-9e27-40cd65a39575">Icon by Wyasa.Design</a>
6. <a target="_blank" href="https://icons8.com/icon/l75OEUJkPAk4/python">Python</a> icon by <a target="_blank" href="https://icons8.com">Icons8</a>