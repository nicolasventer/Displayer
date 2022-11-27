# Copyright(c) Nicolas VENTER All rights reserved.

filenames = ['Displayer.hpp', 'extra/BoxDisplayer.hpp', 'extra/CsvDisplayer.hpp',
             'extra/ExtraDisplayer.hpp', 'extra/JsonDisplayer.hpp']
with open('AllDisplayers.hpp', 'w') as outfile:
    with open('ArrayConverter.hpp') as infile:
        for line in infile:
            if line == '// ============================================================\n':
                break
            outfile.write(line)
    for fname in filenames:
        with open(fname) as infile:
            for line in infile:
                if line in ['// Copyright (c) Nicolas VENTER All rights reserved.\n', '#pragma once\n', '#include "../Displayer.hpp"\n', '#include "ArrayConverter.hpp"\n']:
                    continue
                if line == '// ============================================================\n':
                    break
                outfile.write(line)
    outfile.write('\n')
    outfile.write(
        '// ============================================================\n')
    outfile.write(
        '// ============================================================\n')
    outfile.write(
        '// ===================== Implementations ======================\n')
    outfile.write(
        '// ============================================================\n')
    outfile.write(
        '// ============================================================\n')
    outfile.write('\n')
    with open('ArrayConverter.hpp') as infile:
        lineFound = 0
        for line in infile:
            if lineFound == 2:
                outfile.write(line)
            elif line == '// ============================================================\n':
                lineFound = 1
            elif lineFound == 1 and line == '\n':
                lineFound = 2
    for fname in filenames:
        with open(fname) as infile:
            lineFound = 0
            for line in infile:
                if line in ['// Copyright (c) Nicolas VENTER All rights reserved.\n', '#pragma once\n', '#include "../Displayer.hpp"\n', '#include "ArrayConverter.hpp"\n']:
                    continue
                if lineFound == 2:
                    outfile.write(line)
                elif line == '// ============================================================\n':
                    lineFound = 1
                elif lineFound == 1 and line == '\n':
                    lineFound = 2
