default: PluginMain.cpp
	g++ -I. -I$(V8DIR) -I$(V8DIR)/include/ PluginMain.cpp -shared -fPIC -o v8.dll.so -lv8_monolith -L$(V8DIR)/out.gn/x64.release.sample/obj/ -pthread -std=c++14 -DV8_COMPRESS_POINTERS

install: default
	sudo mkdir -p $(INSTALLDIR)
	sudo cp -a v8.dll.so $(INSTALLDIR)/
	sudo cp -a $(V8DIR)/out.gn/x64.release.sample/icudtl.dat $(INSTALLDIR)/

.PHONY: v8 v8-deps
v8-deps:
	ln -fs /usr/share/zoneinfo/Europe/Dublin /etc/localtime && apt-get install -y tzdata sudo && DEBIAN_FRONTEND=noninteractive dpkg-reconfigure -f noninteractive tzdata
	(cd $(V8DIR) && ./build/install-build-deps.sh --no-nacl --no-arm)


$(V8DIR)/out.gn/x64.release.sample/icudtl.dat:
	(cd $(V8DIR) && ./tools/dev/v8gen.py x64.release.sample && ninja -C out.gn/x64.release.sample v8_monolith)


v8: $(V8DIR)/out.gn/x64.release.sample/icudtl.dat

