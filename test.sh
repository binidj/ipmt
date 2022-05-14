#!/bin/bash

echo "# INIT TEST SCRIPT"

for TRIAL in 1 2 3
do
    echo FROM $PWD
    cd test_data/compression/zip/
    echo TO $PWD
    
    for FILE in ./*; 
    do
        rm $FILE
    done

    echo FROM $PWD
    cd ../../../
    echo TO $PWD

    echo FROM $PWD
    cd test_data/compression/unzip/gzip/
    echo TO $PWD
    
    for FILE in ./*; 
    do
        rm $FILE
    done

    echo FROM $PWD
    cd ../../../../
    echo TO $PWD

    echo FROM $PWD
    cd test_data/compression/unzip/ipmt/
    echo TO $PWD
    
    for FILE in ./*; 
    do
        rm $FILE
    done

    echo FROM $PWD
    cd ../../../../
    echo TO $PWD
    
    echo FROM $PWD
    cd test_data/indexing/
    echo TO $PWD
    
    for FILE in ./*; 
    do
        rm $FILE
    done

    echo FROM $PWD
    cd ../../
    echo TO $PWD

    SET=0    
    SET=$(expr $SET + 1)
    echo "# Zip - Trial $TRIAL"
    
    for ALGORITHM in gzip ipmt
    do
        WORD=0
        while read FILE_PATH 
        do
            WORD=$(expr $WORD + 1)
            echo Doing zip $ALGORITHM with file $FILE_PATH
            if [ "$ALGORITHM" == "ipmt" ]; 
            then
                (time bin/ipmt zip test_data/$FILE_PATH) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
                mv test_data/$FILE_PATH.myz test_data/compression/zip/
            else
                (time gzip < test_data/$FILE_PATH > test_data/$FILE_PATH.gz) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
                mv test_data/$FILE_PATH.gz test_data/compression/zip/
            fi
        done < test_data/compression/paths.txt
    done

    SET=$(expr $SET + 1)
    echo "# Unzip - Trial $TRIAL"

    for ALGORITHM in gzip ipmt
    do
        WORD=0
        while read FILE_PATH 
        do
            WORD=$(expr $WORD + 1)
            echo Doing unzip $ALGORITHM with file $FILE_PATH
            if [ "$ALGORITHM" == "ipmt" ]; 
            then
                (time bin/ipmt unzip test_data/compression/zip/$FILE_PATH.myz) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
                mv test_data/compression/zip/$FILE_PATH test_data/compression/unzip/$ALGORITHM/$FILE_PATH
            else
                (time gzip -d -k test_data/compression/zip/$FILE_PATH.gz) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
                mv test_data/compression/zip/$FILE_PATH test_data/compression/unzip/$ALGORITHM/$FILE_PATH
            fi
        done < test_data/compression/paths.txt
    done

    SET=$(expr $SET + 1)
    echo "# Indexing - Trial $TRIAL"

    for ALGORITHM in ipmt
    do
        WORD=0
        while read FILE_PATH 
        do
            WORD=$(expr $WORD + 1)
            echo Doing indexing $ALGORITHM with file $FILE_PATH
            if [ "$ALGORITHM" == "ipmt" ]; 
            then
                (time bin/ipmt index test_data/$FILE_PATH) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
                mv test_data/$FILE_PATH.idx test_data/indexing/
            else
                echo NOT_USED
                # (time gzip < test_data/$FILE_PATH > test_data/$FILE_PATH.gz) 2>&1 | tee "test_data/compression/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
                # mv test_data/$FILE_PATH.gz test_data/compression/zip/
            fi
        done < test_data/index_files.txt
    done

    SET=$(expr $SET + 1)
    echo "# Search1 - Trial $TRIAL"

    for ALGORITHM in pmt ipmt grep
    do
        WORD=0
        while read PATTERN 
        do
            WORD=$(expr $WORD + 1)
            echo Searching pattern $PATTERN with tool $ALGORITHM 
            if [ "$ALGORITHM" == "ipmt" ]; 
            then
                (time bin/ipmt -c search $PATTERN test_data/indexing/index_english.txt.idx) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
            elif [ "$ALGORITHM" == "pmt" ]
            then
                (time bin/pmt -c $PATTERN test_data/index_english.txt) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
            else
                (time grep -c $PATTERN test_data/index_english.txt) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
            fi
        done < test_data/patterns/indexing/set_4.txt
    done

    SET=$(expr $SET + 1)
    echo "# Search2 - Trial $TRIAL"

    for ALGORITHM in pmt ipmt grep
    do
        WORD=0
        while read PATTERN 
        do
            WORD=$(expr $WORD + 1)
            echo Searching pattern $PATTERN with tool $ALGORITHM 
            if [ "$ALGORITHM" == "ipmt" ]; 
            then
                (time bin/ipmt -c search $PATTERN test_data/indexing/index_dna.txt.idx) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
            elif [ "$ALGORITHM" == "pmt" ];
            then
                (time bin/pmt -c $PATTERN test_data/index_dna.txt) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
            else
                (time grep -c $PATTERN test_data/index_dna.txt) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
            fi
        done < test_data/patterns/indexing/set_5.txt
    done
    
    SET=$(expr $SET + 1)
    echo "# Search File1 - Trial $TRIAL"
    
    for ALGORITHM in pmt ipmt grep
    do
        if [ "$ALGORITHM" == "ipmt" ]; 
        then
            (time bin/ipmt -p test_data/patterns/indexing/set_4.txt -c search test_data/indexing/index_english.txt.idx) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
        elif [ "$ALGORITHM" == "pmt" ];
        then
            (time bin/pmt -p test_data/patterns/indexing/set_4.txt -c test_data/index_english.txt) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
        else
            (time grep -f test_data/patterns/indexing/set_4.txt -c test_data/index_english.txt) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
        fi
    done
    
    SET=$(expr $SET + 1)
    echo "# Search File2 - Trial $TRIAL"
    
    for ALGORITHM in pmt ipmt grep
    do
        if [ "$ALGORITHM" == "ipmt" ]; 
        then
            (time bin/ipmt -p test_data/patterns/indexing/set_5.txt -c search test_data/indexing/index_dna.txt.idx) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
        elif [ "$ALGORITHM" == "pmt" ];
        then
            (time bin/pmt -p test_data/patterns/indexing/set_5.txt -c test_data/index_dna.txt) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
        else
            (time grep -f test_data/patterns/indexing/set_5.txt -c test_data/index_dna.txt) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
        fi
    done

    SET=$(expr $SET + 1)
    echo "# Search File3 - Trial $TRIAL"
    
    for ALGORITHM in pmt ipmt grep
    do
        if [ "$ALGORITHM" == "ipmt" ]; 
        then
            (time bin/ipmt -p test_data/patterns.txt -c search test_data/indexing/index_english.txt.idx) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
        elif [ "$ALGORITHM" == "pmt" ];
        then
            (time bin/pmt -p test_data/patterns.txt -c test_data/index_english.txt) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
        else
            (time grep -f test_data/patterns.txt -c test_data/index_english.txt) 2>&1 | tee "test_data/repports/$TRIAL""_"$SET"_"$WORD"_"$ALGORITHM".txt"
        fi
    done
done