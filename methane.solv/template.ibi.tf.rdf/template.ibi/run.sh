#!/bin/bash

source env.sh
export GMX_NB_GENERIC=1

echo 'running csg_inverse --options "settings.xml"'
GMX_NB_GENERIC=1 csg_inverse --options settings.xml

