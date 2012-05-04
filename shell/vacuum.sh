#!/usr/bin/env sh

echo "WARN: I kill firefox if it is launched!" & pkill firefox
find ~/.mozilla/firefox -name "*.sqlite" -ls -exec sqlite3 {} "VACUUM" \;
echo "Firefox Vaccum SQLite finished.\n";

echo "WARN: I kill thunderbird if it is launched!" & pkill thunderbird
find ~/.thunderbird -name "*.sqlite" -ls -exec sqlite3 {} "VACUUM" \;
echo "Thunderbird Vaccum SQLite finished.";

