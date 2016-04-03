TMP_DIR="./tmp"
APP_BUNDLE_NAME="TitanIM.app"
APP_VERSION="2.0.2"
DMG_NAME_BASE=${APP_BUNDLE_NAME%.*}
DMG_NAME_SUFFIX="-${APP_VERSION}"
DMG_NAME="${DMG_NAME_BASE}${DMG_NAME_SUFFIX}.dmg"
DMG_NAME_TMP="${APP_BUNDLE_NAME%.*}_tmp.dmg"
VOL_NAME=${APP_BUNDLE_NAME%.*}

mkdir "$TMP_DIR"
cp -R "${APP_BUNDLE_NAME}" ${TMP_DIR}/
rm -f "${DMG_NAME_TMP}"

hdiutil create -ov -srcfolder ${TMP_DIR} -format UDRW -volname "${VOL_NAME}" "${DMG_NAME_TMP}"
device=$(hdiutil attach -readwrite -noverify -noautoopen ${DMG_NAME_TMP} | egrep '^/dev/' | sed 1q | awk '{print $1}')
sleep 5

#BG_FOLDER="/Volumes/${VOL_NAME}/.background"
#mkdir "${BG_FOLDER}"
#cp "${APP_BUNDLE_NAME}/Contents/Resources/${BG_IMG_NAME}" "${BG_FOLDER}/"
ICON_FOLDER="/Volumes/${VOL_NAME}"
cp "${APP_BUNDLE_NAME}/Contents/Resources/icon.icns" "${ICON_FOLDER}/.VolumeIcon.icns"

APPLESCRIPT="
tell application \"Finder\"
	tell disk \"${VOL_NAME}\"
		open
		-- Setting view options
		set current view of container window to icon view
		set toolbar visible of container window to false
		set statusbar visible of container window to false
		set the bounds of container window to {400, 100, 700, 300}
		set theViewOptions to the icon view options of container window
		set arrangement of theViewOptions to not arranged
		set icon size of theViewOptions to 72
		-- Adding symlink to /Applications
		make new alias file at container window to POSIX file \"/Applications\" with properties {name:\"Applications\"}
		-- Reopening
		close
		open
		-- Rearranging
		set the position of item \"Applications\" to {50, 20}
		set the position of item \"${APP_BUNDLE_NAME}\" to {265, 20}
		-- Updating and sleeping for 5 secs
		update without registering applications
		delay 5
	end tell
end tell
"

echo "$APPLESCRIPT" | osascript

#SetFile -c icnC "${ICON_FOLDER}/.VolumeIcon.icns"
#SetFile -a C "${ICON_FOLDER}"

chmod -Rf go-w /Volumes/"${VOL_NAME}"
sync
sync
hdiutil detach -force ${device}
rm -f ${DMG_NAME}

hdiutil convert "${DMG_NAME_TMP}" -format UDZO -imagekey zlib-level=9 -o "${DMG_NAME}"
rm -f "${DMG_NAME_TMP}"
rm -rf $TMP_DIR