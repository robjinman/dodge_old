OBJS+=$(BASE_DIR)/renderer/ogl/Colour.o \
	$(BASE_DIR)/renderer/ogl/Renderer.o \
	$(BASE_DIR)/renderer/ogl/RenderMode.o \
	$(BASE_DIR)/renderer/ogl/Texture.o
#ifeq ($(GL_MODE), fixed_pipeline)
	OBJS += $(BASE_DIR)/renderer/ogl/FixedFunctionMode.o
#else
	OBJS += $(BASE_DIR)/renderer/ogl/NonTexturedAlphaMode.o \
		$(BASE_DIR)/renderer/ogl/TexturedAlphaMode.o
#endif
