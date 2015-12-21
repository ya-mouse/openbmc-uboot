# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	= $(obj)lib$(SOC).o

COBJS	= timer.o
COBJS	+= reset.o
COBJS	+= mactest.o
COBJS	+= DRAM_SPI.o
COBJS	+= IO.o
COBJS	+= LIB.o
COBJS	+= MAC.o
COBJS	+= NCSI.o
COBJS	+= PCI_SPI.o
COBJS	+= PHY.o
COBJS	+= SPIM.o
COBJS	+= STDUBOOT.o
COBJS	+= PLLTESTU.o
COBJS	+= TRAPTEST.o
COBJS	+= STRESS.o

SRCS	:= $(START:.o=.S) $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS) $(SOBJS))
START	:= $(addprefix $(obj),$(START))

all:	$(obj).depend $(LIB)

$(LIB):	$(OBJS)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
