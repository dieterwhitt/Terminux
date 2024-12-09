#!/usr/bin/env bash

# animatable executor
# .anim metadata format: 5 lines:

# below: all integers
# (framerate: 1 <= fps <= 360)
# (number of frames)
# (x resolution)
# (y resoulution)
# (looping behavior: 1 - loop, 0 - don't)

# followed by all frames concatenated together


# reads a line into the variable with the name of the first command line arg.
# if it doesnt exist, create it
readline() {
    local var_name="$1"  # The name of the variable to store the input
    IFS= read -r "$var_name"  # Read the input into the variable
}

# reads the first int on a line, ignores the rest of the line
# unsigned int only
readint() {
    local var_name="$1"
    IFS= read -r line
    # Use eval to assign the first integer from the line to the variable whose 
    # name is stored in var_name
    eval "$var_name=\$(echo \"$line\" | grep -o -m 1 '[0-9]\+')"
    if [[ -z $(eval "echo \$$var_name") ]]; then
        # number not found
        echo "Error reading number: number not found on line" >&2
        exit 1
    fi
}

# throws error if given number is not strictly positive
validate_positivity() {
    local val="$1"
    if [[ ${val} -lt 1 ]]; then
        echo "Error reading metadata: number not strictly positive" >&2
        exit 1
    fi
}

# from github
show_cursor() { # also exits
    tput cnorm
    echo
    exit
}

hide_cursor() {
    tput civis
}

# get file
dir="$(pwd)/"
file="${dir}$1"
if [[ ! -f ${file} ]]; then
    echo "File ${file} not found." >&2
    exit 1
fi

# hide cursor to begin operation
trap show_cursor SIGINT TERM EXIT
hide_cursor

# loop logic starts
while true; do
    # read metadata
    # open input file
    exec < ${file}

    readint fps
    if [[ ${fps} -lt 1 || ${fps} -gt 360 ]]; then
        echo "Error reading metadata: fps not within (1, 360)" >&2
        exit 1
    fi

    # rest of shannon data
    readint frames
    validate_positivity $frames

    readint x

    validate_positivity $x

    readint y
    validate_positivity $y

    readint loop
    if [[ ${loop} -ne 1 && ${loop} -ne 0 ]]; then
        echo "Error reading metadata: looping behavior not defined" >&2
        exit 1
    fi

    # metadata read: start print loop
    ofile=$(mktemp)
    line=""
    tick=$(echo "scale=8; 1/$fps" | bc)
    for ((i=0; i < frames; i++)) ; do
        # frame
        for ((j=0; j < y; j++)); do
            # line
            readline line
            echo "${line:0:x}" >> ${ofile}
        done
        clear
        cat ${ofile}
        > ${ofile} # reset temp file
        sleep ${tick}
    done

    rm ${ofile}

    # break depending on loop behavior
    if [[ ${loop} -eq 0 ]]; then
        break
    fi

done

sleep 0.25
show_cursor