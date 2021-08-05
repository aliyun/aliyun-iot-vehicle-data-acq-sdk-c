OUTPUT_DIR = $(PWD)/build


# default compile output
all :
#	$(MAKE) aliyun-iot-vehicle-data-acq-sdk-c

# deps libary
prepare :
	tar xzvf msgpack-3.3.0.tar.gz
	cd msgpack-3.3.0 &&	cmake . && make clean && make DESTDIR=$(OUTPUT_DIR) install

	tar xzvf curl-7.77.0.tar.gz
	cd curl-7.77.0 && ./configure CC=arm-linux-gnueabi-gcc --host=arm-linux-gnueabi --prefix=/usr/bin/arm-linux-gnueabi --enable-static  --without-ssl --without-zlib && make clean && make DESTDIR=$(OUTPUT_DIR) install


	tar xzvf libmicrohttpd-0.9.72.tar.gz
	cd libmicrohttpd-0.9.72 && ./configure CC=arm-linux-gnueabi-gcc --host=arm-linux-gnueabi --prefix=/usr/bin/arm-linux-gnueabi --enable-static --disable-doc --disable-examples --disable-https --disable-largefile --disable-dauth --disable-httpupgrade --disable-curl && make clean && make DESTDIR=$(OUTPUT_DIR) install

## clean tempory compile resource
#clean:
#	$(MAKE) -C src -f leda_sdk_c.mk clean
#	$(MAKE) -C demo -f demo.mk clean
#
#install:
#	$(MAKE) -C demo -f demo.mk install
#

## delete compile resource
#distclean: clean
#	-$(RM) -r ./build
#	-$(RM) -r ./deps/cJSON-1.7.7/
#	-$(RM) -r ./deps/dbus-1.10.18/
#	-$(RM) -r ./deps/libexpat/
#
#.PHONY: deps demo src
