#!/usr/local/bin/python

from ml import build
from ml.boilerplate import cpp
from ml import fileTools as ft
import os
import sys
sys.path.append("../")

fm = "/media/romain/Donnees/Programmation/cpp/frameworks"
libs = "/media/romain/Donnees/Programmation/cpp/libs"

for arg in sys.argv:
    if "libs=" in arg:
        libs = arg.split("=")[1]
    elif "fm=" in arg or "cpp-utils=" in arg:
        fm = arg.split("=")[1]

cpp.generate("../src")

includes = [
    "../src",
    libs + "/json",
    libs + "/eigen",
    #no need to include boost here, just install it systemwide with apt install libboost-all-dev
    fm,
        ]

srcs = [
        "../src",
        ]

libs = [
        ]

defs = ["NO_LOG"]

cpp = build.create("fxlauncher", sys.argv)
cpp.static = False
cpp.includes = includes
cpp.addToSrcs(srcs)
cpp.addToLibs(libs)
cpp.definitions += defs

cpp.addToLibs([
    "stdc++fs",
    "mlapi",
    ])

if("clean" in sys.argv or "clear" in sys.argv):
    cpp.clean()
    exit()
else:
    cpp.build()
