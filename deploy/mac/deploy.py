# -*- coding: utf-8 -*-

import os
import glob
import shutil
from subprocess import call

# Setup app info
appName  = "TitanIM"
pathToQt = "/Users/rnazarov/Qt/qt5.5.0/5.5/clang_64/" 
pathToQML = "/Users/rnazarov/Data/Projects/Qt/TitanIM/data/qml-desktop/qml"

fullApp  = appName + ".app"
dirName  = "Output"

frameworksDir = fullApp + "/Contents/Frameworks/"
pluginsDir    = fullApp + "/Contents/PlugIns/"

print("Prepearing to deploy...")
# Check files and paths 
if not os.path.exists(pathToQt) or not os.path.isdir(pathToQt):
	print("Incorrect path to Qt")
	exit()
if not os.path.exists(fullApp) or not os.path.isdir(fullApp):
	print("App bundle not found")
	exit()

# Create output dir
if not os.path.exists(dirName) and not os.path.isdir(dirName):
	os.makedirs(dirName)

# Remove old build
if os.path.exists(dirName + "/" + fullApp):
	shutil.rmtree(dirName + "/" + fullApp)

# Copy all necessary files to new folder
shutil.copytree(fullApp, dirName + "/" + fullApp)

# Copy Qt libs for create independent app
pwdir = os.getcwd()
os.chdir(pathToQt + "bin/")
print("\nDeploying Qt to .app bundle...")
call(["./macdeployqt", pwdir + "/" + dirName + "/" + fullApp, "-qmldir=" + pathToQML])
print("...done\n")
os.chdir(pwdir + "/" + dirName)

# Copy custom Frameworks
customFrameworks = os.listdir(pwdir + "/Frameworks/")
for customFramework in customFrameworks:
	shutil.copytree(pwdir + "/Frameworks/" + customFramework, frameworksDir + customFramework, True)

# Copy plists for frameworks (it's fix macdeployqt bug)
frameworks = os.listdir(frameworksDir)
for framework in frameworks:
	shutil.copy(pathToQt + "lib/" + framework + "/Resources/Info.plist", frameworksDir + framework + "/Resources/")