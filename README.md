# string_builder

string_builder for C

## functions

+ `string_builder *sb_new(const char *str)`  
  Create a string_builder and initialize

+ `void sb_free(string_builder *psb)`  
  Clear and free string_builder

+ `int sb_init(string_builder *psb)`  
  Initialize string_builder

+ `string_builder *sb_clear(string_builder *psb)`  
  Clear string_builder

+ `string_builder *sb_append(string_builder *psb, const char *str)`  
  Append string to string_builder

+ `string_builder *sb_appendc(string_builder *psb, char c)`  
  Append char to string_builder

+ `string_builder *sb_appendi(string_builder *psb, int val)`  
  Convert an int value to string and append it to string_builder

+ `string_builder *sb_appendf(string_builder *psb, double val)`  
  Convert a double value to string and append it to string_builder

+ `string_builder *sb_insert(string_builder *psb, size_t index, const char *str)`  
  Insert a string into the specified index

+ `string_builder *sb_insertc(string_builder *psb, size_t index, char c)`  
  Insert a char into the specified index

+ `string_builder *sb_inserti(string_builder *psb, size_t index, int val)`  
  Convert an int value to string insert it into the specified index

+ `string_builder *sb_insertf(string_builder *psb, size_t index, double val)`  
  Convert a double value to string insert it into the specified index

+ `char sb_charat(string_builder *psb, size_t index)`  
  Get the char at the specified index

+ `int sb_setcharat(string_builder *psb, size_t index, char c)`  
  Set the char at the specified index

+ `size_t sb_length(string_builder *psb)`  
  Get the length of string_builder

+ `void sb_writeto(string_builder *psb, char *dest)`  
  Output the value of string_builder to a string

+ `char *sb_tostring(string_builder *psb)`  
  Convert string_builder to string

For the function whose return value is string_builder pointer, it returns NULL if an error occurs, otherwise it returns the input pointer.
