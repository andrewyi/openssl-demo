# actually when we build openssl with `shared` opiton or no options at all,
# static library will still be generated
# so we could seperate our build process from openssl

OPENSSL_PATH := ../openssl1.1.1/openssl-OpenSSL_1_1_1c

config_flags := 

dynamic: objects
	g++ -L$(OPENSSL_PATH) main.o auth.o -lcrypto -o main
	@echo "please note that use \`LD_LIBRARY_PATH=$(OPENSSL_PATH) ./main\` to execute"

static: objects
	# g++ -L$(OPENSSL_PATH) main.o auth.o -lcrypto -pthread -ldl -o main # set `config_flags := no-shared` if you wanna build static binary this way
	g++ main.o auth.o $(OPENSSL_PATH)/libcrypto.a -pthread -ldl -o main

objects:
	g++ -I.. -I$(OPENSSL_PATH)/include -c main.cpp -o main.o
	g++ -I.. -I$(OPENSSL_PATH)/include -c ../auth.cpp -o auth.o

openssl:
	cd $(OPENSSL_PATH) && ./config $(config_flags) && make build_libs

clean:
	rm -f main *.o

distclean: clean
	cd $(OPENSSL_PATH) && make clean && make distclean

.PHONY: dynamic static objects openssl clean distclean
