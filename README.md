#Huffman encode-decode program for files
##After compilation write:
    ./*name of executing file* -en/-de(1) -b(2) -info(3) *full path to file*
##Example:
    `./main.x -en -b -info /home/user1/huffman/in.txt`
 (1) For encode or decode file. Decode file must have ".huf" at end.
 (2) Optional. Write "-b" if file is a binary. Files reads as a text by default.
 (3) Optional. For seeing statistic on file, visualize huffman tree and more.