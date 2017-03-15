 #Huffman encode-decode program for files
 ##Compilation:
    Works with GCC with "-std=c99" flag.
 ##After compilation write:
    "Calling executing file" -en/-de(1) -b(2) -info(3) "full path to encode/decode file"
 ##Examples:
    ./main.x -en /home/user1/in.txt
    ./main.x -en -info /home/user1/in.txt
    ./main.x -en -b -info /home/user1/in.x
    ./main.x -de -b /home/user1/out.huf
 1. For encode or decode file. Decode file must have file extension **.huf** at end.
 2. Optional for encoding. Write "-b" if file is a binary. Files reads as a text by default.
 3. Optional. For seeing statistic on file, visualize huffman tree and more.