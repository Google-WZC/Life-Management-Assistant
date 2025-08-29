#pragma once

#include <stdio.h>
#include <time.h>
#include <stdlib.h> //for exit
#include "LedgerStruc.h"
#include "utils.h"


#define DATABASE_FILE_NAME ".Ledger202509.dat"

void init(struct database *, FILE *);
void add_entry(struct database *);
void subtitute_existing_entry(struct database *);
void delete_last_entry(struct database *);
void print_database(struct database *);
void restore_database(struct database *);
void associate_database(struct database *);
char UI_choose(struct database *);