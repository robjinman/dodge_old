include $(BASE_DIR)/$(PLATFORM)/Makefile.inc
include $(BASE_DIR)/math/Makefile.inc
OBJS += $(BASE_DIR)/Animation.o \
	$(BASE_DIR)/AnimFrame.o \
	$(BASE_DIR)/Colour.o \
	$(BASE_DIR)/EEvent.o \
	$(BASE_DIR)/Entity.o \
	$(BASE_DIR)/EntityAnimations.o \
	$(BASE_DIR)/EventManager.o \
	$(BASE_DIR)/Exception.o \
	$(BASE_DIR)/globals.o \
	$(BASE_DIR)/Graphics2d.o \
	$(BASE_DIR)/IWinIO.o \
	$(BASE_DIR)/Range.o \
	$(BASE_DIR)/Renderer.o \
	$(BASE_DIR)/Shape.o \
	$(BASE_DIR)/Sprite.o \
	$(BASE_DIR)/StackAllocator.o \
	$(BASE_DIR)/StringId.o \
	$(BASE_DIR)/Texture.o \
	$(BASE_DIR)/WorldSpace.o
