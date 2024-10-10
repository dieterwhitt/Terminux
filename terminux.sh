#!/usr/bin/env bash

dir="$(pwd)/"
argc=$#

msg="Hello from terminux.sh!"

# sample animation
fps=30
if [[ -n "$1"  &&  "$1" =~ ^[0-9]+$ ]]; then
    fps=$(($1))
fi

tick=$(echo "scale=8; 1/$fps" | bc)
for (( i=0; i < ${#msg}; i++ )); do
    printf "%c" "${msg:i:1}"
    sleep ${tick}
done

# sleep 5 frames
sleep $(echo "5 * ${tick}" | bc)

echo
