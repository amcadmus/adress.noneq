#!/bin/bash

join rf.rc1.2.long/nvt/rdf.0.01.xvg spme.tune.nvt.smooth.20ns/nvt/rdf.0.01.xvg   > tmp.out; mv -f tmp.out rdf.all.0.01.out
join rdf.all.0.01.out zm.l1.alpha0.00.rc1.2.long/nvt/rdf.0.01.xvg   > tmp.out; mv -f tmp.out rdf.all.0.01.out
join rdf.all.0.01.out zm.l2.alpha0.00.rc1.2.long/nvt/rdf.0.01.xvg   > tmp.out; mv -f tmp.out rdf.all.0.01.out
join rdf.all.0.01.out zm.l3.alpha0.00.rc1.2.long/nvt/rdf.0.01.xvg   > tmp.out; mv -f tmp.out rdf.all.0.01.out
join rdf.all.0.01.out zm.l4.alpha0.00.rc1.2.long/nvt/rdf.0.01.xvg   > tmp.out; mv -f tmp.out rdf.all.0.01.out
