#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "map.h"
#include "pokedex.h"
#include "evolutions.h"
#include "location.h"

/**
 * Text parsing functions
 *
 * Available functions:
 * pokedexCreateFromFile        - create a Pokedex type from an open text file
 * evolutionsCreateFromFile     - create an Evolutions map from an open text
 *                                file
 * locationsMapCreateFromFile   - create a Locations map from an open text file
 * parseArgs                    - Parse the main's argv, handling invalid usage
 * tokenizeCommand              - Split an input command into its basic parts.
 */

#define MAX_STR_LENGTH 250

/**
 * pokedexCreateFromFile - create a Pokedex from a text file
 *
 * @param file  - an open file
 *
 * Format of each line should be:
 *  <species>   <initial_cp>    <type> [<other_type> [...]]
 *
 * @return
 *  NULL - NULL arg or memory allocation error
 *  A pokedex type in case of success
 */
Pokedex pokedexCreateFromFile(FILE* file);

/**
 * evolutionsCreateFromFile - create an evlutions map from a text file
 *
 * @param file      - an open file
 * @param pokedex   - A valid Pokedex
 *
 * Format of each line should be:
 *  <species>   <evolved_species>   <level>
 *
 * @return
 *  NULL - NULL arg or memory allocation error
 *  A valid Evolutions type in case of success
 */
Evolutions evolutionsCreateFromFile(FILE* file, Pokedex pokedex);

/**
 * locationsCreateMapFromFile - create a locations map from a text file
 *
 * @param file      - an open file
 * @param pokedex   - a valid Pokedex
 * @param evolutions- a valid Evolutions map
 *
 * Format of each line should be one of these:
 *  <location_name>
 *  <location_name> <first_pokemon> [<second_pokemon> [...]]
 *  <location_name>; <neighbor> [<second_neighbor> [...]]
 *  <location_name> <one_or_more_pokemon>; <one_or_more_neighbors>
 *
 * @return
 *  NULL - NULL arg or memory allocation error
 *  A valid Map type (location_name->Location type) in case of success.
 */
Map locationsCreateMapFromFile(FILE* file, Pokedex pokedex,
                               Evolutions evolutions);

/**
 * parseArgs - Parse the main program's command-line arguments
 *
 * @param argc                  - number of arguments
 * @param argv                  - An array of arguments, length argc
 * @param pokedex_filename      - Will be filled with the filename of pokedex
 *                                data.
 * @param evolutions_filename   - Will be filled with the filename of
 *                                evolutions data.
 * @param locations_filename    - Will be filled with the filename of
 *                                locations data.
 * @param input_filename        - Will be filled with the filename of input
 *                                commands, or NULL if argument wasn't passed.
 * @param output_filename       - Will be filled with the filename to put the
 *                                output in, or NULL if argument wasn't passed.
 *
 * The *_filename parameters don't need to be pre-allocated. They will be set
 * to the corresponding word from the argv array.
 *
 * @return
 *  false - NULL arg or invalid argument format in argv
 *  true  - Successful parsing of arguments.
 */
bool parseArgs(int argc, char** argv, char** pokedex_filename,
               char** evolutions_filename, char** locations_filename,
               char** input_filename, char** output_filename);

/**
 * tokenizeCommand - Split an input command into its parts
 *
 * @param line      - The input command line
 * @param command   - Will be filled with the first word in the input line
 * @param subcommand- Will be filled with the second word in the input line.
 * @param args      - An array which will be filled with the words from the
 *                    rest of the input line. Should be allocated already.
 */
void tokenizeCommand(char* line, char** command, char** subcommand,
                     char** args);
#endif
