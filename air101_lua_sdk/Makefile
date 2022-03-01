#for firmware download
#wm_tool -c $(COM_PORT) -rs rts -ds 2000000 -dl AIR101.fls

DIR      ?= app/led
COM_PORT ?= ttyUSB0
LUA_SRC  :=$(wildcard  $(foreach D, $(DIR), $D/*.lua))
LUA_BC   :=$(LUA_SRC:%.lua=%.luac)
LUA_DEBUG = -s
LUAC     ?= ./tools/luac
WM_TOOL  ?= ./tools/wm_tool
# 50 means 50K，less than 336k for Air101
LFS_SIZE ?= 50


.PHONY: run

.PHONY: download

.PHONY: monitor


run: $(LUA_BC)
	cd tools && ./mklfs -size $(LFS_SIZE)
	mv ./tools/disk.fs script.bin
	$(WM_TOOL) -b  script.bin -it 1 -fc 0 -ih 20008000  -ra 81E0000 -ua 0 -nh 0  -un 0 -o script
	rm script.bin
	$(WM_TOOL) -c $(COM_PORT) -rs rts -ds 2000000 -dl script.img -ua 81E0000

download:
	$(WM_TOOL) -c $(COM_PORT) -rs rts -ds 2000000 -dl script.img -ua 81E0000

monitor:
	$(WM_TOOL) -c $(COM_PORT) -rs rts -sl str -ws 921600

%.luac: %.lua
	$(LUAC) $(LUA_DEBUG) -o $@  $<
	mv $@ ./tools/disk


.PHONY: clean
clean:
	rm script.img ./tools/disk/*.luac
