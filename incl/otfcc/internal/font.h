#ifndef CARYLL_FONT_H
#define CARYLL_FONT_H

#include <otfcc/internal/sfnt.h>

struct _caryll_font;
typedef struct _caryll_font otfcc_Font;

#include <otfcc/internal/glyph-order.h>

#include <otfcc/internal/table/fvar.h>

#include <otfcc/internal/table/head.h>
#include <otfcc/internal/table/glyf.h>
#include <otfcc/internal/table/CFF.h>
#include <otfcc/internal/table/maxp.h>
#include <otfcc/internal/table/hhea.h>
#include <otfcc/internal/table/hmtx.h>
#include <otfcc/internal/table/hdmx.h>
#include <otfcc/internal/table/vhea.h>
#include <otfcc/internal/table/vmtx.h>
#include <otfcc/internal/table/OS_2.h>
#include <otfcc/internal/table/post.h>
#include <otfcc/internal/table/name.h>
#include <otfcc/internal/table/meta.h>

#include <otfcc/internal/table/cmap.h>
#include <otfcc/internal/table/cvt.h>
#include <otfcc/internal/table/fpgm-prep.h>
#include <otfcc/internal/table/gasp.h>
#include <otfcc/internal/table/VDMX.h>

#include <otfcc/internal/table/LTSH.h>
#include <otfcc/internal/table/VORG.h>

#include <otfcc/internal/table/GDEF.h>
#include <otfcc/internal/table/BASE.h>
#include <otfcc/internal/table/otl.h>

#include <otfcc/internal/table/CPAL.h>
#include <otfcc/internal/table/COLR.h>
#include <otfcc/internal/table/SVG.h>

#include <otfcc/internal/table/_TSI.h>
#include <otfcc/internal/table/TSI5.h>

typedef enum { FONTTYPE_TTF, FONTTYPE_CFF } otfcc_font_subtype;

struct _caryll_font {
	otfcc_font_subtype subtype;

	table_fvar *fvar;

	table_head *head;
	table_hhea *hhea;
	table_maxp *maxp;
	table_OS_2 *OS_2;
	table_hmtx *hmtx;
	table_post *post;
	table_hdmx *hdmx;

	table_vhea *vhea;
	table_vmtx *vmtx;
	table_VORG *VORG;

	table_CFF *CFF_;
	table_glyf *glyf;
	table_cmap *cmap;
	table_name *name;
	table_meta *meta;

	table_fpgm_prep *fpgm;
	table_fpgm_prep *prep;
	table_cvt *cvt_;
	table_gasp *gasp;
	table_VDMX *VDMX;

	table_LTSH *LTSH;

	table_OTL *GSUB;
	table_OTL *GPOS;
	table_GDEF *GDEF;
	table_BASE *BASE;

	table_CPAL *CPAL;
	table_COLR *COLR;
	table_SVG *SVG_;

	table_TSI *TSI_01;
	table_TSI *TSI_23;
	table_TSI5 *TSI5;

	otfcc_GlyphOrder *glyph_order;
};

extern caryll_ElementInterfaceOf(otfcc_Font) {
	caryll_RT(otfcc_Font);
	void (*consolidate)(otfcc_Font * font, const otfcc_Options *options);
	void *(*createTable)(otfcc_Font * font, const uint32_t tag);
	void (*deleteTable)(otfcc_Font * font, const uint32_t tag);
}
otfcc_iFont;

// Font builder interfaces
typedef struct otfcc_IFontBuilder {
	otfcc_Font *(*read)(void *source, uint32_t index, const otfcc_Options *options);
	void (*free)(struct otfcc_IFontBuilder *self);
} otfcc_IFontBuilder;
otfcc_IFontBuilder *otfcc_newOTFReader();
otfcc_IFontBuilder *otfcc_newJsonReader();

// Font serializer interface
typedef struct otfcc_IFontSerializer {
	void *(*serialize)(otfcc_Font *font, const otfcc_Options *options);
	void (*free)(struct otfcc_IFontSerializer *self);
} otfcc_IFontSerializer;
otfcc_IFontSerializer *otfcc_newJsonWriter();
otfcc_IFontSerializer *otfcc_newOTFWriter();

#endif
