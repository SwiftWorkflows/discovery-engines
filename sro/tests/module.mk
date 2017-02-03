
include tests/f/module.mk
include tests/c/module.mk

tests: tests-f tests-c

.PHONY: tests
