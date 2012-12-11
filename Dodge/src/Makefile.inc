include $(BASE_DIR)/$(PLATFORM)/Makefile.inc
include $(BASE_DIR)/math/Makefile.inc
include $(BASE_DIR)/renderer/Makefile.inc
include $(BASE_DIR)/ui/Makefile.inc
include $(BASE_DIR)/xml/Makefile.inc
OBJS += $(BASE_DIR)/Animation.o \
	$(BASE_DIR)/AnimFrame.o \
	$(BASE_DIR)/AssetManager.o \
	$(BASE_DIR)/Box2dPhysics.o \
	$(BASE_DIR)/EEvent.o \
	$(BASE_DIR)/Entity.o \
	$(BASE_DIR)/EntityAnimations.o \
	$(BASE_DIR)/EntityTransformations.o \
	$(BASE_DIR)/EventManager.o \
	$(BASE_DIR)/Exception.o \
	$(BASE_DIR)/globals.o \
	$(BASE_DIR)/KvpParser.o \
	$(BASE_DIR)/ShapeFactory.o \
	$(BASE_DIR)/Range.o \
	$(BASE_DIR)/Sprite.o \
	$(BASE_DIR)/StackAllocator.o \
	$(BASE_DIR)/StringId.o \
	$(BASE_DIR)/TextEntity.o \
	$(BASE_DIR)/Transformation.o \
	$(BASE_DIR)/TransFrame.o \
	$(BASE_DIR)/WorldSpace.o
