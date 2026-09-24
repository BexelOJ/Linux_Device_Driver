#! /bin/bash

for dir in */; do
    file="${dir%/}.c"

    if [ -f "$dir$file" ]; then
        cat > "$dir/Makefile" <<EOF
obj-m += ${dir%/}.o

all:
	make -C /lib/modules/\$(shell uname -r)/build M=\$(PWD) modules

clean:
	make -C /lib/modules/\$(shell uname -r)/build M=\$(PWD) clean
EOF
    fi
done