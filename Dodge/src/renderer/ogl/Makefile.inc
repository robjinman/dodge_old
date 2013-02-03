ifeq ($(PLATFORM), linux)
	OBJS+=$(BASE_DIR)/renderer/ogl/Colour.o \
		$(BASE_DIR)/renderer/ogl/NonTexturedAlphaMode.o \
		$(BASE_DIR)/renderer/ogl/Renderer.o \
		$(BASE_DIR)/renderer/ogl/RenderMode.o \
		$(BASE_DIR)/renderer/ogl/Texture.o \
		$(BASE_DIR)/renderer/ogl/TexturedAlphaMode.o
else
	ifeq ($(PLATFORM), windows)
		OBJS+=$(BASE_DIR)/renderer/ogl/Colour.o \
			$(BASE_DIR)/renderer/ogl/FixedFunctionMode.o \
			$(BASE_DIR)/renderer/ogl/Renderer.o \
			$(BASE_DIR)/renderer/ogl/RenderMode.o \
			$(BASE_DIR)/renderer/ogl/Texture.o
	endif
endif
