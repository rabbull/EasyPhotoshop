//
// Created by karl on 30/5/2020.
//

#ifndef EASYPHOTOSHOP_BMP_H
#define EASYPHOTOSHOP_BMP_H

#include "input-format-table.h"
#include "output-format-table.h"

void bmp_init(FileIOInputFormatTable *input_table, FileIOOutputFormatTable *output_table);

void bmp_exit(FileIOInputFormatTable *input_table, FileIOOutputFormatTable *output_table);

#endif //EASYPHOTOSHOP_BMP_H
