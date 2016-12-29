#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <map.h>
#include <pokedex.h>
#include <evolutions.h>
#include <location.h>

#define MAX_STR_LENGTH 250

Pokedex createPokedexFromFile(FILE* file);
Evolutions createEvolutionsFromFile(FILE* file, Pokedex pokedex);
Map createLocationsMapFromFile(FILE* file, Pokedex pokedex,
                               Evolutions evolutions);
bool parseArgs(int argc, char** argv, char** pokedex_filename,
               char** evolutions_filename, char** locations_filename,
               char** input_filename, char** output_filename);
void tokenizeCommand(char* line, char** command, char** subcommand,
                     char** args);
#endif
