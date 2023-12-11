#pragma once

int goToLine(FILE *file, int lineNum) {
    rewind(file);

    char buffer[61];

    for (int i = 1; i < lineNum; ++i)
        if (fgets(buffer, sizeof(buffer), file) == NULL) return 0;

    return 1;
}
