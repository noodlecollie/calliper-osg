Calliper OSGgit remote add origin https://github.com/x6herbius/calliper-osg.git
git push -u origin master
============

Superseded version of https://github.com/x6herbius/calliper using OpenSceneGraph for 3D rendering.

Prerequisites
=============

Building Calliper depends on the following:

* Qt 5 (the latest tested version is 5.11).
* Cmake (for generating builds for dependencies, eg. OpenSceneGraph)
* Make or Visual Studio, depending on your platform.
* Python 3, if you wish to use the build helper scripts (recommended).

Other depdendencies are included as Git submodules. Run `git submodule init` and `git submodule update` after cloning to initialise them.

Compiling
=========

Certain submodules used by Calliper require Cmake in order to be built. Calliper itself, however, aims to be contained entirely within a Qt .pro project to allow Qt as much native management of the project as possible.

Before compiling Calliper, the submodules will first need to be built. The Calliper project is set up to look for the relevant external libraries and include files within the `submodule-libs` directory, and running the `configure-dependencies.py` script in the repo root will create the required CMake builds within `submodule-libs`. Each of the subdirectories will need to be built manually - depending on your platform, this should be as simple as executing `make` in the subdirectory, or by opening and building the Visual Studio project if you're on Windows. This could be automated by the build helper script in the future, but given how long OpenSceneGraph takes to compile if you accidentally clean it, I decided against it for now.

Once all the dependencies are built, open `calliper/calliper.pro` in Qt Creator and hit "Build". I would expect that the project should also compile via the command line/QMake, but I haven't currently tested this.

Terms of Use of My Stuff
========================

Third party software and libraries used by this project must have their respective licenses adhered to.

Apart from that: go nuts. Feel free to use all, some or none of the files otherwise provided here for whatever purpose you see fit (distribution, modification, compilation...), with or without credit, as long as you're not selling the work they were used for. Given those conditions, that any resulting derivative works retain them is preferred, but not required.

TL;DR: Use, fix, learn and let others learn.

These rules (or lack of) apply to all my work that's in the public domain, unless specified otherwise.
