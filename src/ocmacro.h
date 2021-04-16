#ifndef __OC_MACRO_H
#define __OC_MACRO_H

#define OCAPI extern

#define OC_DELETE(p) do{free(p); p = NULL;}while(0)

#endif
