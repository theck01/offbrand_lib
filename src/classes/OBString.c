/**
 * @file OBString.c
 * @brief OBString Method Implementation
 * @author theck
 */

#include "../../include/OBString.h"
#include "../../include/private/OBString_Private.h"

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
  uint32_t i, delim_len;


  assert(s);
  assert(delim);

  tokens = createVector(1);
  copy = copySubstring(s, 0, s->length);
  delim_len = strlen(delim);
  marker = copy->str;

  /* overwrite all instance of delimeter with NUL character(s) */
  while(marker < copy->str+copy->length - delim_len){
    if(strncmp(marker, delim, delim_len) == 0)
      for(i=0; i<delim_len; i++) *(marker++) = '\0';
    else
      marker++;
  }

  marker = copy->str;

  /* copy all found substrings into new OBStrings for Vector */
  while(marker < copy->str+copy->length){
    substring = createString(marker);
    marker += substring->length;
    addToVector(tokens, (obj *)substring);
    release((obj *)substring); /* only tokens vector needs a reference */
    while(*marker == '\0' && marker < copy->str + copy->length) marker++;
  }

  release((obj *)copy);
  return tokens;
}


/* DEFINE ADDITIONAL PUBLIC METHODS HERE */


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify 
 * OBString_Private.h as well if modifications are made */
OBString * createDefaultString(void){

  static const char classname[] = "OBString";
  OBString *new_instance = malloc(sizeof(OBString));
  assert(new_instance != NULL);

  /* initialize base class data */
  initBase((obj *)new_instance, &deallocString, &hashString, &compareStrings,
           classname);

  new_instance->str = malloc(sizeof(char));
  assert(new_instance->str);
  new_instance->str[0] = '\0';
  new_instance->length = 0;

  return new_instance;
}


obhash_t hashString(const obj *to_hash){
  return hash(to_hash);
}


int8_t compareStrings(const obj *a, const obj *b){
  
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


void deallocString(obj *to_dealloc){

  /* cast generic obj to OBString */
  OBString *instance = (OBString *)to_dealloc;

  assert(to_dealloc);
  assert(objIsOfClass(to_dealloc, "OBString"));

  if(instance->str) free(instance->str);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
