CXX:=clang++-13
# List my sources
COMMON_SOURCES := Mandelbrot_functions_fast.cpp Mandelbrot.cpp
SPEED_TEST_SOURCES := main_speed_tests.cpp Mandelbrot_point_simple.cpp speed_tests_fast.cpp
RENDER_SOURCES := main_render.cpp Mandelbrot_create_png.cpp
ALL_SOURCES := $(COMMON_SOURCES) $(SPEED_TEST_SOURCES) $(RENDER_SOURCES)

CPP_FLAGS_BASE:= -iquote . -Wall -Wextra -std=c++20 -mavx2 -mfma -mtune=haswell -D POINTS_COUNT=1
RELEASE_OBJ_DIR:=./obj/cpp
RELEASE_CXXFLAGS+= $(CPP_FLAGS_BASE) -O3

OBJ_DIR_LIST:= X/cpp

# destination path macro we'll use below
df_speed_tests = $(RELEASE_OBJ_DIR)/$(*F)
# create a list of auto dependencies
RELEASE_AUTODEPS:= $(patsubst %.cpp,$(RELEASE_OBJ_DIR)/%.d,$(ALL_SOURCES))

.PHONY: speed_tests render_images clean

LIBRARIES = -lpng -pthread

all : speed_tests render_images

speed_tests: dirs $(COMMON_SOURCES:%.cpp=$(RELEASE_OBJ_DIR)/%.o) $(SPEED_TEST_SOURCES:%.cpp=$(RELEASE_OBJ_DIR)/%.o)
	@echo "Linking speed_tests"
	@$(CXX) -o mandelbrot_speed_tests $(RELEASE_CXXFLAGS) $(patsubst %.cpp,$(RELEASE_OBJ_DIR)/%.o,$(SPEED_TEST_SOURCES)) $(patsubst %.cpp,$(RELEASE_OBJ_DIR)/%.o,$(COMMON_SOURCES)) $(LIBRARIES)

render_images: dirs $(COMMON_SOURCES:%.cpp=$(RELEASE_OBJ_DIR)/%.o) $(RENDER_SOURCES:%.cpp=$(RELEASE_OBJ_DIR)/%.o)
	@echo "Linking render_images"
	@$(CXX) -o mandelbrot_render_images $(RELEASE_CXXFLAGS) $(patsubst %.cpp,$(RELEASE_OBJ_DIR)/%.o,$(RENDER_SOURCES)) $(patsubst %.cpp,$(RELEASE_OBJ_DIR)/%.o,$(COMMON_SOURCES)) $(LIBRARIES)

dirs:
	@mkdir -p $(subst X,$(RELEASE_OBJ_DIR)/..,$(OBJ_DIR_LIST))

clean:
	rm -rf obj/

# include by auto dependencies
-include $(RELEASE_AUTODEPS)

$(RELEASE_OBJ_DIR)/%.o: %.cpp
	@# Build the dependency file
	@$(CXX) -MM -MP -MT $(df_speed_tests).o -MT $(df_speed_tests).d $(RELEASE_CXXFLAGS) $< > $(df_speed_tests).d
	@# Compile the object file
	@echo " C++ : " $< " => " $@
	@$(CXX) -c $< $(RELEASE_CXXFLAGS) -o $@

# Fast math for this one
$(RELEASE_OBJ_DIR)/Mandelbrot_functions_fast.o: Mandelbrot_functions_fast.cpp
	@# Build the dependency file
	@$(CXX) -MM -MP -MT $(df_speed_tests).o -MT $(df_speed_tests).d $(RELEASE_CXXFLAGS) $< > $(df_speed_tests).d
	@# Compile the object file
	@echo " C++(fast) : " $< " => " $@
	@$(CXX) -c Mandelbrot_functions_fast.cpp $(RELEASE_CXXFLAGS) -ffast-math -o $(RELEASE_OBJ_DIR)/Mandelbrot_functions_fast.o

# Fast math for this one
$(RELEASE_OBJ_DIR)/speed_tests_fast.o: speed_tests_fast.cpp
	@# Build the dependency file
	@$(CXX) -MM -MP -MT $(df_speed_tests).o -MT $(df_speed_tests).d $(RELEASE_CXXFLAGS) $< > $(df_speed_tests).d
	@# Compile the object file
	@echo " C++(fast) : " $< " => " $@
	@$(CXX) -c speed_tests_fast.cpp $(RELEASE_CXXFLAGS) -ffast-math -o $(RELEASE_OBJ_DIR)/speed_tests_fast.o

# Fast math for this one
$(RELEASE_OBJ_DIR)/main_render.o: main_render.cpp
	@# Build the dependency file
	@$(CXX) -MM -MP -MT $(df_speed_tests).o -MT $(df_speed_tests).d $(RELEASE_CXXFLAGS) $< > $(df_speed_tests).d
	@# Compile the object file
	@echo " C++(fast) : " $< " => " $@
	@$(CXX) -c main_render.cpp $(RELEASE_CXXFLAGS) -ffast-math -o $(RELEASE_OBJ_DIR)/main_render.o
