# bin2h
Utility for converting binary files to C / C++ headers.

## License
bin2h is released to the Public Domain.

---

GOT FROM:
https://github.com/elnormous/bin2h

Modified to C++ and skewed a little to my style.



  -d        Print decimal instead of hex literals;
  
  -e        Output a variable that holds a pointer to the last element of the data;
  
  -i <in>   Specify the input file;
  
  -n <name> Specify the variable name;
  
  -o <out>  Specify the output file;
  
  -s        Output a variable for data size;
  
  -z        Add a zero to the end of the array;
  
  -c        Collums for a new line (0x1 to 0xF). '0': No line change.;
