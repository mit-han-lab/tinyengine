#!/bin/bash

# List of files to download, their corresponding MD5 checksums, and target local paths
files_and_checksums=(
  "https://www.dropbox.com/s/vcuzqyrewt1jjs3/models.zip 5fb5742f552b4c1eeab15fe83f22d651 models.zip"
  "https://www.dropbox.com/s/dstbc72fp7ka33d/assets.zip 87de2f473c22f0614ea4f658c5001baa assets.zip"
)

# Function to download a file if it doesn't exist or if its MD5 checksum is incorrect
download_if_needed() {
  url="$1"
  expected_md5="$2"
  target_path="$3"

  # Ensure the target directory exists
  target_dir=$(dirname "$target_path")
  mkdir -p "$target_dir"

  # Download the file if it does not exist
  if [ ! -e "$target_path" ]; then
    echo "File '$target_path' does not exist. Downloading..."
    wget -q -O "$target_path" "$url"
  fi

  # Check the MD5 checksum
  actual_md5=$(md5 -q "$target_path")

  if [ "$actual_md5" != "$expected_md5" ]; then
    echo "MD5 checksum for '$target_path' is incorrect. Downloading again..."
    wget -q -O "$target_path" "$url"
  else
    echo "File '$target_path' exists and its MD5 checksum is correct."
  fi
}

# Process each file, its corresponding MD5 checksum, and target local path
for file_and_checksum in "${files_and_checksums[@]}"; do
  url=$(echo "$file_and_checksum" | awk '{ print $1 }')
  expected_md5=$(echo "$file_and_checksum" | awk '{ print $2 }')
  target_path=$(echo "$file_and_checksum" | awk '{ print $3 }')

  download_if_needed "$url" "$expected_md5" "$target_path"
  unzip "$target_path"
done
