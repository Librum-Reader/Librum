#!/bin/sh
# Build-time, packaging-only QML fix — runs against the STAGED source inside the build
# container, never the committed repo. Invoked from specfile.spec.liquid (%prep) and
# deb/rules.liquid (before configure).
#
# Why: several QML files use IconImage, a type from the *private* QtQuick.Controls.impl
# module, while importing only QtQuick.Controls. Qt < 6.11 leaked that type into the public
# namespace so it resolved; Qt 6.11+ does not -> runtime "IconImage is not a type" and a
# broken UI (first seen on openSUSE Tumbleweed). Injecting the missing import is safe on all
# Qt 6 (the impl module has existed since Qt 6.0; verified working on Qt 6.8 too).
#
# Idempotent: only patches files that use IconImage and lack the impl import.
set -eu

for f in $(grep -rlE '\bIconImage\b' src --include='*.qml' 2>/dev/null || true); do
  if ! grep -q 'QtQuick\.Controls\.impl' "$f"; then
    sed -i '/^import QtQuick\.Controls$/a import QtQuick.Controls.impl' "$f"
    echo "patch-qml: added QtQuick.Controls.impl import to $f"
  fi
done
