# -*- coding: utf-8 -*-

import os
import glob

# Setup app info
appName  = "TitanIM"
certificate  = "Ruslan Nazarov" #3rd Party Mac Developer Installer: Ruslan Nazarov

fullApp  = appName + ".app"

print("Prepearing to deploy...")
# Check files and paths 
if not os.path.exists(fullApp) or not os.path.isdir(fullApp):
	print("App bundle not found")
	exit()

print("\nBuilding package...")
os.system('productbuild --component "' + fullApp + '" /Applications --sign "' + certificate + '" --product "' + fullApp + '/Contents/Info.plist" ' + appName + '.pkg')
print("...done\n")

print('\nFor test install, run follow command: sudo installer -store -pkg ' + appName + '.pkg -target /')