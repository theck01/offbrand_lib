/**
 * @file OBString.c
 * @brief OBString Method Implementation
 * @author theck
 */

#include "../../include/OBString.h"
#include "../../include/private/OBString_Private.h"
#include <regex.h>

/** Buffer size for a string to print on a regex error */
#define REGEX_ERROR_BUFFER_SIZE 256

/* PUBLIC METHODS */

OBString *createString(const char *str){

  OBString *instance;
  
  assert(str);

  instance = createDefaultString();

  instance->length = strlen(str);
  instance->str = realloc(instance->str, (instance->length+1)*sizeof(char));
  assert(instance->str);

  strcpy(instance->str, str);

  return instance;
}


OBString * copySubstring(const OBString *s, int64_t start, uint32_t length){
  
  OBString *instance;

  assert(s);

  instance = createDefaultString();

  /* account for negative indexing */
  if(start < 0) start += s->length;

  /* if negative indexing beyond start of string then modify range */
  if(start < 0){
    length += start; /* shrink length to not copy out of range space */
    start = 0;
  }
  
  /* if indexing beyond end of string then shrink lenght to not copy out of 
   * range space */
  if(start+length > s->length) length = s->length - start;

  /* if specified range contains no characters return an "empty" string */
  if(start > s->length || start+length < 0 || s->length == 0 || length <= 0)
    return instance;

  instance->length = length;
  instance->str = realloc(instance->str, (length+1)*sizeof(char));
  assert(instance->str);

  strncpy(instance->str, s->str+start, length);
  instance->str[length] = '\0';

  return instance;
}


uint32_t stringLength(const OBString *s){
  assert(s);  
  return s->length;
}


char charAtStringIndex(const OBString *s, int64_t i){

  assert(s);

  if(i < 0) i += s->length; /* account for negative indexing */
  if(i >= s->length || i < 0) return '\0';
  return s->str[i];
}


OBString * concatenateStrings(const OBString *s1, const OBString *s2){

  OBString *concatted;

  assert(s1);
  assert(s2);

  concatted = createDefaultString();
  concatted->length = s1->length + s2->length;

  if(concatted->length == 0) return concatted;

  concatted->str = realloc(concatted->str, (concatted->length+1)*sizeof(char));
  assert(concatted->str);

  if(s1->length > 0) strcpy(concatted->str, s1->str);
  if(s2->length > 0) strcpy(concatted->str+s1->length, s2->str);

  return concatted;
}


const char * getCString(const OBString *s){
  assert(s);
  return s->str;
}


OBVector * splitString(const OBString *s, const char *delim){

  OBVector *tokens;
  OBString *copy, *substring;
  char *marker;
  uint32_t i, delim_len, substrs;


  assert(s);
  assert(delim);

  tokens = createVector(1);
  copy = copySubstring(s, 0, s->length);
  delim_len = strlen(delim);
  marker = copy->str;

  /* overwrite all instance of delimeter with NUL character(s) */
  while(marker <= copy->str+(copy->length - delim_len)){
    if(strncmp(marker, delim, delim_len) == 0)
      for(i=0; i<delim_len; i++) *(marker++) = '\0';
    else
      marker++;
  }

  marker = copy->str;

  /* copy all found substrings into new OBStrings for Vector */
  substrs=0;
  while(marker < copy->str+copy->length){
    substring = createString(marker);
    marker += substring->length;
    storeAtVectorIndex(tokens, (OBObjType *)substring, substrs++);
    release((OBObjType *)substring); /* only tokens vector needs a reference */
    while(*marker == '\0' && marker < copy->str + copy->length) marker++;
  }

  release((OBObjType *)copy);
  return tokens;
}


uint8_t findSubstring(const OBString *s, const char *substring){

  uint32_t sublen;
  char *marker;

  assert(s);
  assert(substring);

  sublen = strlen(substring);
  
  for(marker = s->str; marker <= s->str +(s->length - sublen); marker++)
    if(strncmp(marker, substring, sublen) == 0) return 1;

  return 0;
}


OBString * matchStringRegex(const OBString *s, const char *regex){

  regex_t comp_regex;
  regmatch_t match;
  int errcode;
  char errbuf[REGEX_ERROR_BUFFER_SIZE];

  assert(s);
  assert(regex);

  /* compile regex */
  if((errcode = regcomp(&comp_regex, regex, REG_EXTENDED))){
    regerror(errcode, &comp_regex, errbuf, REGEX_ERROR_BUFFER_SIZE);
    fprintf(stderr, "OBString::matchStringRegex recieved improper regex\n"
                    "Regex supplied: %s\nError string:%s\n", regex, errbuf);

    regfree(&comp_regex);
    return createString("");
  }

  /* match regex, returning empty string if no match was found */
  if(regexec(&comp_regex, s->str, 1, &match, 0)){
    regfree(&comp_regex);
    return createString("");
  }

  regfree(&comp_regex);

  /* return a new string containing the matching range */
  return copySubstring(s, match.rm_so, match.rm_eo - match.rm_so);
}


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * OBString_Private.h as well if modifications are made */
OBString * createDefaultString(void){

  static const char classname[] = "OBString";
  OBString *new_instance = malloc(sizeof(OBString));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((OBObjType *)new_instance, &deallocString, &hashString, &compareStrings,
           &displayString, classname);

  new_instance->str = malloc(sizeof(char));
  assert(new_instance->str);
  new_instance->str[0] = '\0';
  new_instance->length = 0;

  return new_instance;
}


obhash_t hashString(const OBObjType *to_hash){
  
  static int8_t init = 0;
  static obhash_t seed;

  OBString *instance = (OBString *)to_hash;
  obhash_t value;
  char *pos;

  assert(to_hash);
  assert(objIsOfClass(to_hash, "OBString"));

  if(init == 0){
    srand(time(NULL));
    seed = rand();
    init = 1;
  }
  
  value = seed;

  /* A version of Jenkin's one at a time hash function */
  pos = instance->str;
  while(*pos != '\0'){
    value += *(pos++);
    value += value << 10;
    value ^= value >> 6;
  }

  value += value << 3;
  value ^= value >> 11;
  value += value << 15;
  return value;
}


int8_t compareStrings(const OBObjType *a, const OBObjType *b){
  
  uint32_t i;
  const OBString *comp_a = (OBString *)a;  
  const OBString *comp_b = (OBString *)b;  

  assert(a);
  assert(b);
  assert(objIsOfClass(a, "OBString"));
  assert(objIsOfClass(b, "OBString"));

  /* compare string contents where both have characters */
  for(i=0; i<comp_a->length && i<comp_b->length; i++){
    if(comp_a->str[i] < comp_b->str[i]) return OB_LESS_THAN;
    else if(comp_b->str[i] < comp_a->str[i]) return OB_GREATER_THAN;
  }

  /* if characters matched check lengths for final equality */
  if(comp_a->length < comp_b->length) return OB_LESS_THAN;
  else if(comp_b->length < comp_a->length) return OB_GREATER_THAN;
  return OB_EQUAL_TO;
}


void displayString(const OBObjType *str){
  assert(str != NULL);
  assert(objIsOfClass(str, "OBString"));
  fprintf(stderr, "OBString with Contents:\n  %s\n", ((OBString *)str)->str);
}


void deallocString(OBObjType *to_dealloc){

  /* cast generic obj to OBString */
  OBString *instance = (OBString *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBString"));

  if(instance->str) free(instance->str);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
