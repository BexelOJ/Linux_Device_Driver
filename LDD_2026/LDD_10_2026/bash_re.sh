#!/bin/bash

for dir in */; do

    old_dir="${dir%/}"

    if [[ "$old_dir" == *202061027* ]]; then

        new_dir="${old_dir//202061027/20261027}"

        echo "-------------------------------------------"
        echo "Directory:"
        echo "  $old_dir"
        echo "  -> $new_dir"

        mv "$old_dir" "$new_dir"

        # Rename C files containing the wrong date
        for cfile in "$new_dir"/*.c; do

            [ -e "$cfile" ] || continue

            filename=$(basename "$cfile")

            if [[ "$filename" == *202061027* ]]; then

                new_filename="${filename//202061027/20261027}"

                mv "$cfile" "$new_dir/$new_filename"

                echo "C file:"
                echo "  $filename"
                echo "  -> $new_filename"
            fi

        done

        # Update Makefile
        if [ -f "$new_dir/Makefile" ]; then

            sed -i 's/202061019/20261027/g' "$new_dir/Makefile"

            echo "Makefile updated"

        fi

        echo

    fi

done
