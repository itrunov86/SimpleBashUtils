#!/bin/bash
FUNC_SUCCESS=0
FUNC_FAIL=0
COUNTER=0
DIFF=""
FILEPATTERNS="patterns.txt"

declare -a flags=(
"-b"
"-e"
"-c"
"-E"
"-n"
"-s"
"-t"
"-T"
)

declare -a GNU=(
"--number-nonblank"
"--number"
"--squeeze-blank"
)

declare -a files=(
"s21_cat.c"
"s21_cat_test.txt"
"parcer.c"
"parcer.h"
)

declare -a commline=(
"FLAGS FILES"
)

function testing()
{
    str=$(echo "$@" | sed "s/FLAGS/$var1/")
    str=$(echo "$str" | sed "s/FILES/$var2/")
    ./s21_cat $str > s21_cat_testing.log
    cat $str > system_cat_testing.log
    DIFF="$(diff -s s21_cat_testing.log system_cat_testing.log)"
    (( COUNTER++ ))
    if [ "$DIFF" == "Files s21_cat_testing.log and system_cat_testing.log are identical" ]
    then
        (( FUNC_SUCCESS++ ))
        echo "cat $str FUNCTIONALITY SUCCESS $COUNTER"
    else
        (( FUNC_FAIL++ ))
        echo "cat $str FUNCTIONALITY FAIL $COUNTER"
    fi
    rm s21_cat_testing.log system_cat_testing.log
}


for i in "${GNU[@]}"
do
    for file1 in "${files[@]}"
    do
        for file2 in "${files[@]}"
        do
            if [ $file1 != $file2 ]
            then
                var1="$i"
                var2="$file1 $file2"
                testing "$commline"
            fi
        done
    done
done

for i in "${flags[@]}"
do
    for file1 in "${files[@]}"
    do
        for file2 in "${files[@]}"
        do
            if [ $file1 != $file2 ]
            then
                var1="$i"
                var2="$file1 $file2"
                testing "$commline"
            fi
        done
    done
done

for i in "${flags[@]}"
do
    for i2 in "${flags[@]}"
    do
        if [ $i != $i2 ]
        then
            for file1 in "${files[@]}"
            do
                for file2 in "${files[@]}"
                do
                    if [ $file1 != $file2 ]
                    then
                        var1="$i $i2"
                        var2="$file1 $file2"
                        testing "$commline"
                    fi
                done
            done
        fi
    done
done

for i in "${flags[@]}"
do
    for i2 in "${flags[@]}"
    do
        for i3 in "${flags[@]}"
        do
            if [ $i != i2 ] && [ $i2 != i3 ] && [ $i != $i3 ]
            then
                for file1 in "${files[@]}"
                do
                    for file2 in "${files[@]}"
                    do
                        if [ $file1 != $file2 ]
                        then
                            var1="$i $i2 $i3"
                            var2="$file1 $file2"
                            testing "$commline"
                        fi
                    done
                done
            fi
        done    
    done
done

echo "FAIL: $FUNC_FAIL"
echo "SUCCESS: $FUNC_SUCCESS"
echo "ALL: $COUNTER"
