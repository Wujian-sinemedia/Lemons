#! /bin/sh

# from: https://gist.github.com/talaviram/1f21e141a137744c89e81b58f73e23c3

# Simple Utility Script for allowing debug of hardened macOS apps.
# This is useful mostly for plug-in developer that would like keep developing without turning SIP off.
# Credit for idea goes to (McMartin): https://forum.juce.com/t/apple-gatekeeper-notarised-distributables/29952/57?u=ttg

set -euo

app_path=$1

if [ -z "$app_path" ];
then
    echo "You need to specify the app to re-codesign!"
    exit 1
fi

# This uses local codesign. so it'll be valid ONLY on the machine you've re-signed with.
readonly entitlements_plist=/tmp/debug_entitlements.plist

echo "Grabbing entitlements from app..."

codesign -d --entitlements :- "$app_path" >> $entitlements_plist || { exit 1; }

echo "Patch entitlements (if missing)..."

/usr/libexec/PlistBuddy -c "Add :com.apple.security.cs.disable-library-validation bool true" $entitlements_plist
/usr/libexec/PlistBuddy -c "Add :com.apple.security.cs.allow-unsigned-executable-memory bool true" $entitlements_plist
/usr/libexec/PlistBuddy -c "Add :com.apple.security.get-task-allow bool true" $entitlements_plist

echo "Re-applying entitlements (if missing)..."
codesign --force --options runtime --sign - --entitlements $entitlements_plist "$app_path" || { echo "codesign failed!"; exit 1; }

echo "Removing temporary plist..."
rm $entitlements_plist

exit 0
