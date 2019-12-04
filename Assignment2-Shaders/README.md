## README.md

### Simple Ray Tracer 

Author: Andrew R. Owens
Date: Oct 29, 2019
Copyright (2019)

## Description

The project now includes its own version of glad AND glfw that need to be
compiled with project. This should happen automatically with any of the options
below for installation.

## Configure

### Unix makefile (Tested)
	cmake -H. -Bbuild

### Mac OSX (Tested)
	cmake -H. -Bbuild -G "Xcode"

### Microsoft Windows (Tested)
	cmake -H. -Bbuild -G "Visual Studio 15"

or

	cmake -H. -Bbuild -G "Visual Studio 15 Win64"

## USAGE

	build/SimpleRayTrace
