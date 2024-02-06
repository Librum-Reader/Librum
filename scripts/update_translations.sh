#!/bin/bash

# Get the directory of the script
SCRIPT_DIR=$(dirname "$0")

# Find translation files using find command
TRANSLATION_FILES=$(find "$SCRIPT_DIR/../src/presentation/translations" -name "librum_*.ts")

# Use lupdate with the list of translation files
lupdate "$SCRIPT_DIR/../src/presentation" -ts $TRANSLATION_FILES
