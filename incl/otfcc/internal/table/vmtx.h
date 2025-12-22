#ifndef CARYLL_INCLUDE_TABLE_VMTX_H
#define CARYLL_INCLUDE_TABLE_VMTX_H

#include <otfcc/internal/table/table-common.h>

#include <otfcc/internal/table/vhea.h>
#include <otfcc/internal/table/maxp.h>

typedef struct {
	length_t advanceHeight;
	pos_t tsb;
} vertical_metric;

typedef struct {
	OWNING vertical_metric *metrics;
	pos_t *topSideBearing;
} table_vmtx;

extern caryll_RefElementInterface(table_vmtx) table_iVmtx;

#endif
