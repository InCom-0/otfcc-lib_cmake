#ifndef CARYLL_INCLUDE_TABLE_HMTX_H
#define CARYLL_INCLUDE_TABLE_HMTX_H

#include <otfcc/internal/table/table-common.h>

#include <otfcc/internal/table/hhea.h>
#include <otfcc/internal/table/maxp.h>

typedef struct {
	length_t advanceWidth;
	pos_t lsb;
} horizontal_metric;

typedef struct {
	// Horizontal metrics
	OWNING horizontal_metric *metrics;
	OWNING pos_t *leftSideBearing;
} table_hmtx;
extern caryll_RefElementInterface(table_hmtx) table_iHmtx;

#endif
