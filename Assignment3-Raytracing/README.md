## README.md

### Simple Lambertian Ray tracer with shadows and plane 

Author: Andrew R. Owens
Date: Nov 3, 2019
Copyright (2019)

## Description

Simple ray tracer, use what ever you want from this... cheers.

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

	build/RayTracing_Simple
