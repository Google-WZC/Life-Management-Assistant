#pragma once

#include <stdio.h>
#include <time.h>
#include <stdlib.h> //for exit
#include "LedgerStruc.h"

#define DATABASE_FILE_NAME ".Ledger202509.dat"

void fullfill_entry(struct database *);
void write_entry(struct database *);
void print_database(struct database *);
void init(struct database *, FILE *);
void restore_database(struct database *);
char UI_choose(struct database *);