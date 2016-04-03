# -*- coding: utf-8 -*-

import os
import glob
import shutil

# Setup app info
appName  = "TitanIM"
certificate  = "Ruslan Nazarov" #3rd Party Mac Developer Application: Ruslan Nazarov
entitlements = "Entitlements.plist"
entitlementsWebEngine = "EntitlementsWeb.plist"

fullApp  = appName + ".app"

frameworksDir = fullApp + "/Contents/Frameworks/"
pluginsDir    = fullApp + "/Contents/PlugIns/"

print("Prepearing...")
# Check files and paths 
if not os.path.exists(entitlementsWebEngine) or os.path.isdir(entitlementsWebEngine):
	print("EntitlementsWeb file not found")
	exit()
if not os.path.exists(entitlements) or os.path.isdir(entitlements):
	print("Entitlements file not found")
	exit()
if not os.path.exists(fullApp) or not os.path.isdir(fullApp):
	print("App bundle not found")
	exit()

print("\nSigning frameworks, dylibs, and binary...")
# Sign frameworks (it's strange? but we can't sign "Versions" folder)
os.system('codesign -s "' + certificate + '" -i "org.qt-project.QtWebEngineProcess" --entitlements ' + entitlementsWebEngine + ' ' + frameworksDir + 'QtWebEngineCore.framework/Helpers/QtWebEngineProcess.app')
os.system('codesign -s "' + certificate + '" ' + frameworksDir + "*")

# Sign plugins
pluginGroups = os.listdir(pluginsDir)
for group in pluginGroups:
	os.system('codesign -s "' + certificate + '" ' + pluginsDir + group + "/*")

# Sign app
os.system('codesign --entitlements ' + entitlements + ' -s "' + certificate + '" ' + fullApp)

print("\nCheck signing:")
os.system("codesign --display --verbose=4 " + fullApp)