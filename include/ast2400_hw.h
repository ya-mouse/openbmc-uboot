#ifndef _AST2400_HW_H_
#define _AST2400_HW_H_

#include <config.h>

/* U-Boot does not use MMU. So no mapping */
#define IO_ADDRESS(x)	(x)
#define MEM_ADDRESS(x)  (x)

#include <ast2400/hwdef.h>
#include <ast2400/hwmap.h>
#include <ast2400/hwreg.h>

#endif /* _AST2400_HW_H_ */
