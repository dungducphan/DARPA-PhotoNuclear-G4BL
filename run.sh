#!/bin/bash

setup_g4bl

thicknesses=(2 4 6 8 10)

mkdir ./EMom_100
cd ./EMom_100
for thickness in "${thicknesses[@]}"; do
    echo "Processing thickness: $thickness"
    mkdir ./Thickness_$thickness
    cd ./Thickness_$thickness
    g4bl ../../DARPA_PhotoNuclear.g4bl EMOM=100 THICKNESS=$thickness &
    cd ..
done
cd ..

mkdir ./EMom_80
cd ./EMom_80
for thickness in "${thicknesses[@]}"; do
    echo "Processing thickness: $thickness"
    mkdir ./Thickness_$thickness
    cd ./Thickness_$thickness
    g4bl ../../DARPA_PhotoNuclear.g4bl EMOM=80 THICKNESS=$thickness &
    cd ..
done
cd ..

