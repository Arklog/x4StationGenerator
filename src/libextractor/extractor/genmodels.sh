#!/usr/bin/env bash

OUTPUT_DIR="models"

xsdcpp xsdfiles/modulegroups.xsd -o ${OUTPUT_DIR}
xsdcpp xsdfiles/modules.xsd -o ${OUTPUT_DIR} -e xsdcpp
xsdcpp xsdfiles/t.xsd -o ${OUTPUT_DIR} -e xsdcpp
xsdcpp xsdfiles/waregroups.xsd -o ${OUTPUT_DIR} -e xsdcpp
xsdcpp xsdfiles/wares.xsd -o ${OUTPUT_DIR} -e xsdcpp