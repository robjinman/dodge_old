ifeq ($(PLATFORM), linux)
	include $(BASE_DIR)/renderer/ogles2.0/Makefile.inc
else
	ifeq ($(PLATFORM), windows)
		include $(BASE_DIR)/renderer/ogles2.0/Makefile.inc
	endif
endif
OBJS += $(BASE_DIR)/renderer/Camera.o \
	$(BASE_DIR)/renderer/Font.o \
	$(BASE_DIR)/renderer/SceneGraph.o
