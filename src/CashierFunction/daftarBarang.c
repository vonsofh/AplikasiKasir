#define _GNU_SOURCE // Disable implicit declaration warning

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../vendor/sandbird/sandbird.h"
#include "../../vendor/sqlite3/sqlite3.h"
#include "../sqliteFunction.h"
#include "../utils/utils.h"

#include "cashierFunction.h"

int daftarBarang(sb_Event* e) {
    FILE* fp = fopen("database/daftarBarang.db", "rb");
    if (fp) {
        fclose(fp);
        char* errMsg;
        sqlite3* db;
        SQLRow allRows = {0};

        sqlite3_open("database/daftarBarang.db", &db);

        if (sqlite3_exec(db, "SELECT rowid,* from daftarBarang", RowBack, &allRows, &errMsg) != SQLITE_OK) {
            freeRowBack(&allRows);

            if (!isStr(errMsg, "no such table", 0)) {
                sb_send_status(e->stream, 403, "Ada yang salah pada AplikasiKasir, harap hubungi Pemilik");
                printf("[ERROR] Something wrong in SQLite at daftarBarang.c: %s\n", errMsg);
            }
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return SB_RES_OK;
        }

        sb_send_status(e->stream, 200, "OK");
        sb_write(e->stream, allRows.rows, allRows.totalChar);
        freeRowBack(&allRows);
        sqlite3_close(db);
    }
    return SB_RES_OK;
}
