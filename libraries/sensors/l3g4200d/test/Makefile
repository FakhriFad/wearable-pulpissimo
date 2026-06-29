# ============================================================================
# ICDeC - L3G4200D Sensor Test Makefile
#
# Build dan run aplikasi test L3G4200D pada ICDeC PULPissimo FPGA board.
#
# Usage (Unit Test - host):
#   make unit        Build dan jalankan unit test (mock I2C, tanpa hardware)
#   make unit_build  Hanya build unit test
#   make unit_run    Hanya jalankan unit test
#
# Usage (FPGA Test):
#   make all          Build test untuk FPGA
#   make run platform=fpga  Run test pada FPGA
#   make clean        Bersihkan build artifacts
# ============================================================================

SENSOR      = l3g4200d
PULP_APP    = test_l3g4200d
PULP_APP_C_SRCS = test_l3g4200d.c ../l3g4200d.c

# Include Arduino time functions (provides micros() needed by I2C driver)
PULP_APP_C_SRCS += $(PULPRT_HOME)/arduino/cores/arduino_time.c

# Compiler flags
PULP_CFLAGS = -O3 -g -I.. -I$(PULPRT_HOME)/arduino/cores

# Include pulp-runtime build rules
include $(PULPRT_HOME)/rules/pulp.mk

# ============================================================================
# Unit Test (host-side, mock I2C)
# ============================================================================

UNIT_TEST_BIN = unit_test_l3g4200d
UNIT_TEST_SRC = unit_test_l3g4200d.c

unit: unit_build unit_run

unit_build:
	gcc -o $(UNIT_TEST_BIN) $(UNIT_TEST_SRC) \
		-I.. -Imock -lm
	@echo "Build selesai: ./$(UNIT_TEST_BIN)"

unit_run:
	./$(UNIT_TEST_BIN)

unit_clean:
	rm -f $(UNIT_TEST_BIN)

.PHONY: unit unit_build unit_run unit_clean
