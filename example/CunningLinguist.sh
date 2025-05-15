#!/bin/bash

# Howdy. I'm feeling great .. so great that I will be continuing to write large amounts of
# comments that will not be useful to a novice level hacker at all. Binscript-packer will
# be used to pack this script into binscript resulting in an executable binary that won't
# reveal these worth while comments about how 1+1 does equal 2 but only after you press reset.

vocabulary=0
if [ -d /usr/share/dict ]; then
	vocabulary=$(cat /usr/share/dict/* | wc -l)
fi

echo There are $vocabulary words to learn.
