## README.md

### Simple Curves

Author: Philmo Gu, Andrew R. Owens
Date: November 19, 2019
Copyright (2019)

## Description

A simple example of curves using OpenGL.

The project now includes its own version of glad AND glfw that need to be
compiled with project. This should happen automatically with any of the options
below for installation.

## Configure

### Unix makefile (Tested)
	cmake -H. -Bbuild

### Mac OSX (Tested)
	cmake -H. -Bbuild -G "Xcode"

### Microsoft Windows (Tested - if you have Visual Studio 2017)
	(Change to "Visual Studio 16" if you have Visual Studio 2019)
	cmake -H. -Bbuild -G "Visual Studio 15"

or

	cmake -H. -Bbuild -G "Visual Studio 15 Win64"

## USAGE

	build/Curves
