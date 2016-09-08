#
# Copyright(C) 2016
#      Matheus A. Souza (matheusalcantarasouza@gmail.com)
#

CFLAGS = -O0
LIBFLAGS = /usr/local/lib/libpapi.a

all: papiCacheWrapper

papiCacheWrapper:
	$(CC) $(CFLAGS) papiCacheWrapper.c $(LIBFLAGS) -o papiCacheWrapper

clean:
	rm -rf papiCacheWrapper
