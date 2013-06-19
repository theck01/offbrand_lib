/**
 * @file obstring.c
 * @brief obstring Method Implementation
 * @author theck
 */

#include "../../include/obstring.h"
#include "../../include/private/obstring_private.h"

/** Buffer size for a string to print on a regex error */
#define REGEX_ERROR_BUFFER_SIZE 256

/* PUBLIC METHODS */

obstring *obstring_new(const char *str){

  obstring *instance;

  assert(str);

  instance = obstring_create_default();

  instance->length = strlen(str);
  instance->str = realloc(instance->str, (instance->length+1)*sizeof(char));
  assert(instance->str);

  strcpy(instance->str, str);

  return instance;
}


obstring * obstring_copy_substring(const obstring *s, int64_t start, uint32_t length){

  obstring *instance;

  assert(s);

  instance = obstring_create_default();

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


uint32_t obstring_length(const obstring *s){
  assert(s);
  return s->length;
}


char obstring_char_at_index(const obstring *s, int64_t i){

  assert(s);

  if(i < 0) i += s->length; /* account for negative indexing */
  if(i >= s->length || i < 0) return '\0';
  return s->str[i];
}


obstring * obstring_concat(const obstring *s1, const obstring *s2){

  obstring *concatted;

  assert(s1);
  assert(s2);

  concatted = obstring_create_default();
  concatted->length = s1->length + s2->length;

  if(concatted->length == 0) return concatted;

  concatted->str = realloc(concatted->str, (concatted->length+1)*sizeof(char));
  assert(concatted->str);

  if(s1->length > 0) strcpy(concatted->str, s1->str);
  if(s2->length > 0) strcpy(concatted->str+s1->length, s2->str);

  return concatted;
}


const char * obstring_cstring(const obstring *s){
  assert(s);
  return s->str;
}


obvector * obstring_split(const obstring *s, const char *delim){

  obvector *tokens;
  obstring *copy, *substring;
  char *marker;
  uint32_t i, delim_len, substrs;


  assert(s);
  assert(delim);

  tokens = obvector_new(1);
  copy = obstring_copy_substring(s, 0, s->length);
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

  /* copy all found substrings into new obstrings for Vector */
  substrs=0;
  while(marker < copy->str+copy->length){
    substring = obstring_new(marker);
    marker += substring->length;
    obvector_store_at_index(tokens, (obj *)substring, substrs++);
    ob_release((obj *)substring); /* only tokens vector needs a reference */
    while(*marker == '\0' && marker < copy->str + copy->length) marker++;
  }

  ob_release((obj *)copy);
  return tokens;
}


uint8_t obstring_find_substring(const obstring *s, const char *substring){

  uint32_t sublen;
  char *marker;

  assert(s);
  assert(substring);

  sublen = strlen(substring);

  for(marker = s->str; marker <= s->str +(s->length - sublen); marker++)
    if(strncmp(marker, substring, sublen) == 0) return 1;

  return 0;
}


obstring * obstring_match_regex(const obstring *s, const char *regex){

  regex_t comp_regex;
  regmatch_t match;
  int errcode;
  char errbuf[REGEX_ERROR_BUFFER_SIZE];

  assert(s);
  assert(regex);

  /* compile regex */
  if((errcode = regcomp(&comp_regex, regex, REG_EXTENDED))){
    regerror(errcode, &comp_regex, errbuf, REGEX_ERROR_BUFFER_SIZE);
    fprintf(stderr, "obstring::obstring_match_regex recieved improper regex\n"
                    "Regex supplied: %s\nError string:%s\n", regex, errbuf);

    regfree(&comp_regex);
    return obstring_new("");
  }

  /* match regex, returning empty string if no match was found */
  if(regexec(&comp_regex, s->str, 1, &match, 0)){
    regfree(&comp_regex);
    return obstring_new("");
  }

  regfree(&comp_regex);

  /* return a new string containing the matching range */
  return obstring_copy_substring(s, match.rm_so, match.rm_eo - match.rm_so);
}


/* PRIVATE METHODS */

/* add arguments to complete initialization as needed, modify
 * obstring_Private.h as well if modifications are made */
obstring * obstring_create_default(void){

  static const char classname[] = "obstring";
  obstring *new_instance = malloc(sizeof(obstring));
  assert(new_instance != NULL);

  /* initialize base class data */
  ob_init_base((obj *)new_instance, &obstring_destroy, &obstring_hash,
               &obstring_compare, &obstring_display, classname);

  new_instance->str = malloc(sizeof(char));
  assert(new_instance->str);
  new_instance->str[0] = '\0';
  new_instance->length = 0;

  return new_instance;
}


ob_hash_t obstring_hash(const obj *to_hash){

  static int8_t init = 0;
  static ob_hash_t seed;

  obstring *instance = (obstring *)to_hash;
  ob_hash_t value;
  char *pos;

  assert(to_hash);
  assert(ob_has_class(to_hash, "obstring"));

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


int8_t obstring_compare(const obj *a, const obj *b){

  uint32_t i;
  const obstring *comp_a = (obstring *)a;
  const obstring *comp_b = (obstring *)b;

  assert(a);
  assert(b);
  assert(ob_has_class(a, "obstring"));
  assert(ob_has_class(b, "obstring"));

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


void obstring_display(const obj *str){
  assert(str != NULL);
  assert(ob_has_class(str, "obstring"));
  fprintf(stderr, "obstring with Contents:\n  %s\n", ((obstring *)str)->str);
}


void obstring_destroy(obj *to_dealloc){

  /* cast generic obj to obstring */
  obstring *instance = (obstring *)to_dealloc;

  assert(to_dealloc);
  assert(ob_has_class(to_dealloc, "obstring"));

  if(instance->str) free(instance->str);

  return;
}

/* DEFINE ADDITIONAL PRIVATE METHODS HERE */
