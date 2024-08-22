#!/bin/bash

SOURCE_DIR="../2048"

cppcheck --enable=all --inconclusive --suppress=missingInclude --output-file=output_improved.xml --xml ../2048_improved

cppcheck-htmlreport --file=output_improved.xml --report-dir=report_improved
