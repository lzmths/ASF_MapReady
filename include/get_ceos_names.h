/*
get_ceos_names.h

Functions that match CEOS files with each other using their extensions and
testing to see if the files actually exist

*/

#ifndef _GET_CEOS_NAMES_H_
#define _GET_CEOS_NAMES_H_


/* Enum & strings for ceos metadata file extensions */
typedef enum {
  NO_CEOS_METADATA=0,
  CEOS_L,
  CEOS_LDR,
  CEOS_lea,
  NUM_CEOS_METADATA_EXTS
} ceos_metadata_ext_t;
extern const char ceos_metadata_extensions[][8];

/* Enum & strings for ceos data file extensions */
typedef enum {
  NO_CEOS_DATA=0,
  CEOS_D,
  CEOS_RAW,
  CEOS_dat,
  NUM_CEOS_DATA_EXTS
} ceos_data_ext_t;
extern const char ceos_data_extensions[][8];

/* Enum for pairs of ceos extensions */
typedef enum {
   NO_CEOS_FILE_PAIR=0, /* Maybe an individual match, but not a pair */
   CEOS_D_L_PAIR,       /* .D   data and .L   meta */
   CEOS_RAW_LDR_PAIR,   /* .RAW data and .LDR meta */
   CEOS_dat_lea_PAIR,   /* dat. data and lea. meta */
   NUM_CEOS_FILE_PAIRS
} ceos_file_pairs_t;

/* Returns TRUE if the file exists and has an accepted ASF CEOS metadata file
 * extension  */
int has_ceos_metadata_extension(const char *ceosName);

/* Given the name of a file (potentially with path in front of it), determine
   if it is a CEOS leader file (depending on our accepted CEOS extensions). If
   so populate metaName with the appropriate name and return the appropriate
   ENUM ceos_metadata_ext_t value.  */
ceos_metadata_ext_t get_ceos_metadata_name(const char *ceosName, char *metaName);

/* If get_ceos_metadata_name fails to find a file, then exit the program.  */
ceos_metadata_ext_t require_ceos_metadata(const char *ceosName, char *metaName);

/* Given the name of a file (potentially with path in front of it), determine
   if it is a CEOS data file (depending on our accepted CEOS extensions). If
   so, populate dataName with the appropriate name and return the appropriate
   ENUM ceos_data_ext_t value.  */
ceos_data_ext_t get_ceos_data_name(const char *ceosName, char *dataName);

/* If get_ceos_data_name fails to find a file, then exit the program.  */
ceos_data_ext_t require_ceos_data(const char *ceosName, char *dataName);

/* Given the name of a file (potentially with path in front of it), determine
   if it is one of a CEOS file pair (depending on our accepted CEOS file
   extensions). If so populate dataName & metaName with the appropriate names
   and return the appropriate ENUM ceos_file_pairs_t value.  */
ceos_file_pairs_t get_ceos_names(const char *ceosName, char *dataName,
                                 char *metaName);

/* Do as get_ceos_names unless there is no pair in which case exit the program
   with a failure.  */
ceos_file_pairs_t require_ceos_pair(const char *ceosName, char *dataName,
                                    char *metaName);


#endif
