ifeq ($(PLATFORM), linux)
	include $(BASE_DIR)/renderer/ogl/Makefile.inc
else
	ifeq ($(PLATFORM), windows)
		include $(BASE_DIR)/renderer/ogl/Makefile.inc
	endif
endif
OBJS += $(BASE_DIR)/renderer/Camera.o \
	$(BASE_DIR)/renderer/Font.o \
	$(BASE_DIR)/renderer/Model.o \
	$(BASE_DIR)/renderer/SceneGraph.o
