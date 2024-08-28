#!/bin/bash

cppcheck --enable=all --inconclusive --suppress=missingInclude --output-file=output.xml --xml ../2048

cppcheck-htmlreport --file=output.xml --report-dir=report
