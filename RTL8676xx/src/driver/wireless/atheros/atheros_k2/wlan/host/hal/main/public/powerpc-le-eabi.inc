#
# Copyright (c) 2002-2004 Sam Leffler, Errno Consulting
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer,
#    without modification.
# 2. Redistributions in binary form must reproduce at minimum a disclaimer
#    similar to the "NO WARRANTY" disclaimer below ("Disclaimer") and any
#    redistribution must be conditioned upon including a substantially
#    similar Disclaimer requirement for further binary redistribution.
# 3. Neither the names of the above-listed copyright holders nor the names
#    of any contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
#
# NO WARRANTY
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF NONINFRINGEMENT, MERCHANTIBILITY
# AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
# THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR SPECIAL, EXEMPLARY,
# OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
# IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGES.
#
# $Id: //depot/sw/carriersrc/branches/madwifi_pdf_dev/src/madwifi/hal/main/public/powerpc-le-eabi.inc#1 $
#

#
# Compilation configuration for building little-endian PowerPC/powerpc-eabi.
#

#
# Built with GNU cross-devel tools:
#
# PREFIX=/pub/gnu
# BINUTILS=binutils-2.14
# GCC=gcc-3.3.2
# target=powerpc-eabi
#
# ${BINUTILS}/configure --target=$target --prefix=${PREFIX}
# ${GCC}/configure --target=$target   --prefix=${PREFIX} \
#	--enable-languages=c   --with-gnu-as --with-gnu-ld \
#	--with-newlib   --with-gxx-include-dir=${PREFIX}/$target/include
#
ifndef TOOLPREFIX
TOOLPREFIX=	/pub/gnu/bin/powerpc-eabi-
endif
#
CC=	${TOOLPREFIX}gcc
LD=	${TOOLPREFIX}ld
STRIP=	${TOOLPREFIX}strip
OBJCOPY=${TOOLPREFIX}objcopy
NM=	${TOOLPREFIX}nm

LDOPTS=	-EL
COPTS+=	-DAH_BYTE_ORDER=AH_LITTLE_ENDIAN -DAH_REGOPS_FUNC
COPTS+= -mlittle-endian
# NB: explicitly disable multiple and string instructions for little-endian
COPTS+= -msoft-float -ffixed-r2 -mno-multiple -mno-string
